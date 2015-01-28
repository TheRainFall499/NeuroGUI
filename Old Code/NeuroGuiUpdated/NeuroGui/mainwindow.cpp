#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plot.h"
#include "cbw.h"
#include <qwt_scale_engine.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->channelLabel->setText("Channels: ");
    ui->horizontalSlider->setMaximum(8);
    ui->horizontalSlider->setMinimum(0);
    ui->spinBox->setMaximum(8);
    ui->spinBox->setMinimum(0);

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)),
            ui->spinBox, SLOT(setValue(int)));
    connect(ui->spinBox, SIGNAL(valueChanged(int)),
            ui->horizontalSlider, SLOT(setValue(int)));

    ui->startButton->setText("Start");
    ui->stopButton->setText("Stop");

    connect(this, SIGNAL(dataPointChanged(long)),
            this, SLOT(displayDataPoint(long)));
    connect(this, SIGNAL(longDataValueChanged(unsigned long)),
            this, SLOT(displayLongDataValue(unsigned long)));
    connect(this, SIGNAL(shortDataValueChanged(unsigned short)),
            this, SLOT(displayShortDataValue(unsigned short)));

    d_plot = new Plot(this);
    d_plot->setIntervalLength(10.0);
    d_plot->start();
    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->addWidget( d_plot, 1 );


}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    /* Variable Declarations */
    int BoardNum = 0;
    int ULStat = 0;
    int LowChan = 0;
    int HighChan = 0;
    int Gain = BIP5VOLTS;
    short Status = RUNNING;
    long CurCount;
    long CurIndex;
    long Count = 10000;
    long Rate = 390;
    HANDLE MemHandle = 0;
    WORD *ADData;
    DWORD *ADData32;
    unsigned Options;
    float revision = (float)CURRENTREVNUM;
    BOOL HighResAD = FALSE;
    int ADRes;

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
    Options = CONVERTDATA + BACKGROUND + CONTINUOUS;
    ULStat = cbAInScan (BoardNum, LowChan, HighChan, Count, &Rate, Gain,
                        MemHandle, Options);

    /* Your program could be doing something useful here while data are collected */

    /* During the BACKGROUND operation, check the status */

    while (Status==RUNNING)
    {
        qApp->processEvents();
        // Check the status of the current background operation
        /*Parameters:
            BoardNum  :the number used by CB.CFG to describe this board
            Status    :current status of the operation (IDLE or RUNNING)
            CurCount  :current number of samples collected
            CurIndex  :index to the last data value transferred
            FunctionType: A/D operation (AIFUNCTIOM)*/
        ULStat = cbGetStatus (BoardNum, &Status, &CurCount, &CurIndex,AIFUNCTION);
        // check the current status of the background operation */

        if ((Status == RUNNING) && CurCount > 0)
        {
            emit dataPointChanged(CurIndex);
            if(HighResAD)
                emit longDataValueChanged(ADData32[CurIndex]);
            else
                emit shortDataValueChanged(ADData[CurIndex]);
        }
    }




    /* Data collection terminated */

    /* The BACKGROUND operation must be explicitly stopped
        Parameters:
             BoardNum    :the number used by CB.CFG to describe this board
             FunctionType: A/D operation (AIFUNCTIOM)*/
    ULStat = cbStopBackground (BoardNum,AIFUNCTION);

    cbWinBufFree(MemHandle);
}

void MainWindow::displayDataPoint(long dataPoint)
{
    ui->dataPointLabel->setText("Data point: " + QString::number(dataPoint));
}

void MainWindow::displayLongDataValue(unsigned long longDataValue)
{
    ui->dataValueLabel->setText("Data value: " + QString::number(longDataValue));
}

void MainWindow::displayShortDataValue(unsigned short shortDataValue)
{
    ui->dataValueLabel->setText("Data value: " + QString::number(shortDataValue));
}

void MainWindow::on_stopButton_clicked()
{
    exit(1); // exits background operation and exits gui application
}
