/****************************************************************************
** Meta object code from reading C++ file 'wifi.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.10)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../t507/wifi.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wifi.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.10. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_wifi_t {
    QByteArrayData data[9];
    char stringdata0[135];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_wifi_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_wifi_t qt_meta_stringdata_wifi = {
    {
QT_MOC_LITERAL(0, 0, 4), // "wifi"
QT_MOC_LITERAL(1, 5, 8), // "Mysignal"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 13), // "retBt_clicked"
QT_MOC_LITERAL(4, 29, 21), // "WifiConnectBt_clicked"
QT_MOC_LITERAL(5, 51, 24), // "WifiDisconnectBt_clicked"
QT_MOC_LITERAL(6, 76, 17), // "HotSpotBt_clicked"
QT_MOC_LITERAL(7, 94, 23), // "SignalQualityBt_clicked"
QT_MOC_LITERAL(8, 118, 16) // "StatusBt_clicked"

    },
    "wifi\0Mysignal\0\0retBt_clicked\0"
    "WifiConnectBt_clicked\0WifiDisconnectBt_clicked\0"
    "HotSpotBt_clicked\0SignalQualityBt_clicked\0"
    "StatusBt_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_wifi[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    0,   55,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void wifi::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<wifi *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Mysignal(); break;
        case 1: _t->retBt_clicked(); break;
        case 2: _t->WifiConnectBt_clicked(); break;
        case 3: _t->WifiDisconnectBt_clicked(); break;
        case 4: _t->HotSpotBt_clicked(); break;
        case 5: _t->SignalQualityBt_clicked(); break;
        case 6: _t->StatusBt_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (wifi::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&wifi::Mysignal)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject wifi::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_wifi.data,
    qt_meta_data_wifi,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *wifi::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *wifi::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_wifi.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int wifi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void wifi::Mysignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
