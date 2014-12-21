#-------------------------------------------------
#
# Project created by QtCreator 2014-09-09T10:08:33
#
#-------------------------------------------------

CONFIG += qwt

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NeuroGui

TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    datacollector.cpp

HEADERS += mainwindow.h \
    cbw.h \
    datacollector.h

FORMS += mainwindow.ui

RESOURCES += \
    Resources.qrc

# Enter your own file paths here
LIBS += "C:\Qt\Tools\QtCreator\test\QT_USBDAQ_Project\NeuroGui\cbi_cal.dll"
LIBS += "C:\Qt\Tools\QtCreator\test\QT_USBDAQ_Project\NeuroGui\cbi_node.dll"
LIBS += "C:\Qt\Tools\QtCreator\test\QT_USBDAQ_Project\NeuroGui\cbi_prop.dll"
LIBS += "C:\Qt\Tools\QtCreator\test\QT_USBDAQ_Project\NeuroGui\cbi_test.dll"
LIBS += "C:\Qt\Tools\QtCreator\test\QT_USBDAQ_Project\NeuroGui\cbw32.dll"
LIBS += "C:\Qt\Tools\QtCreator\test\QT_USBDAQ_Project\NeuroGui\DaqDevInfo.dll"
LIBS += "C:\Qt\Tools\QtCreator\test\QT_USBDAQ_Project\NeuroGui\DaqLib.dll"
LIBS += "C:\Qt\Tools\QtCreator\test\QT_USBDAQ_Project\NeuroGui\MccDaq.dll"
LIBS += "C:\Qt\Tools\QtCreator\test\QT_USBDAQ_Project\NeuroGui\MccSktsIfc.dll"

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ -lcbw32
win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ -lcbw32
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ -lcbw64
win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ -lcbw64

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
