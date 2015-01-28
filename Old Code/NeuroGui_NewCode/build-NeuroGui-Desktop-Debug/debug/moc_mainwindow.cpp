/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../NeuroGui/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[10];
    char stringdata[177];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 16),
QT_MOC_LITERAL(2, 28, 0),
QT_MOC_LITERAL(3, 29, 20),
QT_MOC_LITERAL(4, 50, 21),
QT_MOC_LITERAL(5, 72, 22),
QT_MOC_LITERAL(6, 95, 21),
QT_MOC_LITERAL(7, 117, 16),
QT_MOC_LITERAL(8, 134, 20),
QT_MOC_LITERAL(9, 155, 21)
    },
    "MainWindow\0dataPointChanged\0\0"
    "longDataValueChanged\0shortDataValueChanged\0"
    "on_startButton_clicked\0on_stopButton_clicked\0"
    "displayDataPoint\0displayLongDataValue\0"
    "displayShortDataValue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       3,    1,   57,    2, 0x06 /* Public */,
       4,    1,   60,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   63,    2, 0x08 /* Private */,
       6,    0,   64,    2, 0x08 /* Private */,
       7,    1,   65,    2, 0x08 /* Private */,
       8,    1,   68,    2, 0x08 /* Private */,
       9,    1,   71,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Long,    2,
    QMetaType::Void, QMetaType::ULong,    2,
    QMetaType::Void, QMetaType::UShort,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Long,    2,
    QMetaType::Void, QMetaType::ULong,    2,
    QMetaType::Void, QMetaType::UShort,    2,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->dataPointChanged((*reinterpret_cast< long(*)>(_a[1]))); break;
        case 1: _t->longDataValueChanged((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        case 2: _t->shortDataValueChanged((*reinterpret_cast< unsigned short(*)>(_a[1]))); break;
        case 3: _t->on_startButton_clicked(); break;
        case 4: _t->on_stopButton_clicked(); break;
        case 5: _t->displayDataPoint((*reinterpret_cast< long(*)>(_a[1]))); break;
        case 6: _t->displayLongDataValue((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        case 7: _t->displayShortDataValue((*reinterpret_cast< unsigned short(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainWindow::*_t)(long );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::dataPointChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (MainWindow::*_t)(unsigned long );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::longDataValueChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (MainWindow::*_t)(unsigned short );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::shortDataValueChanged)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::dataPointChanged(long _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::longDataValueChanged(unsigned long _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::shortDataValueChanged(unsigned short _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
