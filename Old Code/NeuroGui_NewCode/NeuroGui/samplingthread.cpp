#include "samplingthread.h"
#include "signaldata.h"
#include <qwt_math.h>
#include <math.h>
#include "cbw.h"
#include "mainwindow.h"

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
}

void SamplingThread::setFrequency( double frequency )
{
    d_frequency = frequency;
}


double SamplingThread::frequency() const
{
    return d_frequency;
}

void SamplingThread::setAmplitude( double amplitude )
{
    d_amplitude = amplitude;
}

double SamplingThread::amplitude() const
{
    return d_amplitude;
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

    int ULStat = cbGetStatus (BoardNum, &Status, &CurCount, &CurIndex,AIFUNCTION);

    if ((Status == RUNNING) && CurCount > 20 )
    {
        for (int chantrack=1; (chantrack <= channels); chantrack++)
        {
            int ENG = cbToEngUnits(BoardNum,BIP10VOLTS,ADData[CurIndex+chantrack-1],&v);
            double v_double=(double)v;
            const QPointF s( elapsed, v_double );
            //printf("SendingPlotDataChan:%d\n",chantrack);
            SignalData::instance(chantrack).append( s, chantrack );
        }
    }
}

double SamplingThread::value( double timeStamp ) const
{
    const double period = 1.0 / d_frequency;

    const double x = ::fmod( timeStamp, period );
    const double v = d_amplitude * qFastSin( x / period * 2 * M_PI );

    return v;
}
