/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "D:/app/QT/file/project-chat/clickedlabel.h"

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *head_label;
    QSpacerItem *horizontalSpacer_6;
    QLabel *err_tip;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *user_edit;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *pass_edit;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *forget_edit;
    ClickedLabel *forget_label;
    QRadioButton *radioButton;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *login_btn;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *reg_btn;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QDialog *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName(QString::fromUtf8("Login"));
        Login->resize(315, 500);
        Login->setMinimumSize(QSize(315, 500));
        Login->setMaximumSize(QSize(315, 500));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Login->setWindowIcon(icon);
        Login->setStyleSheet(QString::fromUtf8("\n"
"\n"
"QLineEdit{\n"
"	border\357\274\232none;\n"
"	border-radius:10px;\n"
"}\n"
"\n"
"QPushButton{\n"
"	background-color: rgb(0, 0, 0, 0);\n"
"}\n"
"\n"
"QPushButton#reg_btn,#login_btn{\n"
"	background-color: rgb(158, 218, 255);\n"
"	color: rgb(255, 255, 255);\n"
"	border\357\274\232none;\n"
"	border-radius:10px;\n"
"}"));
        horizontalLayout = new QHBoxLayout(Login);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(7);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(25, 12, 25, 5);
        widget = new QWidget(Login);
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        head_label = new QLabel(widget);
        head_label->setObjectName(QString::fromUtf8("head_label"));
        head_label->setMinimumSize(QSize(150, 150));
        head_label->setMaximumSize(QSize(145, 150));
        head_label->setStyleSheet(QString::fromUtf8(""));
        head_label->setScaledContents(true);
        head_label->setAlignment(Qt::AlignCenter);
        head_label->setMargin(15);
        head_label->setIndent(-1);

        gridLayout->addWidget(head_label, 1, 1, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_6, 1, 2, 1, 1);

        err_tip = new QLabel(widget);
        err_tip->setObjectName(QString::fromUtf8("err_tip"));
        err_tip->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(err_tip, 2, 1, 1, 1);


        verticalLayout->addWidget(widget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        user_edit = new QLineEdit(Login);
        user_edit->setObjectName(QString::fromUtf8("user_edit"));
        user_edit->setMinimumSize(QSize(0, 40));
        user_edit->setMaximumSize(QSize(16777215, 40));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\271\274\345\234\206"));
        font.setPointSize(12);
        user_edit->setFont(font);
        user_edit->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(user_edit);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pass_edit = new QLineEdit(Login);
        pass_edit->setObjectName(QString::fromUtf8("pass_edit"));
        pass_edit->setMinimumSize(QSize(0, 40));
        pass_edit->setMaximumSize(QSize(16777215, 40));
        pass_edit->setFont(font);
        pass_edit->setStyleSheet(QString::fromUtf8(""));
        pass_edit->setAlignment(Qt::AlignCenter);
        pass_edit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        horizontalLayout_3->addWidget(pass_edit);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        forget_edit = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(forget_edit);

        forget_label = new ClickedLabel(Login);
        forget_label->setObjectName(QString::fromUtf8("forget_label"));
        forget_label->setMinimumSize(QSize(0, 25));
        forget_label->setMaximumSize(QSize(16777215, 25));
        forget_label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(forget_label);


        verticalLayout->addLayout(horizontalLayout_4);

        radioButton = new QRadioButton(Login);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setMinimumSize(QSize(0, 25));
        radioButton->setMaximumSize(QSize(16777215, 25));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Microsoft YaHei"));
        radioButton->setFont(font1);

        verticalLayout->addWidget(radioButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        login_btn = new QPushButton(Login);
        login_btn->setObjectName(QString::fromUtf8("login_btn"));
        login_btn->setEnabled(false);
        login_btn->setMinimumSize(QSize(0, 30));
        login_btn->setMaximumSize(QSize(500, 30));
        login_btn->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_5->addWidget(login_btn);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        reg_btn = new QPushButton(Login);
        reg_btn->setObjectName(QString::fromUtf8("reg_btn"));
        reg_btn->setEnabled(false);
        reg_btn->setMinimumSize(QSize(0, 30));
        reg_btn->setMaximumSize(QSize(500, 30));
        reg_btn->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_9->addWidget(reg_btn);


        verticalLayout->addLayout(horizontalLayout_9);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QDialog *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "Dialog", nullptr));
        head_label->setText(QString());
        err_tip->setText(QString());
        user_edit->setPlaceholderText(QCoreApplication::translate("Login", "\350\276\223\345\205\245\350\264\246\345\217\267", nullptr));
        pass_edit->setPlaceholderText(QCoreApplication::translate("Login", "\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        forget_label->setText(QCoreApplication::translate("Login", "\345\277\230\350\256\260\345\257\206\347\240\201", nullptr));
        radioButton->setText(QCoreApplication::translate("Login", "\345\267\262\351\230\205\350\257\273\345\271\266\345\220\214\346\204\217\346\234\215\345\212\241\345\215\217\350\256\256\345\222\214\350\264\246\345\217\267\351\232\220\347\247\201\344\277\235\346\212\244\346\214\207\345\274\225", nullptr));
        login_btn->setText(QCoreApplication::translate("Login", "\347\231\273\345\275\225", nullptr));
        reg_btn->setText(QCoreApplication::translate("Login", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
