#pragma once
#include "Singleton.h"
#include <queue>
#include <thread>
#include "CSession.h"
#include <queue>
#include <map>
#include <functional>
#include "const.h"
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <unordered_map>
#include "data.h"

typedef  function<void(shared_ptr<CSession>, const short &msg_id, const string &msg_data)> FunCallBack;
class LogicSystem:public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();
	void PostMsgToQue(shared_ptr < LogicNode> msg);
private:
	LogicSystem();
	void DealMsg();
	void RegisterCallBacks();
	void LoginHandler(shared_ptr<CSession>, const short &msg_id, const string &msg_data);
	// 单聊/群聊
	void TextChatHandler(shared_ptr<CSession>, const short &msg_id, const string &msg_data);
	void GroupTextChatHandler(shared_ptr<CSession>, const short &msg_id, const string &msg_data);
	// 文件上传
	void FileInitHandler(shared_ptr<CSession>, const short &msg_id, const string &msg_data);
	void FileChunkHandler(shared_ptr<CSession>, const short &msg_id, const string &msg_data);
	void FileCompleteHandler(shared_ptr<CSession>, const short &msg_id, const string &msg_data);
	// WebRTC 信令
	void RtcCallHandler(shared_ptr<CSession>, const short &msg_id, const string &msg_data);
	void RtcOfferHandler(shared_ptr<CSession>, const short &msg_id, const string &msg_data);
	void RtcAnswerHandler(shared_ptr<CSession>, const short &msg_id, const string &msg_data);
	void RtcIceHandler(shared_ptr<CSession>, const short &msg_id, const string &msg_data);
	void RtcHangupHandler(shared_ptr<CSession>, const short &msg_id, const string &msg_data);
	void bindUidSession(int uid, std::shared_ptr<CSession> session);
	std::shared_ptr<CSession> findSessionByUid(int uid);
	bool forwardToUid(int toUid, short msgId, const Json::Value &payload);
	// 仅入本地队列（Kafka消费回调使用，避免环路）
	void EnqueueLocal(shared_ptr < LogicNode> msg);

	std::thread _worker_thread;
	std::queue<shared_ptr<LogicNode>> _msg_que;
	std::mutex _mutex;
	std::condition_variable _consume;
	bool _b_stop;
	std::map<short, FunCallBack> _fun_callbacks;
	std::unordered_map<int, std::shared_ptr<UserInfo>> _users;
	std::unordered_map<int, std::weak_ptr<CSession>> _uid_sessions;
};

