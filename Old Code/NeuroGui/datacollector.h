#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include "cbw.h"

class DataCollector {

    int BoardNum;// = 0;
    int ULStat;// = 0;
    int LowChan;// = 0;
    int HighChan;// = 0;
    int Gain;// = BIP5VOLTS;
    short Status;// = RUNNING;
    long CurCount;
    long CurIndex;
    long Count;// = 10000;
    long Rate;// = 390;
    HANDLE MemHandle;// = 0;
    WORD *ADData;
    unsigned Options;
    float revision;// = (float) CURRENTREVNUM;
    int ADRes;

public:
    DataCollector(int BoardNum, int LowChan, int HighChan, int Gain, long Count, long Rate);

    void startOperation();
    void updateStatus();
    int getCurCount();
    int getCurIndex();
    bool isRunning();
    void convertUnits(float *EngUnits);
    void stopOperation();
};

#endif // DATACOLLECTOR_H
