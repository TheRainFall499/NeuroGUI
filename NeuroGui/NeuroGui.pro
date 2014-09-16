#-------------------------------------------------
#
# Project created by QtCreator 2014-09-09T10:08:33
#
#-------------------------------------------------

CONFIG += qwt

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NeuroGui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ULAI06.cpp

HEADERS  += mainwindow.h \
    cbw.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ -lcbw32
win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ -lcbw32

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ -lcbw64
win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ -lcbw64

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
