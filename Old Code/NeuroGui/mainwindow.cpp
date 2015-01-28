#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cbw.h"
#include <iostream>
#include <typeinfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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
    connect(this, SIGNAL(floatDataValueChanged(float)),
            this, SLOT(displayFloatDataValue(float)));
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
    unsigned Options;
    float revision = (float) CURRENTREVNUM;
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

    /*  set aside memory to hold data */
    MemHandle = cbWinBufAlloc(Count);
    ADData = (WORD*) MemHandle;

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
    while (Status == RUNNING)
    {
        qApp->processEvents();
        /* Check the status of the current background operation
        Parameters:
            BoardNum  :the number used by CB.CFG to describe this board
            Status    :current status of the operation (IDLE or RUNNING)
            CurCount  :current number of samples collected
            CurIndex  :index to the last data value transferred
            FunctionType: A/D operation (AIFUNCTIOM)*/
        ULStat = cbGetStatus (BoardNum, &Status, &CurCount, &CurIndex, AIFUNCTION);
        /* check the current status of the background operation */
        float *EngUnits;
        if ((Status == RUNNING) && CurCount > 0)
        {
            emit dataPointChanged(CurIndex);
            cbToEngUnits(BoardNum, Gain, ADData[CurIndex], EngUnits);
            emit floatDataValueChanged(*EngUnits);
        }
    }
    /* Data collection terminated */

    /* The BACKGROUND operation must be explicitly stopped
        Parameters:
             BoardNum    :the number used by CB.CFG to describe this board
             FunctionType: A/D operation (AIFUNCTIOM)*/
    ULStat = cbStopBackground (BoardNum, AIFUNCTION);

    cbWinBufFree(MemHandle);
}

void MainWindow::displayDataPoint(long dataPoint)
{
    ui->dataPointLabel->setText("Data point: " + QString::number(dataPoint));
}

void MainWindow::displayFloatDataValue(float floatDataValue) {
    ui->dataValueLabel->setText("Data value: " + QString::number(floatDataValue, 'g', 6));
}

void MainWindow::on_stopButton_clicked()
{
    //ULStat = cbStopBackground (BoardNum, AIFUNCTION);
    //cbWinBufFree(MemHandle);

    exit(1); // exits gui application
}
