#pragma once
#include <functional>


enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,  //Json��������
	RPCFailed = 1002,  //RPC�������
	VarifyExpired = 1003, //��֤�����
	VarifyCodeErr = 1004, //��֤�����
	UserExist = 1005,       //�û��Ѿ�����
	PasswdErr = 1006,    //�������
	EmailNotMatch = 1007,  //���䲻ƥ��
	PasswdUpFailed = 1008,  //��������ʧ��
	PasswdInvalid = 1009,   //�������ʧ��
	TokenInvalid = 1010,   //TokenʧЧ
	UidInvalid = 1011,  //uid��Ч
};


// Defer��
class Defer {
public:
	// ����һ��lambda����ʽ���ߺ���ָ��
	Defer(std::function<void()> func) : func_(func) {}

	// ����������ִ�д���ĺ���
	~Defer() {
		func_();
	}

private:
	std::function<void()> func_;
};

#define MAX_LENGTH  1024*2
//ͷ���ܳ���
#define HEAD_TOTAL_LEN 4
//ͷ��id����
#define HEAD_ID_LEN 2
//ͷ�����ݳ���
#define HEAD_DATA_LEN 2
#define MAX_RECVQUE  10000
#define MAX_SENDQUE 1000


enum MSG_IDS {
	MSG_CHAT_LOGIN = 1005, //�û���½
	MSG_CHAT_LOGIN_RSP = 1006, //�û���½�ذ�
	// 系统通知：踢下线（与客户端1021对齐）
	MSG_NOTIFY_OFFLINE = 1021,
	// 单聊文本
	MSG_TEXT_CHAT_MSG_REQ = 1017,
	MSG_TEXT_CHAT_MSG_RSP = 1018,
	MSG_NOTIFY_TEXT_CHAT_MSG_REQ = 1019,
	// 群聊文本（预留）
	MSG_GROUP_TEXT_MSG_REQ = 1117,
	MSG_GROUP_TEXT_MSG_RSP = 1118,
	MSG_NOTIFY_GROUP_TEXT_MSG_REQ = 1119,
	// 文件分片上传
	MSG_FILE_INIT = 2001,
	MSG_FILE_INIT_RSP = 2002,
	MSG_FILE_CHUNK = 2003,
	MSG_FILE_CHUNK_RSP = 2004,
	MSG_FILE_COMPLETE = 2005,
	MSG_FILE_COMPLETE_RSP = 2006,
	// WebRTC 信令控制
	MSG_RTC_CALL = 3001,
	MSG_RTC_CALL_RSP = 3002,
	MSG_RTC_OFFER = 3003,
	MSG_RTC_ANSWER = 3004,
	MSG_RTC_ICE = 3005,
	MSG_RTC_HANGUP = 3006,
};


