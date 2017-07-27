/****************************************************************************
** Meta object code from reading C++ file 'RigidDataInspectorQT.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../RigidDataInspectorQT.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RigidDataInspectorQT.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TCM__Editor__QT__RigidDataInspectorQT_t {
    QByteArrayData data[9];
    char stringdata0[143];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TCM__Editor__QT__RigidDataInspectorQT_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TCM__Editor__QT__RigidDataInspectorQT_t qt_meta_stringdata_TCM__Editor__QT__RigidDataInspectorQT = {
    {
QT_MOC_LITERAL(0, 0, 37), // "TCM::Editor::QT::RigidDataIns..."
QT_MOC_LITERAL(1, 38, 13), // "OnMassChanged"
QT_MOC_LITERAL(2, 52, 0), // ""
QT_MOC_LITERAL(3, 53, 5), // "value"
QT_MOC_LITERAL(4, 59, 17), // "OnFrictionChanged"
QT_MOC_LITERAL(5, 77, 20), // "OnRestitutionChanged"
QT_MOC_LITERAL(6, 98, 17), // "OnUseGravyChanged"
QT_MOC_LITERAL(7, 116, 5), // "state"
QT_MOC_LITERAL(8, 122, 20) // "OnIsKinematicChanged"

    },
    "TCM::Editor::QT::RigidDataInspectorQT\0"
    "OnMassChanged\0\0value\0OnFrictionChanged\0"
    "OnRestitutionChanged\0OnUseGravyChanged\0"
    "state\0OnIsKinematicChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TCM__Editor__QT__RigidDataInspectorQT[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x09 /* Protected */,
       4,    1,   42,    2, 0x09 /* Protected */,
       5,    1,   45,    2, 0x09 /* Protected */,
       6,    1,   48,    2, 0x09 /* Protected */,
       8,    1,   51,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Bool,    7,

       0        // eod
};

void TCM::Editor::QT::RigidDataInspectorQT::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RigidDataInspectorQT *_t = static_cast<RigidDataInspectorQT *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnMassChanged((*reinterpret_cast< const float(*)>(_a[1]))); break;
        case 1: _t->OnFrictionChanged((*reinterpret_cast< const float(*)>(_a[1]))); break;
        case 2: _t->OnRestitutionChanged((*reinterpret_cast< const float(*)>(_a[1]))); break;
        case 3: _t->OnUseGravyChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->OnIsKinematicChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject TCM::Editor::QT::RigidDataInspectorQT::staticMetaObject = {
    { &VarInspectorQT::staticMetaObject, qt_meta_stringdata_TCM__Editor__QT__RigidDataInspectorQT.data,
      qt_meta_data_TCM__Editor__QT__RigidDataInspectorQT,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TCM::Editor::QT::RigidDataInspectorQT::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TCM::Editor::QT::RigidDataInspectorQT::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TCM__Editor__QT__RigidDataInspectorQT.stringdata0))
        return static_cast<void*>(const_cast< RigidDataInspectorQT*>(this));
    return VarInspectorQT::qt_metacast(_clname);
}

int TCM::Editor::QT::RigidDataInspectorQT::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VarInspectorQT::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
