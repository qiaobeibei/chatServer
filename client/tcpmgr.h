#ifndef TCPMGR_H
#define TCPMGR_H
#include <QTcpSocket>
#include "singleton.h"
#include "global.h"
#include <functional>
#include <QObject>
#include "userdata.h"
#include <QJsonArray>

class TcpMgr:public QObject, public Singleton<TcpMgr>,
        public std::enable_shared_from_this<TcpMgr>
{
    Q_OBJECT
public:
   ~ TcpMgr();
    void CloseConnection();
    // 发送：文件分片 & RTC 信令
    void sendFileInit(const QString &fileId, const QString &fileName, const QString &dir, quint64 totalSize, quint32 totalChunks);
    void sendFileChunk(const QString &fileId, const QString &fileName, const QString &dir, quint64 totalSize, quint32 totalChunks, quint32 index, const QByteArray &rawData);
    void sendFileComplete(const QString &fileId);

    void sendRtcCall(int fromUid, int toUid, const QString &mediaType);
    void sendRtcOffer(int fromUid, int toUid, const QString &sdp);
    void sendRtcAnswer(int fromUid, int toUid, const QString &sdp);
    void sendRtcIce(int fromUid, int toUid, const QJsonObject &candidate);
    void sendRtcHangup(int fromUid, int toUid);
private:
    friend class Singleton<TcpMgr>;
    TcpMgr();
    void initHandlers();
    void handleMsg(ReqId id, int len, QByteArray data);
    QTcpSocket _socket;
    QString _host;
    uint16_t _port;
    QByteArray _buffer;
    bool _b_recv_pending;
    quint16 _message_id;
    quint16 _message_len;
    QMap<ReqId, std::function<void(ReqId id, int len, QByteArray data)>> _handlers;
public slots:
    void slot_tcp_connect(ServerInfo);
    void slot_send_data(ReqId reqId, QByteArray data);
signals:
    void sig_con_success(bool bsuccess);
    void sig_send_data(ReqId reqId, QByteArray data);
    void sig_swich_chatdlg();
    void sig_load_apply_list(QJsonArray json_array);
    void sig_login_failed(int);
    void sig_user_search(std::shared_ptr<SearchInfo>);
    void sig_friend_apply(std::shared_ptr<AddFriendApply>);
    void sig_add_auth_friend(std::shared_ptr<AuthInfo>);
    void sig_auth_rsp(std::shared_ptr<AuthRsp>);
    void sig_text_chat_msg(std::shared_ptr<TextChatMsg> msg);
    void sig_notify_offline();
    void sig_connection_closed();
    // 新增：文件与 RTC 信令
    void sig_file_init_rsp(QJsonObject obj);
    void sig_file_chunk_rsp(QJsonObject obj);
    void sig_file_complete_rsp(QJsonObject obj);
    void sig_rtc_call(QJsonObject obj);
    void sig_rtc_call_rsp(QJsonObject obj);
    void sig_rtc_offer(QJsonObject obj);
    void sig_rtc_answer(QJsonObject obj);
    void sig_rtc_ice(QJsonObject obj);
    void sig_rtc_hangup(QJsonObject obj);
};

#endif // TCPMGR_H
