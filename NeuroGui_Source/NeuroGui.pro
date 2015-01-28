
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
    curvedata.cpp \
    samplingthread.cpp \
    signaldata.cpp \
    plot.cpp \
    writedatathread.cpp \
    customslider.cpp

HEADERS += mainwindow.h \
    cbw.h \
    curvedata.h \
    plot.h \
    samplingthread.h \
    signaldata.h \
    writedatathread.h \
    customslider.h

FORMS += mainwindow.ui

RESOURCES += \
    Resources.qrc

# Enter your own file paths here
LIBS += "C:\Users\Elijah\Desktop\New folder\NeuroGui\cbi_cal.dll"
LIBS += "C:\Users\Elijah\Desktop\New folder\NeuroGui\cbi_node.dll"
LIBS += "C:\Users\Elijah\Desktop\New folder\NeuroGui\cbi_prop.dll"
LIBS += "C:\Users\Elijah\Desktop\New folder\NeuroGui\cbi_test.dll"
LIBS += "C:\Users\Elijah\Desktop\New folder\NeuroGui\cbw32.dll"
LIBS += "C:\Users\Elijah\Desktop\New folder\NeuroGui\DaqDevInfo.dll"
LIBS += "C:\Users\Elijah\Desktop\New folder\NeuroGui\DaqLib.dll"
LIBS += "C:\Users\Elijah\Desktop\New folder\NeuroGui\MccDaq.dll"
LIBS += "C:\Users\Elijah\Desktop\New folder\NeuroGui\MccSktsIfc.dll"

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ -lcbw32
win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ -lcbw32
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ -lcbw64
win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ -lcbw64

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
