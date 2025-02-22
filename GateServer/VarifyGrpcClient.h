#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"
#include "Singleton.h"
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

class RPCconPool {
public:
	RPCconPool(std::size_t poolsize, std::string host, std::string port);
	~RPCconPool();
	void Close();
	std::unique_ptr<VarifyService::Stub> getConnection();
	void returnConnection(std::unique_ptr<VarifyService::Stub> context);
private:
	std::atomic<bool> b_stop_;
	std::size_t poolSize_;
	std::string host;
	std::string port;
	std::queue<std::unique_ptr<VarifyService::Stub>> connections_;
	std::mutex mutex_;
	std::condition_variable cond_;
};

class VarifyGrpcClient : public Singleton<VarifyGrpcClient>
{
	friend class Singleton<VarifyGrpcClient>;
public:
	GetVarifyRsp GetVarifyCode(std::string email);
private:
	VarifyGrpcClient();
	std::unique_ptr<RPCconPool> pool_;
};

