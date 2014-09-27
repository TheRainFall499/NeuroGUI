#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ULAI06.h"

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

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->spinBox, SLOT(setValue(int)));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), ui->horizontalSlider, SLOT(setValue(int)));

    ui->startButton->setText("Start");
    ui->stopButton->setText("Stop");

    connect(this, SIGNAL(dataPointChanged(long)), this, SLOT(displayDataPoint(long)));
    connect(this, SIGNAL(longDataValueChanged(unsigned long)), this, SLOT(displayLongDataValue(unsigned long)));
    connect(this, SIGNAL(shortDataValueChanged(unsigned short)), this, SLOT(displayShortDataValue(unsigned short)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

Test *test = new Test();
int Row, Col;
int BoardNum = 0;
int ULStat = 0;
short Status = RUNNING;
HANDLE MemHandle = 0;

void MainWindow::on_startButton_clicked()
{
    /* Variable Declarations */
    //int Row, Col;
    //int BoardNum = 0;
    //int ULStat = 0;
    int LowChan = 0;
    int HighChan = 0;
    int Gain = BIP5VOLTS;
    //short Status = RUNNING;
    long CurCount;
    long CurIndex;
    long Count = 10000;
    long Rate = 390;
    //HANDLE MemHandle = 0;
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

    if (!MemHandle)    /* Make sure it is a valid pointer */
    {
        //printf("\nout of memory\n");
        exit(1);
    }

    /* set up the display screen */
    test->ClearScreen();
    //printf ("Demonstration of cbAInScan()\n\n");
    //printf ("Data are being collected in the BACKGROUND, CONTINUOUSLY\n");


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

    //printf ("--------------------------------------------------------------------------------");
    //printf ("| Your program could be doing something useful here while data are collected...|");
    //printf ("--------------------------------------------------------------------------------");
    //printf ("\nCollecting data...\n\n");
    //printf ("Press any key to quit.\n\n");

    test->GetTextCursor (&Col, &Row);

    /* During the BACKGROUND operation, check the status */
    while (!kbhit() && Status==RUNNING)
    {
        /* Check the status of the current background operation
        Parameters:
            BoardNum  :the number used by CB.CFG to describe this board
            Status    :current status of the operation (IDLE or RUNNING)
            CurCount  :current number of samples collected
            CurIndex  :index to the last data value transferred
            FunctionType: A/D operation (AIFUNCTIOM)*/
        ULStat = cbGetStatus (BoardNum, &Status, &CurCount, &CurIndex,AIFUNCTION);
        /* check the current status of the background operation */
        if ((Status == RUNNING) && CurCount > 0)
        {
            test->MoveCursor (Col, Row);
            printf ("Data point: %3ld   ", CurIndex); //
            emit dataPointChanged(CurIndex);
            if(HighResAD) {
                printf ("  Value: %lu  ",ADData32[CurIndex]);
                emit longDataValueChanged(ADData32[CurIndex]);
            }
            else {
                printf ("  Value: %hu  ",ADData[CurIndex]);
                emit shortDataValueChanged(ADData[CurIndex]);
            }
        }
    }
}

void MainWindow::displayDataPoint(long dataPoint) {
    ui->dataPointLabel->setText("Data point: " + QString::number(dataPoint));
}

void MainWindow::displayLongDataValue(unsigned long longDataValue) {
    ui->dataValueLabel->setText("Data value: " + QString::number(longDataValue));
}

void MainWindow::displayShortDataValue(unsigned short shortDataValue) {
    ui->dataValueLabel->setText("Data value: " + QString::number(shortDataValue));
}

void MainWindow::on_stopButton_clicked()
{
    Status = IDLE;

    //printf ("\n");
    test->MoveCursor (Col, Row + 2);
    //printf ("Data collection terminated.");

    /* The BACKGROUND operation must be explicitly stopped
        Parameters:
             BoardNum    :the number used by CB.CFG to describe this board
             FunctionType: A/D operation (AIFUNCTIOM)*/
    ULStat = cbStopBackground (BoardNum,AIFUNCTION);

    cbWinBufFree(MemHandle);
    test->MoveCursor (1, 22);
    //printf ("\n");
}
