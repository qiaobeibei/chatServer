#pragma once
#include "Singleton.h"
#include <boost/asio.hpp>
#include <vector>

class AsioIOServicePool : public Singleton<AsioIOServicePool>
{
	friend Singleton<AsioIOServicePool>;
public:
	using IOService = boost::asio::io_context;
	using Work = boost::asio::io_context::work; // work用于绑定ioc，使ioc不会主动退出
	using WorkPtr = std::unique_ptr<Work>; // 希望该work不会被拷贝，只能移动或者从头用到尾不被改变
	~AsioIOServicePool();
	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator = (const AsioIOServicePool&) = delete;
	// 使用round-robin 的方式返回一个io_context
	boost::asio::io_context& GetIOService();
	void Stop();
private:
	AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency()); // hardware_concurrency获取CPu核数
	std::vector<IOService> _ioServices;
	std::vector<WorkPtr> _works;
	std::vector<std::thread> _threads;
	// 通过轮询返回ioc时，需要记录当前ioc的下标，累加，当超过vector的size时就归零，然后继续按轮询的方式返回
	// 记录ioc在vector的下标
	std::size_t _nextIOService;
};
