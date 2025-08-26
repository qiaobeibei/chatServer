#ifndef HTTPMGR_H
#define HTTPMGR_H

#include <QString>
#include <QUrl>
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include "singleton.h"
#include "global.h"

class HttpMgr :  public QObject, public Singleton<HttpMgr>, public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT
private:
    HttpMgr();
private:
    friend class Singleton<HttpMgr>;
    QNetworkAccessManager _manger;
private slots:
    void slotHttpFinish( ReqId req_id, QString res, ErrorCodes err, Modules mod);
signals:
    void sigHttpFinish( ReqId req_id, QString res, ErrorCodes err, Modules mod);
    void sigRegModFinish(ReqId id, QString res, ErrorCodes err);
    void sig_reset_mod_finish(ReqId id, QString res, ErrorCodes err);
    void sig_login_mod_finish(ReqId id, QString res, ErrorCodes err);
public:
    ~HttpMgr();
    void PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod);
};

#endif // HTTPMGR_H
