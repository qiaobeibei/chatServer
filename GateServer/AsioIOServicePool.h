#pragma once
#include "Singleton.h"
#include <boost/asio.hpp>
#include <vector>

class AsioIOServicePool : public Singleton<AsioIOServicePool>
{
	friend Singleton<AsioIOServicePool>;
public:
	using IOService = boost::asio::io_context;
	using Work = boost::asio::io_context::work; // work���ڰ�ioc��ʹioc���������˳�
	using WorkPtr = std::unique_ptr<Work>; // ϣ����work���ᱻ������ֻ���ƶ����ߴ�ͷ�õ�β�����ı�
	~AsioIOServicePool();
	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator = (const AsioIOServicePool&) = delete;
	// ʹ��round-robin �ķ�ʽ����һ��io_context
	boost::asio::io_context& GetIOService();
	void Stop();
private:
	AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency()); // hardware_concurrency��ȡCPu����
	std::vector<IOService> _ioServices;
	std::vector<WorkPtr> _works;
	std::vector<std::thread> _threads;
	// ͨ����ѯ����iocʱ����Ҫ��¼��ǰioc���±꣬�ۼӣ�������vector��sizeʱ�͹��㣬Ȼ���������ѯ�ķ�ʽ����
	// ��¼ioc��vector���±�
	std::size_t _nextIOService;
};
