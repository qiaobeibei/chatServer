#include "VarifyGrpcClient.h"
#include "ConfigMgr.h"

GetVarifyRsp VarifyGrpcClient::GetVarifyCode(std::string email)
{
    ClientContext context;
    GetVarifyRsp reply; // grpc 接口返回的参数
    GetVarifyReq request; // grpc 接口接收的参数
    request.set_email(email);

    auto stub = pool_->getConnection();
    Status status = stub->GetVarifyCode(&context, request, &reply);
    if (status.ok()) {
        pool_->returnConnection(std::move(stub));
        return reply;
    }
    else {
        pool_->returnConnection(std::move(stub));
        reply.set_error(ErrorCodes::RPCFailed);
        return reply;
    }
}
VarifyGrpcClient::VarifyGrpcClient(){
    auto& gCfgMgr = ConfigMgr::GetInstance();
    std::string host = gCfgMgr["VarifyServer"]["Host"];
    std::string port = gCfgMgr["VarifyServer"]["Port"];
    pool_.reset(new RPCconPool(std::thread::hardware_concurrency(), host, port));
}


RPCconPool::RPCconPool(std::size_t poolsize, std::string host, std::string port) :
    poolSize_(poolsize), host(host), port(port), b_stop_(false){
    for (std::size_t i = 0; i < poolSize_; i++) {
        // nodejs 和 GateServer 通过 127.0.0.1:50051 建立联系
        std::shared_ptr<Channel> channel = grpc::CreateChannel(host+":"+port,
            grpc::InsecureChannelCredentials());
        connections_.push(VarifyService::NewStub(channel));
    }
}

RPCconPool::~RPCconPool(){
    std::lock_guard<std::mutex> lock(mutex_);
    Close();
    while (!connections_.empty()) {
        connections_.pop();
    }
}

void RPCconPool::Close(){
    b_stop_ = true;
    cond_.notify_all();
}

std::unique_ptr<VarifyService::Stub> RPCconPool::getConnection(){
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [this]() {
        if (b_stop_) {
            return true;
        }
        return !connections_.empty();
        });

    if (b_stop_) {
        return nullptr;
    }

    auto context = std::move(connections_.front());
    connections_.pop();
    return context;
}

void RPCconPool::returnConnection(std::unique_ptr<VarifyService::Stub> context){
    std::lock_guard<std::mutex> lock(mutex_);
    if (b_stop_) {
        return;
    }
    connections_.push(std::move(context));
    cond_.notify_one();
}
