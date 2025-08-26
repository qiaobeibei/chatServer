#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _login = new Login(this);
    _login->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_login);
    // _login->show();

    // 创建和注册消息链接
    connect(_login, &Login::switchRegister, this, &MainWindow::SlotSwitchReg);
    connect(_login, &Login::switchReset, this, &MainWindow::SlotSwitchReset);
}

MainWindow::~MainWindow()
{
    delete ui;
    // if(_login){
    //     delete _login;
    // }
    // if(_register){
    //     delete _register;
    // }
}

void MainWindow::SlotSwitchReg()
{
    _register = new Register(this);
    _register->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    // 连接注册界面返回登录信号
    connect(_register,&Register::sigSwitchLogin, this, &MainWindow::SlotSwitchLogin);

    setCentralWidget(_register);
    _login->hide();
    _register->show();
}

void MainWindow::SlotSwitchLogin()
{
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _login = new Login(this);
    _login->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login);
    _register->hide();
    _login->show();
    //连接登录界面注册信号
    connect(_login, &Login::switchRegister, this, &MainWindow::SlotSwitchReg);
    //连接登录界面忘记密码信号
    connect(_login, &Login::switchReset, this, &MainWindow::SlotSwitchReset);
}

void MainWindow::SlotSwitchReset()
{
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _reset_dlg = new ResetDialog(this);
    _reset_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_reset_dlg);
    _login->hide();
    _reset_dlg->show();
    //注册返回登录信号和槽函数
    connect(_reset_dlg, &ResetDialog::switchLogin, this, &MainWindow::SlotSwitchLogin2);
}

// 从重置界面返回登录界面
void MainWindow::SlotSwitchLogin2()
{
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _login = new Login(this);
    _login->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login);
    _login->hide();
    _login->show();
    //连接登录界面忘记密码信号
    connect(_login, &Login::switchReset, this, &MainWindow::SlotSwitchReset);
    //连接登录界面注册信号
    connect(_login, &Login::switchRegister, this, &MainWindow::SlotSwitchReg);
}
