#ifndef _SIGNAL_DATA_H_
#define _SIGNAL_DATA_H_ 1

#include <qrect.h>
#include <qwt_series_data.h>
#include <qpointer.h>

class SignalData
{
public:
    static SignalData &instance(int chan);

    void append( const QPointF &pos,int chan );
    void clearStaleValues( double min );
    void resetDataValues(int chans);

    int size(int chan) const;
    QPointF value( int index , int chan) const;

    QRectF boundingRect(int chan) const;

    void lock(int chan);
    void unlock(int chan);

private:
    SignalData();
    SignalData( const SignalData & );
    SignalData &operator=( const SignalData & );

    virtual ~SignalData();

    class PrivateData;
    PrivateData *d_data1;
    PrivateData *d_data2;
    PrivateData *d_data3;
    PrivateData *d_data4;
    PrivateData *d_data5;
    PrivateData *d_data6;
    PrivateData *d_data7;
    PrivateData *d_data8;
};

#endif
