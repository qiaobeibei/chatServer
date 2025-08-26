#include "rtcbridge.h"

RtcBridge::RtcBridge(QObject *parent)
    : QObject(parent)
{
    _view = new QWebEngineView();
    _channel = new QWebChannel(_view);
    _view->page()->setWebChannel(_channel);
    _channel->registerObject(QStringLiteral("rtcBridge"), this);
}

void RtcBridge::loadPage(const QUrl &url)
{
    _view->load(url);
}

void RtcBridge::sendOffer(const QJsonObject &obj)
{
    QJsonDocument doc(obj);
    QString json = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    _view->page()->runJavaScript(QStringLiteral("onRemoteOffer(%1)").arg(json));
}

void RtcBridge::sendAnswer(const QJsonObject &obj)
{
    QJsonDocument doc(obj);
    QString json = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    _view->page()->runJavaScript(QStringLiteral("onRemoteAnswer(%1)").arg(json));
}

void RtcBridge::sendIce(const QJsonObject &obj)
{
    QJsonDocument doc(obj);
    QString json = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    _view->page()->runJavaScript(QStringLiteral("onRemoteIce(%1)").arg(json));
} 