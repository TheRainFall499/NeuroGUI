
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qwidget.h>
#include <qwt_plot.h>
#include "cbw.h"
#include <QSlider>
#include <QPushButton>

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

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void MyTimerSlot();


    void on_pushButton_clicked();

    void on_inputsession_editingFinished();

    void on_inputrecordingfreq_editingFinished();

    void on_chancounter_valueChanged(double value);

    void on_chan1name_editingFinished();

    void on_chan2name_editingFinished();

    void on_chan3name_editingFinished();

    void on_chan4name_editingFinished();

    void on_chan5name_editingFinished();

    void on_chan6name_editingFinished();

    void on_chan7name_editingFinished();

    void on_chan8name_editingFinished();

    void on_pushButton_2_clicked();

    void on_setting_overwrite_clicked();

    void on_usetrigger_clicked();

    void on_trigger_duration_editingFinished();

    void write_text_file();

    void on_chan1Color_clicked();

    void on_chan2Color_clicked();

    void on_chan3Color_clicked();

    void on_chan4Color_clicked();

    void on_chan5Color_clicked();

    void on_chan6Color_clicked();

    void on_chan7Color_clicked();

    void on_chan8Color_clicked();

private:
    Ui::MainWindow *ui;
    void loadSettings();
    void modifyenabledchans(int chansvisible);
    void setChanColor(QPushButton *chanColor, int chanNum);
    void closeEvent(QCloseEvent *bar);
    void clearLayout(QLayout *layout);
    QTimer *timer;
    double triggerduration;
    int tracker;
    QString append_data;
    QString append_text;

public:
    int StoredLocation;
    QString globaloutputfolder;
    QString globalsessionname;
    int globalrecordingfrequency;
    int globalchannelnumber;
    bool PlotStarted_mainwindow;
    int textfilessaved;




};

#endif // MAINWINDOW_H
