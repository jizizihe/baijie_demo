/****************************************************************************
** Meta object code from reading C++ file 'ipset_popup.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.10)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ipset_popup.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ipset_popup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.10. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ipset_popup_t {
    QByteArrayData data[12];
    char stringdata0[118];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ipset_popup_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ipset_popup_t qt_meta_stringdata_ipset_popup = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ipset_popup"
QT_MOC_LITERAL(1, 12, 5), // "addip"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 10), // "addipinfor"
QT_MOC_LITERAL(4, 30, 5), // "modip"
QT_MOC_LITERAL(5, 36, 10), // "modipinfor"
QT_MOC_LITERAL(6, 47, 7), // "go_back"
QT_MOC_LITERAL(7, 55, 6), // "remain"
QT_MOC_LITERAL(8, 62, 15), // "getaddipinforma"
QT_MOC_LITERAL(9, 78, 15), // "getmodipinforma"
QT_MOC_LITERAL(10, 94, 12), // "screen_clear"
QT_MOC_LITERAL(11, 107, 10) // "returnmain"

    },
    "ipset_popup\0addip\0\0addipinfor\0modip\0"
    "modipinfor\0go_back\0remain\0getaddipinforma\0"
    "getmodipinforma\0screen_clear\0returnmain"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ipset_popup[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    2,   65,    2, 0x06 /* Public */,
       4,    0,   70,    2, 0x06 /* Public */,
       5,    1,   71,    2, 0x06 /* Public */,
       6,    0,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   75,    2, 0x08 /* Private */,
       8,    0,   76,    2, 0x08 /* Private */,
       9,    0,   77,    2, 0x08 /* Private */,
      10,    0,   78,    2, 0x08 /* Private */,
      11,    0,   79,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ipset_popup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ipset_popup *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addip(); break;
        case 1: _t->addipinfor((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->modip(); break;
        case 3: _t->modipinfor((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->go_back(); break;
        case 5: _t->remain(); break;
        case 6: _t->getaddipinforma(); break;
        case 7: _t->getmodipinforma(); break;
        case 8: _t->screen_clear(); break;
        case 9: _t->returnmain(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ipset_popup::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ipset_popup::addip)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ipset_popup::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ipset_popup::addipinfor)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ipset_popup::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ipset_popup::modip)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ipset_popup::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ipset_popup::modipinfor)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ipset_popup::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ipset_popup::go_back)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ipset_popup::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_ipset_popup.data,
    qt_meta_data_ipset_popup,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ipset_popup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ipset_popup::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ipset_popup.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ipset_popup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void ipset_popup::addip()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ipset_popup::addipinfor(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ipset_popup::modip()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ipset_popup::modipinfor(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ipset_popup::go_back()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
