/****************************************************************************
** Meta object code from reading C++ file 'eth0_popup.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.10)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../t507/eth0_popup.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'eth0_popup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.10. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_eth0_popup_t {
    QByteArrayData data[12];
    char stringdata0[118];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_eth0_popup_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_eth0_popup_t qt_meta_stringdata_eth0_popup = {
    {
QT_MOC_LITERAL(0, 0, 10), // "eth0_popup"
QT_MOC_LITERAL(1, 11, 5), // "addip"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 10), // "addipinfor"
QT_MOC_LITERAL(4, 29, 5), // "modip"
QT_MOC_LITERAL(5, 35, 10), // "modipinfor"
QT_MOC_LITERAL(6, 46, 5), // "delip"
QT_MOC_LITERAL(7, 52, 10), // "delipinfor"
QT_MOC_LITERAL(8, 63, 6), // "remain"
QT_MOC_LITERAL(9, 70, 15), // "getaddipinforma"
QT_MOC_LITERAL(10, 86, 15), // "getmodipinforma"
QT_MOC_LITERAL(11, 102, 15) // "getdelipinforma"

    },
    "eth0_popup\0addip\0\0addipinfor\0modip\0"
    "modipinfor\0delip\0delipinfor\0remain\0"
    "getaddipinforma\0getmodipinforma\0"
    "getdelipinforma"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_eth0_popup[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    3,   65,    2, 0x06 /* Public */,
       4,    0,   72,    2, 0x06 /* Public */,
       5,    3,   73,    2, 0x06 /* Public */,
       6,    0,   80,    2, 0x06 /* Public */,
       7,    1,   81,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   84,    2, 0x08 /* Private */,
       9,    0,   85,    2, 0x08 /* Private */,
      10,    0,   86,    2, 0x08 /* Private */,
      11,    0,   87,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void eth0_popup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<eth0_popup *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addip(); break;
        case 1: _t->addipinfor((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 2: _t->modip(); break;
        case 3: _t->modipinfor((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 4: _t->delip(); break;
        case 5: _t->delipinfor((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->remain(); break;
        case 7: _t->getaddipinforma(); break;
        case 8: _t->getmodipinforma(); break;
        case 9: _t->getdelipinforma(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (eth0_popup::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eth0_popup::addip)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (eth0_popup::*)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eth0_popup::addipinfor)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (eth0_popup::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eth0_popup::modip)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (eth0_popup::*)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eth0_popup::modipinfor)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (eth0_popup::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eth0_popup::delip)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (eth0_popup::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&eth0_popup::delipinfor)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject eth0_popup::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_eth0_popup.data,
    qt_meta_data_eth0_popup,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *eth0_popup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *eth0_popup::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_eth0_popup.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int eth0_popup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void eth0_popup::addip()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void eth0_popup::addipinfor(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void eth0_popup::modip()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void eth0_popup::modipinfor(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void eth0_popup::delip()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void eth0_popup::delipinfor(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
