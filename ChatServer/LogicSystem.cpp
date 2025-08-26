#include "LogicSystem.h"
#include "StatusGrpcClient.h"
#include "MysqlMgr.h"
#include "const.h"
#include "FileUploadManager.h"
#include "CallManager.h"
#include "ConfigMgr.h"
#include "KafkaMgr.h"
#include "CServer.h"

using namespace std;

LogicSystem::LogicSystem():_b_stop(false){
    // 初始化Kafka
    auto& cfg = ConfigMgr::Inst();
    bool enableKafka = cfg["Kafka"]["Enable"] == "1";
    if (enableKafka) {
        KafkaMgr::Inst().setEnabled(true);
        KafkaMgr::Inst().init(cfg["Kafka"]["Brokers"], cfg["Kafka"]["Group"]);
        // 启动消费线程：将Kafka中的消息重新入本地队列处理
        KafkaMgr::Inst().consume(cfg["Kafka"]["TopicIngress"], [this](const std::string& key, const std::string& payload){
            Json::Value root;
            Json::Reader reader;
            if (!reader.parse(payload, root)) return;
            short msgid = static_cast<short>(root["msgid"].asInt());
            std::string uuid = root.get("uuid", "").asString();
            std::string pl = root.get("payload", "").asString();
            std::shared_ptr<CSession> session = nullptr;
            if (!uuid.empty() && g_server) {
                session = g_server->FindSession(uuid);
            }
            auto dummy = std::make_shared<RecvNode>((short)pl.size(), msgid);
            memcpy(dummy->_data, pl.data(), pl.size());
            EnqueueLocal(std::make_shared<LogicNode>(session, dummy));
        });
        // 订阅控制主题：跨服踢人等
        KafkaMgr::Inst().consume(cfg["Kafka"]["TopicControl"], [this](const std::string& key, const std::string& payload){
            Json::Value root; Json::Reader reader; if (!reader.parse(payload, root)) return;
            std::string cmd = root.get("cmd", "").asString();
            if (cmd == "kick") {
                int uid = root.get("uid", 0).asInt();
                // 向该用户所有连接发送下线通知（这里按 uid->session 路由）
                auto target = findSessionByUid(uid);
                if (target) {
                    Json::Value rsp; rsp["error"] = 0; rsp["uid"] = uid;
                    target->Send(rsp.toStyledString(), MSG_NOTIFY_OFFLINE);
                    target->Close();
                }
            }
        });
    }
    RegisterCallBacks();
    _worker_thread = std::thread (&LogicSystem::DealMsg, this);
}

LogicSystem::~LogicSystem(){
    _b_stop = true;
    _consume.notify_one();
    _worker_thread.join();
}

void LogicSystem::PostMsgToQue(shared_ptr < LogicNode> msg) {
    if (KafkaMgr::Inst().enable()) {
        // 生产到 Kafka：包含会话uuid、msgid、payload
        Json::Value wrapper;
        wrapper["uuid"] = msg->_session ? msg->_session->GetUuid() : "";
        wrapper["msgid"] = msg->_recvnode->_msg_id;
        wrapper["payload"] = std::string(msg->_recvnode->_data, msg->_recvnode->_cur_len);
        Json::FastWriter w;
        std::string data = w.write(wrapper);
        KafkaMgr::Inst().produce(ConfigMgr::Inst()["Kafka"]["TopicIngress"], wrapper["uuid"].asString(), data);
        return;
    } else {
        std::unique_lock<std::mutex> unique_lk(_mutex);
        _msg_que.push(msg);
        if (_msg_que.size() == 1) {
            unique_lk.unlock();
            _consume.notify_one();
        }
    }
}

void LogicSystem::EnqueueLocal(shared_ptr < LogicNode> msg) {
    std::unique_lock<std::mutex> unique_lk(_mutex);
    _msg_que.push(msg);
    if (_msg_que.size() == 1) {
        unique_lk.unlock();
        _consume.notify_one();
    }
}

void LogicSystem::DealMsg() {
    for (;;) {
        std::unique_lock<std::mutex> unique_lk(_mutex);
        //ж϶Ϊȴͷ
        while (_msg_que.empty() && !_b_stop) {
            _consume.wait(unique_lk);
        }

        //жǷΪר״̬߼ִ˳ѭ
        if (_b_stop ) {
            while (!_msg_que.empty()) {
                auto msg_node = _msg_que.front();
                cout << "recv_msg id  is " << msg_node->_recvnode->_msg_id << endl;
                auto call_back_iter = _fun_callbacks.find(msg_node->_recvnode->_msg_id);
                if (call_back_iter == _fun_callbacks.end()) {
                    _msg_que.pop();
                    continue;
                }
                call_back_iter->second(msg_node->_session, msg_node->_recvnode->_msg_id,
                    std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_cur_len));
                _msg_que.pop();
            }
            break;
        }

        //ûͣ˵
        auto msg_node = _msg_que.front();
        cout << "recv_msg id  is " << msg_node->_recvnode->_msg_id << endl;
        auto call_back_iter = _fun_callbacks.find(msg_node->_recvnode->_msg_id);
        if (call_back_iter == _fun_callbacks.end()) {
            _msg_que.pop();
            std::cout << "msg id [" << msg_node->_recvnode->_msg_id << "] handler not found" << std::endl;
            continue;
        }
        // 根据消息 id 执行对应回调
        call_back_iter->second(msg_node->_session, msg_node->_recvnode->_msg_id, 
            std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_cur_len));
        _msg_que.pop();
    }
}

void LogicSystem::RegisterCallBacks() {
    _fun_callbacks[MSG_CHAT_LOGIN] = std::bind(&LogicSystem::LoginHandler, this,
        placeholders::_1, placeholders::_2, placeholders::_3);
    // 文件上传
    _fun_callbacks[MSG_FILE_INIT] = std::bind(&LogicSystem::FileInitHandler, this,
        placeholders::_1, placeholders::_2, placeholders::_3);
    _fun_callbacks[MSG_FILE_CHUNK] = std::bind(&LogicSystem::FileChunkHandler, this,
        placeholders::_1, placeholders::_2, placeholders::_3);
    _fun_callbacks[MSG_FILE_COMPLETE] = std::bind(&LogicSystem::FileCompleteHandler, this,
        placeholders::_1, placeholders::_2, placeholders::_3);
    // WebRTC 信令
    _fun_callbacks[MSG_RTC_CALL] = std::bind(&LogicSystem::RtcCallHandler, this,
        placeholders::_1, placeholders::_2, placeholders::_3);
    _fun_callbacks[MSG_RTC_OFFER] = std::bind(&LogicSystem::RtcOfferHandler, this,
        placeholders::_1, placeholders::_2, placeholders::_3);
    _fun_callbacks[MSG_RTC_ANSWER] = std::bind(&LogicSystem::RtcAnswerHandler, this,
        placeholders::_1, placeholders::_2, placeholders::_3);
    _fun_callbacks[MSG_RTC_ICE] = std::bind(&LogicSystem::RtcIceHandler, this,
        placeholders::_1, placeholders::_2, placeholders::_3);
    _fun_callbacks[MSG_RTC_HANGUP] = std::bind(&LogicSystem::RtcHangupHandler, this,
        placeholders::_1, placeholders::_2, placeholders::_3);
    // 单聊与群聊
    _fun_callbacks[MSG_TEXT_CHAT_MSG_REQ] = std::bind(&LogicSystem::TextChatHandler, this,
        placeholders::_1, placeholders::_2, placeholders::_3);
    _fun_callbacks[MSG_GROUP_TEXT_MSG_REQ] = std::bind(&LogicSystem::GroupTextChatHandler, this,
        placeholders::_1, placeholders::_2, placeholders::_3);
}

// 用户登录回调
void LogicSystem::LoginHandler(shared_ptr<CSession> session, const short &msg_id, const string &msg_data) {
    Json::Reader reader;
    Json::Value root;
    reader.parse(msg_data, root);
    auto uid = root["uid"].asInt();
    std::cout << "user login uid is  " << uid << " user token  is "
        << root["token"].asString() << endl;

    //状态服务校验token是否准确
    auto rsp = StatusGrpcClient::GetInstance()->Login(uid, root["token"].asString());
    Json::Value  rtvalue;
    Defer defer([this, &rtvalue, session]() {
        std::string return_str = rtvalue.toStyledString();
        session->Send(return_str, MSG_CHAT_LOGIN_RSP);
    });

    rtvalue["error"] = rsp.error();
    if (rsp.error() != ErrorCodes::Success) {
        return;
    }

    //内存查询用户信息
    auto find_iter = _users.find(uid);
    std::shared_ptr<UserInfo> user_info = nullptr;
    if (find_iter == _users.end()) {
        //查询数据库
        user_info = MysqlMgr::GetInstance()->GetUser(uid);
        if (user_info == nullptr) {
            rtvalue["error"] = ErrorCodes::UidInvalid;
            return;
        }

        _users[uid] = user_info;
    }
    else {
        user_info = find_iter->second;
    }

    rtvalue["uid"] = uid;
    rtvalue["token"] = rsp.token();
    rtvalue["name"] = user_info->name;
    // 绑定 uid -> session
    bindUidSession(uid, session);
}

// 简易base64解码（仅用于分片数据示例）
static inline unsigned char b64_index(char c){
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return 255;
}
static std::string base64_decode(const std::string &in){
    std::string out;
    size_t len = in.size();
    int val = 0, valb = -8;
    for (size_t i=0;i<len;i++){
        unsigned char c = b64_index(in[i]);
        if (in[i] == '=') break;
        if (c == 255) continue;
        val = (val<<6) + c;
        valb += 6;
        if (valb >= 0){
            out.push_back(char((val>>valb)&0xFF));
            valb -= 8;
        }
    }
    return out;
}

void LogicSystem::FileInitHandler(shared_ptr<CSession> session, const short &msg_id, const string &msg_data){
    Json::Reader reader; Json::Value req; reader.parse(msg_data, req);
    Json::Value rsp;
    std::string err;
    bool ok = FileUploadManager::Inst().initTask(
        req["fileId"].asString(),
        req["fileName"].asString(),
        req.get("dir", "uploads").asString(),
        req.get("totalSize", 0).asUInt64(),
        req.get("totalChunks", 0).asUInt(),
        err
    );
    rsp["error"] = ok ? 0 : 1;
    if (!ok) rsp["msg"] = err;
    std::string body = rsp.toStyledString();
    session->Send(body, MSG_FILE_INIT_RSP);
}

void LogicSystem::FileChunkHandler(shared_ptr<CSession> session, const short &msg_id, const string &msg_data){
    Json::Reader reader; Json::Value req; reader.parse(msg_data, req);
    FileChunkMeta meta;
    meta.fileId = req["fileId"].asString();
    meta.fileName = req["fileName"].asString();
    meta.targetDir = req.get("dir", "uploads").asString();
    meta.totalSize = req.get("totalSize", 0).asUInt64();
    meta.totalChunks = req.get("totalChunks", 0).asUInt();
    meta.index = req.get("index", 0).asUInt();
    if (req.isMember("data_b64")) {
        meta.data = base64_decode(req["data_b64"].asString());
    } else if (req.isMember("data")) {
        meta.data = req["data"].asString();
    }
    FileUploadManager::Inst().submitChunk(meta);
    Json::Value rsp; rsp["error"] = 0; rsp["fileId"] = meta.fileId; rsp["index"] = meta.index;
    session->Send(rsp.toStyledString(), MSG_FILE_CHUNK_RSP);
}

void LogicSystem::FileCompleteHandler(shared_ptr<CSession> session, const short &msg_id, const string &msg_data){
    Json::Reader reader; Json::Value req; reader.parse(msg_data, req);
    Json::Value rsp; rsp["error"] = 0; rsp["fileId"] = req["fileId"].asString();
    session->Send(rsp.toStyledString(), MSG_FILE_COMPLETE_RSP);
}

// 单聊文本
void LogicSystem::TextChatHandler(shared_ptr<CSession> session, const short &msg_id, const string &msg_data){
    Json::Reader reader; Json::Value req; if (!reader.parse(msg_data, req)) return;
    int fromuid = req.get("fromuid", 0).asInt();
    int touid = req.get("touid", 0).asInt();
    // 回复发送端受理
    {
        Json::Value rsp; rsp["error"] = 0; rsp["touid"] = touid;
        session->Send(rsp.toStyledString(), MSG_TEXT_CHAT_MSG_RSP);
    }
    // 通知接收端
    Json::Value notify = req; notify["error"] = 0;
    forwardToUid(touid, MSG_NOTIFY_TEXT_CHAT_MSG_REQ, notify);
}

// 群聊文本（最小实现：请求内带 members 列表）
void LogicSystem::GroupTextChatHandler(shared_ptr<CSession> session, const short &msg_id, const string &msg_data){
    Json::Reader reader; Json::Value req; if (!reader.parse(msg_data, req)) return;
    int groupid = req.get("groupid", 0).asInt();
    // 回复发送端受理
    {
        Json::Value rsp; rsp["error"] = 0; rsp["groupid"] = groupid;
        session->Send(rsp.toStyledString(), MSG_GROUP_TEXT_MSG_RSP);
    }
    // 广播到成员（请求需包含 members: [uid,...]），后续可用DB/Redis查询群成员
    if (req.isMember("members") && req["members"].isArray()) {
        for (auto &mem : req["members"]) {
            int uid = mem.asInt();
            Json::Value notify = req; notify["error"] = 0;
            forwardToUid(uid, MSG_NOTIFY_GROUP_TEXT_MSG_REQ, notify);
        }
    }
}

void LogicSystem::bindUidSession(int uid, std::shared_ptr<CSession> session){
    std::lock_guard<std::mutex> g(_mutex);
    _uid_sessions[uid] = session;
}

std::shared_ptr<CSession> LogicSystem::findSessionByUid(int uid){
    std::lock_guard<std::mutex> g(_mutex);
    auto it = _uid_sessions.find(uid);
    if (it == _uid_sessions.end()) return nullptr;
    return it->second.lock();
}

bool LogicSystem::forwardToUid(int toUid, short msgId, const Json::Value &payload){
    auto target = findSessionByUid(toUid);
    if (!target) return false;
    target->Send(payload.toStyledString(), msgId);
    return true;
}

void LogicSystem::RtcCallHandler(shared_ptr<CSession> session, const short &msg_id, const string &msg_data){
    Json::Reader reader; Json::Value req; reader.parse(msg_data, req);
    int from = req.get("from", 0).asInt();
    int to = req["to"].asInt();
    const char* reason = nullptr;
    Json::Value rsp;
    if (!CallManager::Inst().canCall(from, to, &reason)) {
        rsp["error"] = 1; rsp["msg"] = reason ? reason : "busy"; rsp["to"] = to;
        session->Send(rsp.toStyledString(), MSG_RTC_CALL_RSP);
        return;
    }
    CallManager::Inst().startRinging(from, to);
    if (!forwardToUid(to, MSG_RTC_CALL, req)) {
        // 对方不在线，回滚状态
        CallManager::Inst().hangup(from);
        rsp["error"] = 2; rsp["msg"] = "peer offline"; rsp["to"] = to;
        session->Send(rsp.toStyledString(), MSG_RTC_CALL_RSP);
        return;
    }
    rsp["error"] = 0; rsp["to"] = to;
    session->Send(rsp.toStyledString(), MSG_RTC_CALL_RSP);
}

void LogicSystem::RtcOfferHandler(shared_ptr<CSession> session, const short &msg_id, const string &msg_data){
    Json::Reader reader; Json::Value req; reader.parse(msg_data, req);
    int from = req.get("from", 0).asInt();
    int to = req["to"].asInt();
    // 仅转发，状态已在 CallManager 管控
    forwardToUid(to, MSG_RTC_OFFER, req);
}

void LogicSystem::RtcAnswerHandler(shared_ptr<CSession> session, const short &msg_id, const string &msg_data){
    Json::Reader reader; Json::Value req; reader.parse(msg_data, req);
    int from = req.get("from", 0).asInt();
    int to = req["to"].asInt();
    const char* reason = nullptr;
    if (!CallManager::Inst().accept(from, to, &reason)) {
        Json::Value rsp; rsp["error"] = 1; rsp["msg"] = reason ? reason : "accept failed"; rsp["to"] = to;
        session->Send(rsp.toStyledString(), MSG_RTC_CALL_RSP);
        return;
    }
    forwardToUid(to, MSG_RTC_ANSWER, req);
}

void LogicSystem::RtcIceHandler(shared_ptr<CSession> session, const short &msg_id, const string &msg_data){
    Json::Reader reader; Json::Value req; reader.parse(msg_data, req);
    int to = req["to"].asInt();
    forwardToUid(to, MSG_RTC_ICE, req);
}

void LogicSystem::RtcHangupHandler(shared_ptr<CSession> session, const short &msg_id, const string &msg_data){
    Json::Reader reader; Json::Value req; reader.parse(msg_data, req);
    int from = req.get("from", 0).asInt();
    const char* reason = nullptr;
    int peer = CallManager::Inst().hangup(from, &reason);
    if (peer != -1) {
        forwardToUid(peer, MSG_RTC_HANGUP, req);
    }
}
