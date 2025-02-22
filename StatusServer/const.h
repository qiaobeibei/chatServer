#pragma once
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>  
#include <memory>
#include <iostream>
#include "Singleton.h"
#include <map>
#include <unordered_map>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <hiredis.h>
#include "ConfigMgr.h"
#include "RedisMgr.h"
#include <cassert>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

enum ErrorCodes {
	Success = 0,
	Error_Json = 1001, // JSON ����ʧ��
	RPCFailed = 1002, // RPC �������
	VarifyExpired = 1003, // ��֤�����
	VarifyCodeErr = 1004, // ��֤�����
	UserExist = 1005, // �û��Ѵ���
	PasswdErr = 1006, // �������
	EmailNotMatch = 1007, // ���䲻ƥ��
	PasswdUpFailed = 1008, // ��������ʧ��
	PasswdInvalid = 1009, // �����ʽ��Ч
	TokenInvalid = 1010,   //TokenʧЧ
	UidInvalid = 1011,  //uid��Ч
};

// Defer�࣬���� RAII
class Defer {
public:
	// ����һ��lambda���ʽ���ߺ���ָ��
	Defer(std::function<void()> func) : func_(func) {}
	// ����������ִ�д���ĺ���
	~Defer() {
		func_();
	}

private:
	std::function<void()> func_;
};

#define USERIPPREFIX  "uip_"
#define USERTOKENPREFIX  "utoken_"
#define IPCOUNTPREFIX  "ipcount_"
#define USER_BASE_INFO "ubaseinfo_"
#define LOGIN_COUNT  "logincount"