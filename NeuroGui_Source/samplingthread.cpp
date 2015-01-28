#include "samplingthread.h"
#include "signaldata.h"
#include <qwt_math.h>
#include <math.h>
#include "cbw.h"
#include "mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <writedatathread.h>

#if QT_VERSION < 0x040600
#define qFastSin(x) ::sin(x)
#endif

SamplingThread::SamplingThread( QObject *parent ):
    QwtSamplingThread( parent ),
    d_frequency( 5.0 ),
    d_amplitude( 20.0 )
{
    StoredLocation=0;
    LastChannel=0;
    FirstWrite=1;
    WroteFirst=0;
    WroteSecond=0;
    PlotStarted=false;
    StopWriting=false;
}

void SamplingThread::setFrequency( double frequency )
{
    d_frequency = frequency;
}


double SamplingThread::frequency() const
{
    return d_frequency;
}

void SamplingThread::settriggerplot(bool inputvalue)
{
    PlotStarted=inputvalue;
}

void SamplingThread::setAmplitude( double amplitude )
{
    d_amplitude = amplitude;
}

double SamplingThread::amplitude() const
{
    return d_amplitude;
}

void SamplingThread::flush_data()
{
    StopWriting=true;
    PlotStarted=false;
    qDebug() << "Buffer Flushed";
}

void SamplingThread::sample( double elapsed )
{
//    const QPointF s( elapsed, 1 );
//    SignalData::instance().append( s );
    long CurCount = 10;
    long CurIndex;
    short Status = RUNNING;
    int BoardNum = 0;
    float v;
    extern WORD *ADData;
    extern int channels;
    extern HANDLE MemHandle;
    extern bool triggerchecked;
    extern bool StartPlot;
    extern int globalrecfreq;
    int rounder=globalrecfreq*.05;

    int ULStat = cbGetStatus (BoardNum, &Status, &CurCount, &CurIndex,AIFUNCTION);
    //qDebug() << "triggerchecked:" << triggerchecked;
    //qDebug() << "PlotStarted:" << PlotStarted;
    //qDebug() << "CurCount:" << CurCount;
    //qDebug() << "Firstwrite:" << FirstWrite;
    if (CurCount < rounder)
        PlotStarted = false;

    qDebug() << CurCount;
    qDebug() << PlotStarted;
    if ((CurCount>(rounder)) && (triggerchecked==true) && PlotStarted==false)
    {
        qDebug() << "StartPlotting";
        //qDebug() << CurCount;
        StartPlot=true;
        PlotStarted=true;
        starttime=elapsed;
    }
    else if (CurCount > rounder && PlotStarted == false)
    {
        PlotStarted=true;
    }
    else
    {
    }

    if ((CurCount == 0) && triggerchecked==false)
        starttime=elapsed;
    if ((Status == RUNNING) && CurCount > 20 )
    {
        for (int chantrack=1; (chantrack <= channels); chantrack++)
        {
            elapsed = elapsed-starttime;
            int ENG = cbToEngUnits(BoardNum,BIP10VOLTS,ADData[CurIndex+chantrack-1],&v);
            double v_double=(double)v;
            const QPointF s( elapsed, v_double );
            //printf("SendingPlotDataChan:%d\n",chantrack);
            SignalData::instance(chantrack).append( s, chantrack );
        }
    }
    extern int globalrecfreq;
    if (Status==RUNNING && PlotStarted==true)
    {
        if (CurIndex < (channels*globalrecfreq))
        {
            if (WroteFirst != false)
                WroteFirst = false;
            if (FirstWrite == true)
                WroteSecond = true;
            if (WroteSecond==false)
            {
                qDebug() << "writing 2";
                datathread.run(2,StopWriting);
                WroteSecond=true;
            }
        }
        else
        {
            if (FirstWrite != false)
                FirstWrite=false;
            if (WroteSecond != false)
                WroteSecond = false;
            if (WroteFirst == false)
            {
                qDebug() << "writing 1";
                datathread.run(1,StopWriting);

                WroteFirst=true;
            }

        }
    }
    //qDebug() << StopWriting;
    if (StopWriting==true)
    {
        this->stop();
        StopWriting=false;
        FirstWrite=true;
    }
}

double SamplingThread::value( double timeStamp ) const
{
    const double period = 1.0 / d_frequency;

    const double x = ::fmod( timeStamp, period );
    const double v = d_amplitude * qFastSin( x / period * 2 * M_PI );

    return v;
}
