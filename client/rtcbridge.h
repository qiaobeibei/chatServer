#pragma once
#include <QObject>
#include <QWebEngineView>
#include <QWebChannel>
#include <QJsonObject>

class RtcBridge : public QObject
{
    Q_OBJECT
public:
    explicit RtcBridge(QObject *parent = nullptr);
    QWebEngineView* view() { return _view; }
    void loadPage(const QUrl &url);

    // C++ -> JS
    Q_INVOKABLE void sendOffer(const QJsonObject &obj);
    Q_INVOKABLE void sendAnswer(const QJsonObject &obj);
    Q_INVOKABLE void sendIce(const QJsonObject &obj);

signals:
    // JS -> C++
    void sig_local_offer(QJsonObject obj);
    void sig_local_answer(QJsonObject obj);
    void sig_local_ice(QJsonObject obj);

private:
    QWebEngineView *_view{nullptr};
    QWebChannel *_channel{nullptr};
}; 