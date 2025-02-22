#pragma once
#include "const.h"
class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
	friend class LogicSystem;
private:
	void CheckDeadline(); // 检测超时
	void WriteResponse(); // 写响应头
	void HandleReq(); // 处理请求头
	void PreParseGetParam();
private:
	tcp::socket _socket;
	beast::flat_buffer _buffer{ 8192 };
	http::request<http::dynamic_body> _request;
	http::response<http::dynamic_body> _response;
	net::steady_timer _deadline{ // 定时器
		_socket.get_executor(), std::chrono::seconds(60)
	};
	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;
public:
	HttpConnection(boost::asio::io_context& ioc);
	void Start();
	tcp::socket& GetSocket(){ return _socket; }
};

