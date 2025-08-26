/******************************************************************************
 *
 * @file       mainwindow.h
 * @brief      主窗口
 *
 * @author     爱吃土豆
 * @date       2025/01/10
 * @history
 *****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "login.h"
#include "register.h"
#include "resetdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void SlotSwitchReg();
    void SlotSwitchLogin();
    void SlotSwitchReset();
    void SlotSwitchLogin2();
private:
    Ui::MainWindow *ui;
    Login* _login;
    Register* _register;
    ResetDialog* _reset_dlg;
};
#endif // MAINWINDOW_H
