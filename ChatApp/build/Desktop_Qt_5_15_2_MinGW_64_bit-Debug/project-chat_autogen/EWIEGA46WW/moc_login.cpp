/****************************************************************************
** Meta object code from reading C++ file 'login.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../login.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'login.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Login_t {
    QByteArrayData data[18];
    char stringdata0[218];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Login_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Login_t qt_meta_stringdata_Login = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Login"
QT_MOC_LITERAL(1, 6, 14), // "switchRegister"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 11), // "switchReset"
QT_MOC_LITERAL(4, 34, 15), // "sig_connect_tcp"
QT_MOC_LITERAL(5, 50, 10), // "ServerInfo"
QT_MOC_LITERAL(6, 61, 22), // "on_radioButton_clicked"
QT_MOC_LITERAL(7, 84, 20), // "on_login_btn_clicked"
QT_MOC_LITERAL(8, 105, 15), // "slot_forget_pwd"
QT_MOC_LITERAL(9, 121, 21), // "slot_login_mod_finish"
QT_MOC_LITERAL(10, 143, 5), // "ReqId"
QT_MOC_LITERAL(11, 149, 2), // "id"
QT_MOC_LITERAL(12, 152, 3), // "res"
QT_MOC_LITERAL(13, 156, 10), // "ErrorCodes"
QT_MOC_LITERAL(14, 167, 3), // "err"
QT_MOC_LITERAL(15, 171, 19), // "slot_tcp_con_finish"
QT_MOC_LITERAL(16, 191, 8), // "bsuccess"
QT_MOC_LITERAL(17, 200, 17) // "slot_login_failed"

    },
    "Login\0switchRegister\0\0switchReset\0"
    "sig_connect_tcp\0ServerInfo\0"
    "on_radioButton_clicked\0on_login_btn_clicked\0"
    "slot_forget_pwd\0slot_login_mod_finish\0"
    "ReqId\0id\0res\0ErrorCodes\0err\0"
    "slot_tcp_con_finish\0bsuccess\0"
    "slot_login_failed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Login[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    1,   61,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   64,    2, 0x08 /* Private */,
       7,    0,   65,    2, 0x08 /* Private */,
       8,    0,   66,    2, 0x08 /* Private */,
       9,    3,   67,    2, 0x08 /* Private */,
      15,    1,   74,    2, 0x08 /* Private */,
      17,    1,   77,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10, QMetaType::QString, 0x80000000 | 13,   11,   12,   14,
    QMetaType::Void, QMetaType::Bool,   16,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void Login::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Login *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->switchRegister(); break;
        case 1: _t->switchReset(); break;
        case 2: _t->sig_connect_tcp((*reinterpret_cast< ServerInfo(*)>(_a[1]))); break;
        case 3: _t->on_radioButton_clicked(); break;
        case 4: _t->on_login_btn_clicked(); break;
        case 5: _t->slot_forget_pwd(); break;
        case 6: _t->slot_login_mod_finish((*reinterpret_cast< ReqId(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< ErrorCodes(*)>(_a[3]))); break;
        case 7: _t->slot_tcp_con_finish((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->slot_login_failed((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Login::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Login::switchRegister)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Login::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Login::switchReset)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Login::*)(ServerInfo );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Login::sig_connect_tcp)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Login::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_Login.data,
    qt_meta_data_Login,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Login::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Login::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Login.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Login::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Login::switchRegister()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Login::switchReset()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Login::sig_connect_tcp(ServerInfo _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
