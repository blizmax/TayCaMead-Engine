/****************************************************************************
** Meta object code from reading C++ file 'Vec3InspectorQT.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Vec3InspectorQT.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Vec3InspectorQT.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TCM__Editor__QT__Vec3InspectorQT_t {
    QByteArrayData data[9];
    char stringdata0[104];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TCM__Editor__QT__Vec3InspectorQT_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TCM__Editor__QT__Vec3InspectorQT_t qt_meta_stringdata_TCM__Editor__QT__Vec3InspectorQT = {
    {
QT_MOC_LITERAL(0, 0, 32), // "TCM::Editor::QT::Vec3InspectorQT"
QT_MOC_LITERAL(1, 33, 10), // "OnXChanged"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 4), // "text"
QT_MOC_LITERAL(4, 50, 10), // "OnYChanged"
QT_MOC_LITERAL(5, 61, 10), // "OnZChanged"
QT_MOC_LITERAL(6, 72, 15), // "SetThirdMinSize"
QT_MOC_LITERAL(7, 88, 8), // "QWidget*"
QT_MOC_LITERAL(8, 97, 6) // "widget"

    },
    "TCM::Editor::QT::Vec3InspectorQT\0"
    "OnXChanged\0\0text\0OnYChanged\0OnZChanged\0"
    "SetThirdMinSize\0QWidget*\0widget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TCM__Editor__QT__Vec3InspectorQT[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x08 /* Private */,
       4,    1,   37,    2, 0x08 /* Private */,
       5,    1,   40,    2, 0x08 /* Private */,
       6,    1,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void TCM::Editor::QT::Vec3InspectorQT::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Vec3InspectorQT *_t = static_cast<Vec3InspectorQT *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnXChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->OnYChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->OnZChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->SetThirdMinSize((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject TCM::Editor::QT::Vec3InspectorQT::staticMetaObject = {
    { &VarInspectorQT::staticMetaObject, qt_meta_stringdata_TCM__Editor__QT__Vec3InspectorQT.data,
      qt_meta_data_TCM__Editor__QT__Vec3InspectorQT,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TCM::Editor::QT::Vec3InspectorQT::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TCM::Editor::QT::Vec3InspectorQT::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TCM__Editor__QT__Vec3InspectorQT.stringdata0))
        return static_cast<void*>(const_cast< Vec3InspectorQT*>(this));
    return VarInspectorQT::qt_metacast(_clname);
}

int TCM::Editor::QT::Vec3InspectorQT::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VarInspectorQT::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
