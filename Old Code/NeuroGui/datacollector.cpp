#include "datacollector.h"

DataCollector::DataCollector(int BoardNum, int LowChan, int HighChan, int Gain, long Count, long Rate) {
    this->BoardNum = BoardNum;
    this->LowChan = LowChan;
    this->HighChan = HighChan;
    this->Gain = Gain;
    this->Count = Count;
    this->Rate = Rate;

    this->ULStat = 0;
    this->Status = RUNNING;
    this->MemHandle = 0;
    this->revision = (float) CURRENTREVNUM;
}

void DataCollector::startOperation() {
//    Declare Revision level of the Universal Library
    ULStat = cbDeclareRevision(&revision);

//    Initiate error handling
//        Parameters:
//            PRINTALL :all warnings and errors encountered will be printed
//            DONTSTOP :program will continue even if error occurs.
//                     Note that STOPALL and STOPFATAL are only effective in
//                     Windows applications, not Console applications.
    cbErrHandling (PRINTALL, DONTSTOP);

//    Get the resolution of A/D
    cbGetConfig(BOARDINFO, BoardNum, 0, BIADRES, &ADRes);

//    Set aside memory to hold data
    MemHandle = cbWinBufAlloc(Count);
    ADData = (WORD*) MemHandle;

//    Make sure it is a valid pointer
    if (!MemHandle) {
//    Out of memory
        exit(1);
    }

//    Set up the display screen
//    Demonstration of cbAInScan()
//    Data are being collected in the BACKGROUND, CONTINUOUSLY

//    Collect the values with cbAInScan() in BACKGROUND mode, CONTINUOUSLY
//        Parameters:
//             BoardNum    :the number used by CB.CFG to describe this board
//             LowChan     :low channel of the scan
//             HighChan    :high channel of the scan
//             Count       :the total number of A/D samples to collect
//             Rate        :sample rate in samples per second
//             Gain        :the gain for the board
//             ADData[]    :the array for the collected data values
//             Options     :data collection options
    Options = CONVERTDATA + BACKGROUND + CONTINUOUS;
    ULStat = cbAInScan (BoardNum, LowChan, HighChan, Count, &Rate, Gain,
                        MemHandle, Options);
}

bool DataCollector::isRunning() {
    if(Status == RUNNING) {
        return true;
    }
    else {
        return false;
    }
}

void DataCollector::updateStatus() {
//    Check the status of the current background operation
//        Parameters:
//            BoardNum  :the number used by CB.CFG to describe this board
//            Status    :current status of the operation (IDLE or RUNNING)
//            CurCount  :current number of samples collected
//            CurIndex  :index to the last data value transferred
//            FunctionType: A/D operation (AIFUNCTIOM) */
    ULStat = cbGetStatus (BoardNum, &Status, &CurCount, &CurIndex, AIFUNCTION);
}

int DataCollector::getCurCount() {
    return CurCount;
}

int DataCollector::getCurIndex() {
    return CurIndex;
}

void DataCollector::convertUnits(float *EngUnits) {
    cbToEngUnits(BoardNum, Gain, ADData[CurIndex], EngUnits);
}

void DataCollector::stopOperation() {
//    The BACKGROUND operation must be explicitly stopped
//        Parameters:
//             BoardNum    :the number used by CB.CFG to describe this board
//             FunctionType: A/D operation (AIFUNCTION)
    ULStat = cbStopBackground (BoardNum, AIFUNCTION);

    cbWinBufFree(MemHandle);
}
