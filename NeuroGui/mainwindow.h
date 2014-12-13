#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
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
    void dataValueChanged(float);

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void displayDataPoint(long);
    void displayDataValue(float);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
