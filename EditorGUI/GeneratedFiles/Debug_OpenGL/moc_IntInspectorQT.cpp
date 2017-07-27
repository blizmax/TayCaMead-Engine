/****************************************************************************
** Meta object code from reading C++ file 'IntInspectorQT.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../IntInspectorQT.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'IntInspectorQT.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TCM__Editor__QT__IntInspectorQT_t {
    QByteArrayData data[4];
    char stringdata0[54];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TCM__Editor__QT__IntInspectorQT_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TCM__Editor__QT__IntInspectorQT_t qt_meta_stringdata_TCM__Editor__QT__IntInspectorQT = {
    {
QT_MOC_LITERAL(0, 0, 31), // "TCM::Editor::QT::IntInspectorQT"
QT_MOC_LITERAL(1, 32, 14), // "OnValueChanged"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 5) // "value"

    },
    "TCM::Editor::QT::IntInspectorQT\0"
    "OnValueChanged\0\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TCM__Editor__QT__IntInspectorQT[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void TCM::Editor::QT::IntInspectorQT::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IntInspectorQT *_t = static_cast<IntInspectorQT *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnValueChanged((*reinterpret_cast< const int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject TCM::Editor::QT::IntInspectorQT::staticMetaObject = {
    { &VarInspectorQT::staticMetaObject, qt_meta_stringdata_TCM__Editor__QT__IntInspectorQT.data,
      qt_meta_data_TCM__Editor__QT__IntInspectorQT,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TCM::Editor::QT::IntInspectorQT::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TCM::Editor::QT::IntInspectorQT::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TCM__Editor__QT__IntInspectorQT.stringdata0))
        return static_cast<void*>(const_cast< IntInspectorQT*>(this));
    return VarInspectorQT::qt_metacast(_clname);
}

int TCM::Editor::QT::IntInspectorQT::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VarInspectorQT::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
