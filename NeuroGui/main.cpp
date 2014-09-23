#include "mainwindow.h"
#include <QApplication>
#include "ULAI06.h"

int main (int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    Test *test = new Test();
    test->DisplayData();

    w.show();
    return a.exec();
}
