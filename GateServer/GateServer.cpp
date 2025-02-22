#include <iostream>
#include "CServer.h"
#include <cassert>
#include "ConfigMgr.h"

int main()
{
    //TestRedisMgr();
    auto& gCfgMgr = ConfigMgr::GetInstance();
    std::string gate_port_str = gCfgMgr["GateServer"]["Port"];
    unsigned short gate_port = atoi(gate_port_str.c_str());
    try {
        unsigned short port = static_cast<unsigned short>(1250);
        net::io_context ioc{ 1 }; // 一个线程运行
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {
            if (error) {
                return;
            }
            ioc.stop();
            });

        std::make_shared<CServer>(ioc, port)->Start();
        std::cout << "Gate Server listen on port: " << port << std::endl;
        ioc.run();
    }
    catch (std::exception const& er) {
        std::cerr << "Error: " << er.what() << std::endl;
        return EXIT_FAILURE;
    }
}