#include "samplingthread.h"
#include "signaldata.h"
#include <qwt_math.h>
#include <math.h>
#include "cbw.h"

#if QT_VERSION < 0x040600
#define qFastSin(x) ::sin(x)
#endif

SamplingThread::SamplingThread( QObject *parent ):
    QwtSamplingThread( parent ),
    d_frequency( 5.0 ),
    d_amplitude( 20.0 )
{
}

void SamplingThread::setFrequency( double frequency )
{
    d_frequency = frequency;
}

void SamplingThread::setADData( ushort *ADDataInput )
{
    *ADData = *ADDataInput;
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
    long CurCount;
    long CurIndex;
    short Status;
    int BoardNum = 0;
    int ULStat = cbGetStatus (BoardNum, &Status, &CurCount, &CurIndex,AIFUNCTION);

    if ((Status == RUNNING) && CurCount > 0 )
    {
        const QPointF s( elapsed, ADData[CurIndex] );
        SignalData::instance().append( s );
    }
}

double SamplingThread::value( double timeStamp ) const
{
    const double period = 1.0 / d_frequency;

    const double x = ::fmod( timeStamp, period );
    const double v = d_amplitude * qFastSin( x / period * 2 * M_PI );

    return v;
}
