#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include "global.h"
namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private slots:
    void on_get_code_clicked();
    void slotRegModFinish(ReqId id, QString res, ErrorCodes err);
    void on_sure_btn_clicked();
    void on_pushButton_clicked();

    void on_cancel_btn_clicked();

private:
    void initHttpHandlers();
    void showTip(QString str, bool b_ok);
    bool checkUserValid();
    bool checkEmailValid();
    bool checkPassValid();
    bool checkConfirmValid();
    bool checkVerifyValid();
    void AddTipErr(TipErr te, QString tips);
    void DelTipErr(TipErr te);
    void ChangeTipPage();

    Ui::Register *ui;
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
    QMap<TipErr, QString> _tip_errs;

    QTimer* _countdown_timer;
    int _countdown;
signals:
    void sigSwitchLogin();
};

#endif // REGISTER_H
