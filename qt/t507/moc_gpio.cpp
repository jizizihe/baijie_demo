/****************************************************************************
** Meta object code from reading C++ file 'gpio.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.10)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gpio.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gpio.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.10. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_gpio_t {
    QByteArrayData data[16];
    char stringdata0[160];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_gpio_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_gpio_t qt_meta_stringdata_gpio = {
    {
QT_MOC_LITERAL(0, 0, 4), // "gpio"
QT_MOC_LITERAL(1, 5, 8), // "Mysignal"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 15), // "rBtnout_clicked"
QT_MOC_LITERAL(4, 31, 14), // "rBtnin_clicked"
QT_MOC_LITERAL(5, 46, 11), // "ret_clicked"
QT_MOC_LITERAL(6, 58, 11), // "srceenclear"
QT_MOC_LITERAL(7, 70, 9), // "isEnglish"
QT_MOC_LITERAL(8, 80, 8), // "QString&"
QT_MOC_LITERAL(9, 89, 7), // "qstrSrc"
QT_MOC_LITERAL(10, 97, 8), // "isNumber"
QT_MOC_LITERAL(11, 106, 10), // "istrueport"
QT_MOC_LITERAL(12, 117, 1), // "i"
QT_MOC_LITERAL(13, 119, 16), // "rBtnhigh_clicked"
QT_MOC_LITERAL(14, 136, 15), // "rBtnlow_clicked"
QT_MOC_LITERAL(15, 152, 7) // "warning"

    },
    "gpio\0Mysignal\0\0rBtnout_clicked\0"
    "rBtnin_clicked\0ret_clicked\0srceenclear\0"
    "isEnglish\0QString&\0qstrSrc\0isNumber\0"
    "istrueport\0i\0rBtnhigh_clicked\0"
    "rBtnlow_clicked\0warning"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_gpio[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   70,    2, 0x08 /* Private */,
       4,    0,   71,    2, 0x08 /* Private */,
       5,    0,   72,    2, 0x08 /* Private */,
       6,    0,   73,    2, 0x08 /* Private */,
       7,    1,   74,    2, 0x08 /* Private */,
      10,    1,   77,    2, 0x08 /* Private */,
      11,    2,   80,    2, 0x08 /* Private */,
      13,    0,   85,    2, 0x08 /* Private */,
      14,    0,   86,    2, 0x08 /* Private */,
      15,    0,   87,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, 0x80000000 | 8,    9,
    QMetaType::Bool, 0x80000000 | 8,    9,
    QMetaType::Bool, QMetaType::QString, QMetaType::Int,    2,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,

       0        // eod
};

void gpio::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<gpio *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Mysignal(); break;
        case 1: _t->rBtnout_clicked(); break;
        case 2: _t->rBtnin_clicked(); break;
        case 3: _t->ret_clicked(); break;
        case 4: _t->srceenclear(); break;
        case 5: { bool _r = _t->isEnglish((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->isNumber((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->istrueport((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->rBtnhigh_clicked(); break;
        case 9: _t->rBtnlow_clicked(); break;
        case 10: { bool _r = _t->warning();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (gpio::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&gpio::Mysignal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject gpio::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_gpio.data,
    qt_meta_data_gpio,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *gpio::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *gpio::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_gpio.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int gpio::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void gpio::Mysignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
