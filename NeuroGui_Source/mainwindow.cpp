#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plot.h"
#include "cbw.h"
#include <qwt_scale_engine.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include "samplingthread.h"
#include <iostream>
#include <typeinfo>
#include <qwt_system_clock.h>
#include "signaldata.h"
#include <QFileDialog>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QDateTime>
#include <QSlider>

WORD *ADData;
DWORD *ADData32;
int channels;
int Count;
HANDLE MemHandle = 0;
int globalrecfreq;
QFile f;
SamplingThread  samplingThread;
bool triggerchecked;
bool StartPlot;
int blocks_stored;
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->chan1name->setEnabled(false);
    ui->chan2name->setEnabled(false);
    ui->chan3name->setEnabled(false);
    ui->chan4name->setEnabled(false);
    ui->chan5name->setEnabled(false);
    ui->chan6name->setEnabled(false);
    ui->chan7name->setEnabled(false);
    ui->chan8name->setEnabled(false);
    loadSettings();
    StartPlot=false;
    PlotStarted_mainwindow=false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
          this, SLOT(MyTimerSlot()));
    triggerduration=1.0;
    tracker=1;
    append_data="_data.bin";
    append_text="_info.txt";
    blocks_stored=1;
    textfilessaved=0;
}

void MainWindow::MyTimerSlot()
{
    qDebug() << "running stop";
    MainWindow::on_stopButton_clicked();
    qDebug() <<"runstart?:"<< ui->trigger_multi->isChecked();
    if (ui->trigger_multi->isChecked())
        MainWindow::on_startButton_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    QSettings settings("ColemanLab","DAQCode");
    globaloutputfolder=settings.value("outputfolder").toString();
    globalsessionname=settings.value("sessionname").toString();

    QString filepath=QString( "%1\\%2_data.bin").arg(globaloutputfolder,globalsessionname);
    QFileInfo checkinfo(filepath);
    if (checkinfo.exists() && checkinfo.isFile())
    {        
        if (ui->setting_overwrite->isChecked())
        {
            f.remove(globaloutputfolder + "\\" + globalsessionname + append_data);
            f.remove(globaloutputfolder + "\\" + globalsessionname + append_text);
        }
        else
        {
            QMessageBox::critical(0,"Error", "File already exists in output folder.");
            return;
        }
    }

    if (ui->setting_increment->isChecked())
    {
        append_data=QString("_%1_data.bin").arg(tracker);
        append_text=QString("_%1_text.txt").arg(tracker);
        tracker=tracker+1;
        qDebug() << "tracker:" << tracker;
    }
    f.setFileName(globaloutputfolder + "\\" + globalsessionname + append_data);
    f.open(QIODevice::WriteOnly | QIODevice::Append);

    ui->stopButton->setEnabled(true);
    Plot *d_plot = new Plot(this);

    d_plot->setIntervalLength(5.0);

    //ui->horizontalLayout_5->addWidget(d_plot,1);
//    QVBoxLayout* vLayout1 = new QVBoxLayout();
//    QSlider *horizslider= new QSlider(Qt::Horizontal);
//    QSlider *vertslider= new QSlider(Qt::Vertical);
//    vLayout1->addWidget(d_plot);
//    vLayout1->addWidget(horizslider);
    //Construct Plot Widget

    QHBoxLayout* yboxes = new QHBoxLayout();
    QLineEdit* yminbox = new QLineEdit("-10");
    QLineEdit* ymaxbox = new QLineEdit("10");
    yboxes->addWidget(yminbox,1);
    yboxes->addWidget(ymaxbox,1);

    QVBoxLayout* zoomproperties = new QVBoxLayout();
    QLabel* yzoomlabel = new QLabel("Ymin/Ymax");
    QLabel* xzoomlabel = new QLabel("Window (s)");
    QLineEdit* xwindow = new QLineEdit("10");

    zoomproperties->addWidget(yzoomlabel,1);
    zoomproperties->addLayout(yboxes,1);
    zoomproperties->addWidget(xzoomlabel,1);
    zoomproperties->addWidget(xwindow,1);


    ui->horizontalLayout_5->addWidget(d_plot,100);
    ui->horizontalLayout_5->addLayout(zoomproperties,1);
    globalrecordingfrequency=settings.value("recordingfrequency").toInt();
    globalrecfreq=globalrecordingfrequency;
    globalchannelnumber=settings.value("channelnumber").toInt();



    /* Variable Declarations */
    int BoardNum = 0;
    int ULStat = 0;
    int LowChan = 0;
    int HighChan = globalchannelnumber-1;
    int Gain = BIP10VOLTS;
    short Status = RUNNING;
    long CurCount;
    long CurIndex;
    long Rate = (long)globalrecordingfrequency;
    Count = (HighChan+1)*globalrecordingfrequency*2;
    int divisor=30+HighChan+1;
    int remainder = Count % (divisor);
    while (remainder != 0)
    {
        Count=Count+1;
        remainder = Count % divisor;

        //qDebug() << "Count:" << Count << "Remainder:" << remainder;

    }
    channels = globalchannelnumber;
    unsigned Options;
    float revision = (float)CURRENTREVNUM;
    BOOL HighResAD = FALSE;
    int ADRes;

    samplingThread.setInterval( 0.1 );

   /* Declare Revision level of the Universal Library */
    ULStat = cbDeclareRevision(&revision);

    /* Initiate error handling
        Parameters:
            PRINTALL :all warnings and errors encountered will be printed
            DONTSTOP :program will continue even if error occurs.
                     Note that STOPALL and STOPFATAL are only effective in
                     Windows applications, not Console applications.
   */
    cbErrHandling (PRINTALL, DONTSTOP);

    /* Get the resolution of A/D */
    cbGetConfig(BOARDINFO, BoardNum, 0, BIADRES, &ADRes);

    /* check If the resolution of A/D is higher than 16 bit.
       If it is, then the A/D is high resolution. */
    if(ADRes > 16)
        HighResAD = TRUE;

    /*  set aside memory to hold data */
    if(HighResAD)
    {
        MemHandle = cbWinBufAlloc32(Count);
        ADData32 = (DWORD*) MemHandle;
    }
    else
    {
        MemHandle = cbWinBufAlloc(Count);
        ADData = (WORD*) MemHandle;
    }

    /* Make sure it is a valid pointer */
    if (!MemHandle)
        exit(1); /* out of memory */
    /* set up the display screen */
    /* Demonstration of cbAInScan() */
    /* Data are being collected in the BACKGROUND, CONTINUOUSLY */

    /* Collect the values with cbAInScan() in BACKGROUND mode, CONTINUOUSLY
        Parameters:
             BoardNum    :the number used by CB.CFG to describe this board
             LowChan     :low channel of the scan
             HighChan    :high channel of the scan
             Count       :the total number of A/D samples to collect
             Rate        :sample rate in samples per second
             Gain        :the gain for the board
             ADData[]    :the array for the collected data values
             Options     :data collection options */



    if (ui->usetrigger->isChecked())
    {
        /*
         Select the trigger source
        Parametes:
           BoardNum      :the number used by CB.CFG to describe this board
           TrigType      :the type of triggering based on the external trigger
                          source.
           LowThreshold  :Low threshold when trigger input is analog
           HighThreshold :High threshold when trigger input is analog

        */
         USHORT HighThreshold;
         USHORT LowThreshold;
         ULStat = cbFromEngUnits(BoardNum, BIP5VOLTS, 1.0, &HighThreshold);
         ULStat = cbFromEngUnits(BoardNum, BIP5VOLTS, 0.1f, &LowThreshold);

         /* AD conversions are enabled when the external
                                 analog trigger makes a transition from below
                                 HighThreshold to above. */
         ULStat = cbSetTrigger (BoardNum, TRIG_POS_EDGE, LowThreshold, HighThreshold);


        Options = CONVERTDATA + BACKGROUND + CONTINUOUS + EXTTRIGGER;
        qDebug() << "LowChan:" << LowChan;
        qDebug() << "HighChan:" << HighChan;
        ULStat = cbAInScan (BoardNum, LowChan, HighChan, Count, &Rate, Gain, MemHandle, Options);
        //ULStat = cbSetTrigger(0, GATEHIGH,0.1,4.5);

        samplingThread.start();
        qDebug() << "SamplingThread Started";
    }
    else
    {
        Options = CONVERTDATA + BACKGROUND + CONTINUOUS;
        ULStat = cbAInScan (BoardNum, LowChan, HighChan, Count, &Rate, Gain, MemHandle, Options);
        samplingThread.settriggerplot(true);
        samplingThread.start();
        qDebug() << "SamplingThread Started";
        d_plot->start();
    }


    /* Your program could be doing something useful here while data are collected */

    /* During the BACKGROUND operation, check the status */
    //float EngUnits;
    //QwtSystemClock checktimer;
    //checktimer.start();
    //StoredLocation=0;
    //float v;
    extern bool StartPlot;
    qDebug() << "Entering While";
    while (Status==RUNNING)
    {
        if (StartPlot==true )
        {
            qDebug() << "PlotStarted: " << PlotStarted_mainwindow;
            if (PlotStarted_mainwindow==false)
            {
                StartPlot=false;
                PlotStarted_mainwindow=true;
                d_plot->start();
                qDebug() << "Mainwindow started plot";
                if (ui->trigger_datablock->isChecked() || ui->trigger_multi->isChecked())
                {
                    QString tempStr = ui->trigger_duration->text();
                    double converted = tempStr.toDouble();
                    timer->setInterval(converted*1000);
                    timer->start();
                }
            }
        }
        qApp->processEvents();
//        // Check the status of the current background operation
//        /*Parameters:
//            BoardNum  :the number used by CB.CFG to describe this board
//            Status    :current status of the operation (IDLE or RUNNING)
//            CurCount  :current number of samples collected
//            CurIndex  :index to the last data value transferred
//            FunctionType: A/D operation (AIFUNCTIOM)*/
//        ULStat = cbGetStatus (BoardNum, &Status, &CurCount, &CurIndex,AIFUNCTION);

//        if (CurIndex==StoredLocation+1)
//        {
//            double elapsed=checktimer.elapsed();
//            elapsed=elapsed/1000;
//            int ENG = cbToEngUnits(BoardNum,BIP5VOLTS,ADData[CurIndex],&v);
//            double v_double=(double)v;
//            const QPointF s(elapsed, 1.0);
//            SignalData::instance().append( s );
//            StoredLocation=CurIndex;
//            //printf("IIndex:%d\n",CurIndex);
//            checktimer.restart();
//        }
//            //printf("OIndex:%d",CurIndex);
////        // check the current status of the background operation */

        if ((Status == RUNNING) && CurCount > 0)
        {

//          int ENG = cbToEngUnits(BoardNum,BIP5VOLTS,ADData[CurIndex],&EngUnits);
//          double v_double=static_cast<double>(EngUnits);
//            printf ("  Value: %d  ",v_double);
//            emit dataValueChanged(*EngUnits);

        }
    }




    /* Data collection terminated */

    /* The BACKGROUND operation must be explicitly stopped
        Parameters:
             BoardNum    :the number used by CB.CFG to describe this board
             FunctionType: A/D operation (AIFUNCTIOM)*/

}

void MainWindow::on_pushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QSettings setting("ColemanLab","DAQCode");
    setting.setValue("outputfolder",dir);
    ui->output_display->setText(dir);
}


void MainWindow::on_inputsession_editingFinished()
{
    QString sessionname = ui->inputsession->text();
    QSettings setting("ColemanLab","DAQCode");
    setting.setValue("sessionname",sessionname);
    if (ui->setting_increment->isChecked())
    {
        append_data=QString("_%1_data.bin").arg(tracker);
        append_text=QString("_%1_text.txt").arg(tracker);
        ui->output_file1->setText(sessionname+append_data);
        ui->output_file2->setText(sessionname+append_text);
    }
    else
    {
        ui->output_file1->setText(sessionname+append_data);
        ui->output_file2->setText(sessionname+append_text);
    }
}

void MainWindow::on_inputrecordingfreq_editingFinished()
{
    QString recordingfrequency = ui->inputrecordingfreq->text();
    QSettings setting("ColemanLab","DAQCode");
    setting.setValue("recordingfrequency", recordingfrequency);
    ui->output_recfreq->setText("Currently: "+recordingfrequency+" Hz");
}

void MainWindow::on_chancounter_valueChanged(double value)
{
    double chans = ui->chancounter->value();
    channels = (int)chans;

    QString savechans = QString::number(channels);
    QSettings setting("ColemanLab","DAQCode");
    setting.setValue("channelnumber", savechans);
    modifyenabledchans(channels);
}

void MainWindow::on_chan1name_editingFinished()
{
    QString channame = ui->chan1name->text();
    QSettings setting("ColemanLab","DAQCode");
    setting.setValue("chan1name",channame);
}

void MainWindow::on_chan2name_editingFinished()
{
    QString channame = ui->chan2name->text();
    QSettings setting("ColemanLab","DAQCode");
    setting.setValue("chan2name",channame);
}

void MainWindow::on_chan3name_editingFinished()
{
    QString channame = ui->chan3name->text();
    QSettings setting("ColemanLab","DAQCode");
    setting.setValue("chan3name",channame);
}

void MainWindow::on_chan4name_editingFinished()
{
    QString channame = ui->chan4name->text();
    QSettings setting("ColemanLab","DAQCode");
    setting.setValue("chan4name",channame);
}

void MainWindow::on_chan5name_editingFinished()
{
    QString channame = ui->chan5name->text();
    QSettings setting("ColemanLab","DAQCode");
    setting.setValue("chan5name",channame);
}

void MainWindow::on_chan6name_editingFinished()
{
    QString channame = ui->chan6name->text();
    QSettings setting("ColemanLab","DAQCode");
    setting.setValue("chan6name",channame);
}

void MainWindow::on_chan7name_editingFinished()
{
    QString channame = ui->chan7name->text();
    QSettings setting("ColemanLab","DAQCode");
    setting.setValue("chan7name",channame);
}

void MainWindow::on_chan8name_editingFinished()
{
    QString channame = ui->chan8name->text();
    QSettings setting("ColemanLab","DAQCode");
    setting.setValue("chan8name",channame);
}

void MainWindow::on_pushButton_2_clicked()
{

    bool validate;
    QString tempStr;
    tempStr = ui->trigger_duration->text();
    double converted = tempStr.toDouble(&validate);
    if (validate == true)
    {
        if (converted < 1)
            validate = false;
    }

    int completesum = 0;
    QSettings checksettings("ColemanLab", "DAQCode");
    if(!checksettings.contains("outputfolder"))
    {
        QMessageBox::critical(0,"Error", "Missing output folder.");
    }
    else if (!checksettings.contains("sessionname"))
    {
        QMessageBox::critical(0,"Error", "Missing custom session name.");
    }
    else if (!checksettings.contains("recordingfrequency"))
    {
        QMessageBox::critical(0,"Error", "Missing recording frequency.");
    }
    else if (!checksettings.contains("channelnumber"))
    {
        QMessageBox::critical(0,"Error", "Missing channelnumber.");
    }
    else if (validate == false)
    {
        QMessageBox::critical(0,"Error", "Timer duration must be a number >= 1.0");
    }
    else
    {
        completesum=1;
    }
    if (completesum == 1)
    {
        ui->startButton->setEnabled(true);
    }

}



void MainWindow::loadSettings()
{
    QSettings settings("ColemanLab","DAQCode");
    if (settings.contains("outputfolder"))
    {
        QString output = settings.value("outputfolder", "").toString();
        ui->output_display->setText(output);
    }
    if (settings.contains("sessionname"))
    {
        QString output = settings.value("sessionname", "").toString();
        ui->inputsession->setText(output);
        ui->output_file1->setText(output+"_data.bin");
        ui->output_file2->setText(output+"_info.txt");
    }
    if (settings.contains("recordingfrequency"))
    {
        QString output = settings.value("recordingfrequency", "").toString();
        ui->inputrecordingfreq->setText(output);
        ui->output_recfreq->setText(output);
    }
    if (settings.contains("channelnumber"))
    {
        int output = settings.value("channelnumber", "").toInt();
        ui->chancounter->setValue(output);
        modifyenabledchans(output);
    }
}

void MainWindow::modifyenabledchans(int chansvisible)
{
    if (chansvisible==0)
    {
        ui->chan1name->setEnabled(false);
        ui->chan2name->setEnabled(false);
        ui->chan3name->setEnabled(false);
        ui->chan4name->setEnabled(false);
        ui->chan5name->setEnabled(false);
        ui->chan6name->setEnabled(false);
        ui->chan7name->setEnabled(false);
        ui->chan8name->setEnabled(false);
    }

    if (chansvisible==1)
    {
        ui->chan1name->setEnabled(true);
        ui->chan2name->setEnabled(false);
        ui->chan3name->setEnabled(false);
        ui->chan4name->setEnabled(false);
        ui->chan5name->setEnabled(false);
        ui->chan6name->setEnabled(false);
        ui->chan7name->setEnabled(false);
        ui->chan8name->setEnabled(false);
    }
    if (chansvisible==2)
    {
        ui->chan1name->setEnabled(true);
        ui->chan2name->setEnabled(true);
        ui->chan3name->setEnabled(false);
        ui->chan4name->setEnabled(false);
        ui->chan5name->setEnabled(false);
        ui->chan6name->setEnabled(false);
        ui->chan7name->setEnabled(false);
        ui->chan8name->setEnabled(false);
    }
    if (chansvisible==3)
    {
        ui->chan1name->setEnabled(true);
        ui->chan2name->setEnabled(true);
        ui->chan3name->setEnabled(true);
        ui->chan4name->setEnabled(false);
        ui->chan5name->setEnabled(false);
        ui->chan6name->setEnabled(false);
        ui->chan7name->setEnabled(false);
        ui->chan8name->setEnabled(false);
    }
    if (chansvisible==4)
    {
        ui->chan1name->setEnabled(true);
        ui->chan2name->setEnabled(true);
        ui->chan3name->setEnabled(true);
        ui->chan4name->setEnabled(true);
        ui->chan5name->setEnabled(false);
        ui->chan6name->setEnabled(false);
        ui->chan7name->setEnabled(false);
        ui->chan8name->setEnabled(false);
    }
    if (chansvisible==5)
    {
        ui->chan1name->setEnabled(true);
        ui->chan2name->setEnabled(true);
        ui->chan3name->setEnabled(true);
        ui->chan4name->setEnabled(true);
        ui->chan5name->setEnabled(true);
        ui->chan6name->setEnabled(false);
        ui->chan7name->setEnabled(false);
        ui->chan8name->setEnabled(false);
    }
    if (chansvisible==6)
    {
        ui->chan1name->setEnabled(true);
        ui->chan2name->setEnabled(true);
        ui->chan3name->setEnabled(true);
        ui->chan4name->setEnabled(true);
        ui->chan5name->setEnabled(true);
        ui->chan6name->setEnabled(true);
        ui->chan7name->setEnabled(false);
        ui->chan8name->setEnabled(false);
    }
    if (chansvisible==7)
    {
        ui->chan1name->setEnabled(true);
        ui->chan2name->setEnabled(true);
        ui->chan3name->setEnabled(true);
        ui->chan4name->setEnabled(true);
        ui->chan5name->setEnabled(true);
        ui->chan6name->setEnabled(true);
        ui->chan7name->setEnabled(true);
        ui->chan8name->setEnabled(false);
    }
    if (chansvisible==8)
    {
        ui->chan1name->setEnabled(true);
        ui->chan2name->setEnabled(true);
        ui->chan3name->setEnabled(true);
        ui->chan4name->setEnabled(true);
        ui->chan5name->setEnabled(true);
        ui->chan6name->setEnabled(true);
        ui->chan7name->setEnabled(true);
        ui->chan8name->setEnabled(true);
    }
}


void MainWindow::on_stopButton_clicked()
{
    int ULStat = cbStopBackground (0,AIFUNCTION);


    //
    samplingThread.stop();

    while (samplingThread.isRunning())
    {
        qDebug() << "waiting for samp thread to stop...";
    }
    qDebug() << "flushing data";
    samplingThread.flush_data();
    while (samplingThread.isRunning())
    {
        qDebug() << "waiting for samp thread to stop...";
    }
    qDebug() << "resetting trigger plot";
    samplingThread.settriggerplot(false);
    while (samplingThread.isRunning())
    {
        qDebug() << "waiting for samp thread to stop...";
    }
    if (!ui->savesettings->isChecked())
    {
        QSettings settings("ColemanLab","DAQCode");
        settings.remove("outputfolder");
        settings.remove("sessionname");
        settings.remove("recordingfrequency");
        settings.remove("channelnumber");
        settings.remove("chan1name");
        settings.remove("chan2name");
        settings.remove("chan3name");
        settings.remove("chan4name");
        settings.remove("chan5name");
        settings.remove("chan6name");
        settings.remove("chan7name");
        settings.remove("chan8name");
    }

    f.close();
    cbWinBufFree(MemHandle);

    if (ui->onetext->isChecked())
    {
        if (textfilessaved == 0)
        {
            write_text_file();
            textfilessaved=1;
        }
    }
    else
    {
        write_text_file();
    }
//    delete vertslider;
//    delete horizslider;
//    delete d_plot;
//    delete MainWindow::testslider;
//    QLayoutItem *child;
//    while ((child=ui->horizontalLayout_5->takeAt(0))!=0)
//    {
//        delete child;
//    }


    clearLayout(ui->horizontalLayout_5);

    ui->stopButton->setEnabled(false);
    StartPlot=false;
    PlotStarted_mainwindow=false;
    if (samplingThread.isFinished())
        qDebug() << "Thread is done";


}

void MainWindow::clearLayout(QLayout *layout)
{
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != 0) {
        if(child->layout() != 0)
            clearLayout( child->layout() );
        else if(child->widget() != 0)
            delete child->widget();

        delete child;
    }
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    exit(1);
}

void MainWindow::write_text_file()
{
    QFile outfile;
    outfile.setFileName(globaloutputfolder + "\\" + globalsessionname + append_text);
    outfile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out(&outfile);
    out << "STORED OPTIONS, FILE = " + globalsessionname << endl;

    out << "Time of Recording (recording stopped)" << endl;
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString();
    out << dateTimeString << endl;

    out << "Session Name" << endl;
    out << ui->inputsession->text() << endl;

    out << "Recording Frequency" << endl;
    out << ui->inputrecordingfreq->text() << endl;

    out << QString("Total Channels: %1").arg(channels) << endl;

    if (channels >= 1)
    {
        out << "Channel 1 Name: ";
        out << ui->chan1name->text() << endl;
    }
    if (channels >= 2)
    {
        out << "Channel 2 Name: ";
        out << ui->chan2name->text() << endl;
    }
    if (channels >= 3)
    {
        out << "Channel 3 Name: ";
        out << ui->chan3name->text() << endl;
    }
    if (channels >=4 )
    {
        out << "Channel 4 Name: ";
        out << ui->chan4name->text() << endl;
    }
    if (channels >= 5)
    {
        out << "Channel 5 Name: ";
        out << ui->chan5name->text() << endl;
    }
    if (channels >=6 )
    {
        out << "Channel 6 Name: ";
        out << ui->chan6name->text() << endl;
    }
    if (channels >=7 )
    {
        out << "Channel 7 Name: ";
        out << ui->chan7name->text() << endl;
    }
    if (channels == 8 )
    {
        out << "Channel 8 Name: ";
        out << ui->chan8name->text() << endl;
    }
    out << "Experimental Notes" << endl;
    out << ui->textEdit->toPlainText() << endl;

    outfile.close();
}

void MainWindow::on_setting_overwrite_clicked()
{

}

void MainWindow::on_usetrigger_clicked()
{
    triggerchecked = ui->usetrigger->isChecked();
}


void MainWindow::on_trigger_duration_editingFinished()
{
    bool validate;
    QString tempStr;
    tempStr = ui->trigger_duration->text();
    double converted = tempStr.toDouble(&validate);
    if (validate == false)
    {
        QMessageBox::critical(0,"Error", "Timer duration must be a number.");
        ui->trigger_duration->setText("0");
    }
    else
    {
        triggerduration=converted;
    }
}
