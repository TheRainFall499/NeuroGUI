#include "mainwindow.h"
#include "samplingthread.h"
#include <QApplication>

int main (int argc, char *argv[])
{
    SamplingThread samplingThread;
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    samplingThread.start();


    return a.exec();
}
