#include "httpmgr.h"

HttpMgr::HttpMgr()
{
    connect(this, &HttpMgr::sigHttpFinish, this, &HttpMgr::slotHttpFinish);
}

void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
{
    QByteArray data = QJsonDocument(json).toJson(); // 获取发送内容（比如邮箱或注册信息）
    QNetworkRequest request(url); // 请求头/路由
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));
    auto self = shared_from_this();
    QNetworkReply* reply = _manger.post(request, data); // 将发送内容发送至GateServer，即和127.0.0.1:1250建立连接并发送内容

    QObject::connect(reply, &QNetworkReply::finished, [self, reply, req_id, mod](){ //设置信号和槽等待发送完成
        // 处理错误情况
        if(reply->error() != QNetworkReply::NoError){
            qDebug() << reply->errorString();
            // 发送信号通知完成
            emit self->sigHttpFinish(req_id, "", ErrorCodes::ERR_NETWORK, mod);
            return;
        }

        // 无错误
        QString res = reply->readAll();
        // 发送信号 通知完成
        emit self->sigHttpFinish(req_id,res,ErrorCodes::SUCCESS, mod);
        reply->deleteLater();
        return;
    } );
}

void HttpMgr::slotHttpFinish(ReqId id, QString res, ErrorCodes err, Modules mod)
{
    // 注册
    if(mod == Modules::REGISTERMOD){
        // 发送信号通知指定模块 http 的响应结束了
        emit sigRegModFinish(id,res,err);
    }
    // 重置
    if(mod == Modules::RESETMOD){
        // 发送信号通知指定模块 http 的响应结束了
        emit sig_reset_mod_finish(id,res,err);
    }
    // 登录
    if(mod == Modules::LOGINMOD){
        // 发送信号通知指定模块 http 的响应结束了
        emit sig_login_mod_finish(id,res,err);
    }

}

HttpMgr::~HttpMgr()
{

}
