#include "curvedata.h"
#include "signaldata.h"

CurveData::CurveData(int channel)
{
    this->channel=channel;
}

const SignalData &CurveData::values() const
{
    return SignalData::instance(channel);
}

SignalData &CurveData::values()
{
    return SignalData::instance(channel);
}

QPointF CurveData::sample( size_t i ) const
{
    return SignalData::instance(channel).value( i , channel);
}

size_t CurveData::size() const
{
    return SignalData::instance(channel).size(channel);
}

QRectF CurveData::boundingRect() const
{
    return SignalData::instance(channel).boundingRect(channel);
}
