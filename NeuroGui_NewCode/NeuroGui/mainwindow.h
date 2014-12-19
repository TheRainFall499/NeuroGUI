
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qwidget.h>
#include <qwt_plot.h>
#include "cbw.h"

class Plot;

namespace Ui
{
    class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void dataPointChanged(long);
    void longDataValueChanged(unsigned long);
    void shortDataValueChanged(unsigned short);
    void dataValueChanged(float);

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void displayDataPoint(long);
    void displayLongDataValue(unsigned long);
    void displayShortDataValue(unsigned short);
    void displayDataValue(float);


private:
    Ui::MainWindow *ui;
    Plot *d_plot;

    int StoredLocation;

};

#endif // MAINWINDOW_H
