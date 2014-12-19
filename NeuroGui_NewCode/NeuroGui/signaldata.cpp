#include "signaldata.h"
#include <qvector.h>
#include <qmutex.h>
#include <qreadwritelock.h>
#include "mainwindow.h"

class SignalData::PrivateData
{
public:
    PrivateData():
        boundingRect( 1.0, 1.0, -2.0, -2.0 ) // invalid
    {
        values.reserve( 1000 );
    }

    inline void append( const QPointF &sample )
    {
        values.append( sample );

        // adjust the bounding rectangle

        if ( boundingRect.width() < 0 || boundingRect.height() < 0 )
        {
            boundingRect.setRect( sample.x(), sample.y(), 0.0, 0.0 );
        }
        else
        {
            boundingRect.setRight( sample.x() );

            if ( sample.y() > boundingRect.bottom() )
                boundingRect.setBottom( sample.y() );

            if ( sample.y() < boundingRect.top() )
                boundingRect.setTop( sample.y() );
        }
    }

    QReadWriteLock lock;

    QVector<QPointF> values;
    QRectF boundingRect;

    QMutex mutex; // protecting pendingValues
    QVector<QPointF> pendingValues;
};

SignalData::SignalData()
{
    d_data1 = new PrivateData();
    d_data2 = new PrivateData();
    d_data3 = new PrivateData();
    d_data4 = new PrivateData();
    d_data5 = new PrivateData();
    d_data6 = new PrivateData();
    d_data7 = new PrivateData();
    d_data8 = new PrivateData();
}

SignalData::~SignalData()
{
    delete d_data1;
    delete d_data2;
    delete d_data3;
    delete d_data4;
    delete d_data5;
    delete d_data6;
    delete d_data7;
    delete d_data8;

}

int SignalData::size(int channel) const
{

    switch (channel)
    {
      case 1:
        return d_data1->values.size();
        break;
      case 2:
        return d_data2->values.size();
        break;
      case 3:
        return d_data3->values.size();
        break;
      case 4:
        return d_data4->values.size();
        break;
      case 5:
        return d_data5->values.size();
        break;
      case 6:
        return d_data6->values.size();
        break;
      case 7:
        return d_data7->values.size();
        break;
      case 8:
        return d_data8->values.size();
        break;
      default:
        return d_data1->values.size();
        break;
      }
}

QPointF SignalData::value( int index,int channel ) const
{

    switch (channel)
    {
      case 1:
        return d_data1->values[index];
        break;
      case 2:
        return d_data2->values[index];
        break;
      case 3:
        return d_data3->values[index];
        break;
      case 4:
        return d_data4->values[index];
        break;
      case 5:
        return d_data5->values[index];
        break;
      case 6:
        return d_data6->values[index];
        break;
      case 7:
        return d_data7->values[index];
        break;
      case 8:
        return d_data8->values[index];
        break;
      default:
        return d_data1->values[index];
        break;
      }
}

QRectF SignalData::boundingRect(int channel) const
{

    switch (channel)
    {
      case 1:
        return d_data1->boundingRect;
        break;
      case 2:
        return d_data2->boundingRect;
        break;
      case 3:
        return d_data3->boundingRect;
        break;
      case 4:
        return d_data4->boundingRect;
        break;
      case 5:
        return d_data5->boundingRect;
        break;
      case 6:
        return d_data6->boundingRect;
        break;
      case 7:
        return d_data7->boundingRect;
        break;
      case 8:
        return d_data8->boundingRect;
        break;
      default:
        return d_data1->boundingRect;
        break;
      }
}

void SignalData::lock(int channel)
{
    switch (channel)
    {
      case 1:
        d_data1->lock.lockForRead();
        break;
      case 2:
        d_data2->lock.lockForRead();
        break;
      case 3:
        d_data3->lock.lockForRead();
        break;
      case 4:
        d_data4->lock.lockForRead();
        break;
      case 5:
        d_data5->lock.lockForRead();
        break;
      case 6:
        d_data6->lock.lockForRead();
        break;
      case 7:
        d_data7->lock.lockForRead();
        break;
      case 8:
        d_data8->lock.lockForRead();
        break;
      default:
        d_data1->lock.lockForRead();
        break;
      }
}

void SignalData::unlock(int channel)
{

    switch (channel)
    {
      case 1:
        d_data1->lock.unlock();
        break;
      case 2:
        d_data2->lock.unlock();
        break;
      case 3:
        d_data3->lock.unlock();
        break;
      case 4:
        d_data4->lock.unlock();
        break;
      case 5:
        d_data5->lock.unlock();
        break;
      case 6:
        d_data6->lock.unlock();
        break;
      case 7:
        d_data7->lock.unlock();
        break;
      case 8:
        d_data8->lock.unlock();
        break;
      default:
        d_data1->lock.unlock();
        break;
      }
}

void SignalData::append( const QPointF &sample , int channel)
{   
    switch (channel)
    {
      case 1:
       {
            d_data1->mutex.lock();
            d_data1->pendingValues += sample;
            const bool isLocked = d_data1->lock.tryLockForWrite();
            if ( isLocked )
            {
                const int numValues = d_data1->pendingValues.size();
                const QPointF *pendingValues = d_data1->pendingValues.data();

                for ( int i = 0; i < numValues; i++ )
                    d_data1->append( pendingValues[i] );

                d_data1->pendingValues.clear();

                d_data1->lock.unlock();
            }
            d_data1->mutex.unlock();
       }
       break;
      case 2:
        {
             d_data2->mutex.lock();
             d_data2->pendingValues += sample;
             const bool isLocked = d_data2->lock.tryLockForWrite();
             if ( isLocked )
             {
                 const int numValues = d_data2->pendingValues.size();
                 const QPointF *pendingValues = d_data2->pendingValues.data();

                 for ( int i = 0; i < numValues; i++ )
                     d_data2->append( pendingValues[i] );

                 d_data2->pendingValues.clear();

                 d_data2->lock.unlock();
             }
             d_data2->mutex.unlock();
        }
        break;
      case 3:
        {
             d_data3->mutex.lock();
             d_data3->pendingValues += sample;
             const bool isLocked = d_data3->lock.tryLockForWrite();
             if ( isLocked )
             {
                 const int numValues = d_data3->pendingValues.size();
                 const QPointF *pendingValues = d_data3->pendingValues.data();

                 for ( int i = 0; i < numValues; i++ )
                     d_data3->append( pendingValues[i] );

                 d_data3->pendingValues.clear();

                 d_data3->lock.unlock();
             }
             d_data3->mutex.unlock();
        }
        break;
      case 4:
        {
             d_data4->mutex.lock();
             d_data4->pendingValues += sample;
             const bool isLocked = d_data4->lock.tryLockForWrite();
             if ( isLocked )
             {
                 const int numValues = d_data4->pendingValues.size();
                 const QPointF *pendingValues = d_data4->pendingValues.data();

                 for ( int i = 0; i < numValues; i++ )
                     d_data4->append( pendingValues[i] );

                 d_data4->pendingValues.clear();

                 d_data4->lock.unlock();
             }
             d_data4->mutex.unlock();
        }
        break;
      case 5:
        {
             d_data5->mutex.lock();
             d_data5->pendingValues += sample;
             const bool isLocked = d_data5->lock.tryLockForWrite();
             if ( isLocked )
             {
                 const int numValues = d_data5->pendingValues.size();
                 const QPointF *pendingValues = d_data5->pendingValues.data();

                 for ( int i = 0; i < numValues; i++ )
                     d_data5->append( pendingValues[i] );

                 d_data5->pendingValues.clear();

                 d_data5->lock.unlock();
             }
             d_data5->mutex.unlock();
        }
        break;
      case 6:
        {
             d_data6->mutex.lock();
             d_data6->pendingValues += sample;
             const bool isLocked = d_data6->lock.tryLockForWrite();
             if ( isLocked )
             {
                 const int numValues = d_data6->pendingValues.size();
                 const QPointF *pendingValues = d_data6->pendingValues.data();

                 for ( int i = 0; i < numValues; i++ )
                     d_data6->append( pendingValues[i] );

                 d_data6->pendingValues.clear();

                 d_data6->lock.unlock();
             }
             d_data6->mutex.unlock();
        }
        break;
      case 7:
        {
             d_data7->mutex.lock();
             d_data7->pendingValues += sample;
             const bool isLocked = d_data7->lock.tryLockForWrite();
             if ( isLocked )
             {
                 const int numValues = d_data7->pendingValues.size();
                 const QPointF *pendingValues = d_data7->pendingValues.data();

                 for ( int i = 0; i < numValues; i++ )
                     d_data7->append( pendingValues[i] );

                 d_data7->pendingValues.clear();

                 d_data7->lock.unlock();
             }
             d_data7->mutex.unlock();
        }
        break;
      case 8:
        {
             d_data8->mutex.lock();
             d_data8->pendingValues += sample;
             const bool isLocked = d_data8->lock.tryLockForWrite();
             if ( isLocked )
             {
                 const int numValues = d_data8->pendingValues.size();
                 const QPointF *pendingValues = d_data8->pendingValues.data();

                 for ( int i = 0; i < numValues; i++ )
                     d_data8->append( pendingValues[i] );

                 d_data8->pendingValues.clear();

                 d_data8->lock.unlock();
             }
             d_data8->mutex.unlock();
        }
        break;
      default:
        {
             d_data1->mutex.lock();
             d_data1->pendingValues += sample;
             const bool isLocked = d_data1->lock.tryLockForWrite();
             if ( isLocked )
             {
                 const int numValues = d_data1->pendingValues.size();
                 const QPointF *pendingValues = d_data1->pendingValues.data();

                 for ( int i = 0; i < numValues; i++ )
                     d_data1->append( pendingValues[i] );

                 d_data1->pendingValues.clear();

                 d_data1->lock.unlock();
             }
             d_data1->mutex.unlock();
        }
        break;
      }


}

void SignalData::clearStaleValues( double limit )
{
    extern int channels;
    if (channels >= 1)
    {
        d_data1->lock.lockForWrite();
        d_data1->boundingRect = QRectF( 1.0, 1.0, -2.0, -2.0 ); // invalid
        const QVector<QPointF> values = d_data1->values;
        d_data1->values.clear();
        d_data1->values.reserve( values.size() );
        int index;
        for ( index = values.size() - 1; index >= 0; index-- )
        {
            if ( values[index].x() < limit )
                break;
        }
        if ( index > 0 )
            d_data1->append( values[index++] );
        while ( index < values.size() - 1 )
            d_data1->append( values[index++] );
        d_data1->lock.unlock();
    }
    if (channels >=2)
    {
        d_data2->lock.lockForWrite();
        d_data2->boundingRect = QRectF( 1.0, 1.0, -2.0, -2.0 ); // invalid
        const QVector<QPointF> values = d_data2->values;
        d_data2->values.clear();
        d_data2->values.reserve( values.size() );
        int index;
        for ( index = values.size() - 1; index >= 0; index-- )
        {
            if ( values[index].x() < limit )
                break;
        }
        if ( index > 0 )
            d_data2->append( values[index++] );
        while ( index < values.size() - 1 )
            d_data2->append( values[index++] );
        d_data2->lock.unlock();
    }
    if (channels >= 3)
    {
        d_data3->lock.lockForWrite();
        d_data3->boundingRect = QRectF( 1.0, 1.0, -2.0, -2.0 ); // invalid
        const QVector<QPointF> values = d_data3->values;
        d_data3->values.clear();
        d_data3->values.reserve( values.size() );
        int index;
        for ( index = values.size() - 1; index >= 0; index-- )
        {
            if ( values[index].x() < limit )
                break;
        }
        if ( index > 0 )
            d_data3->append( values[index++] );
        while ( index < values.size() - 1 )
            d_data3->append( values[index++] );
        d_data3->lock.unlock();
    }
    if (channels >= 4)
    {
        d_data4->lock.lockForWrite();
        d_data4->boundingRect = QRectF( 1.0, 1.0, -2.0, -2.0 ); // invalid
        const QVector<QPointF> values = d_data4->values;
        d_data4->values.clear();
        d_data4->values.reserve( values.size() );
        int index;
        for ( index = values.size() - 1; index >= 0; index-- )
        {
            if ( values[index].x() < limit )
                break;
        }
        if ( index > 0 )
            d_data4->append( values[index++] );
        while ( index < values.size() - 1 )
            d_data4->append( values[index++] );
        d_data4->lock.unlock();
    }
    if (channels >= 5)
    {
        d_data5->lock.lockForWrite();
        d_data5->boundingRect = QRectF( 1.0, 1.0, -2.0, -2.0 ); // invalid
        const QVector<QPointF> values = d_data5->values;
        d_data5->values.clear();
        d_data5->values.reserve( values.size() );
        int index;
        for ( index = values.size() - 1; index >= 0; index-- )
        {
            if ( values[index].x() < limit )
                break;
        }
        if ( index > 0 )
            d_data5->append( values[index++] );
        while ( index < values.size() - 1 )
            d_data5->append( values[index++] );
        d_data5->lock.unlock();
    }
    if (channels >= 6)
    {
        d_data6->lock.lockForWrite();
        d_data6->boundingRect = QRectF( 1.0, 1.0, -2.0, -2.0 ); // invalid
        const QVector<QPointF> values = d_data6->values;
        d_data6->values.clear();
        d_data6->values.reserve( values.size() );
        int index;
        for ( index = values.size() - 1; index >= 0; index-- )
        {
            if ( values[index].x() < limit )
                break;
        }
        if ( index > 0 )
            d_data6->append( values[index++] );
        while ( index < values.size() - 1 )
            d_data6->append( values[index++] );
        d_data6->lock.unlock();
    }
    if (channels >= 7)
    {
        d_data7->lock.lockForWrite();
        d_data7->boundingRect = QRectF( 1.0, 1.0, -2.0, -2.0 ); // invalid
        const QVector<QPointF> values = d_data7->values;
        d_data7->values.clear();
        d_data7->values.reserve( values.size() );
        int index;
        for ( index = values.size() - 1; index >= 0; index-- )
        {
            if ( values[index].x() < limit )
                break;
        }
        if ( index > 0 )
            d_data7->append( values[index++] );
        while ( index < values.size() - 1 )
            d_data7->append( values[index++] );
        d_data7->lock.unlock();
    }
    if (channels == 8)
    {
        d_data8->lock.lockForWrite();
        d_data8->boundingRect = QRectF( 1.0, 1.0, -2.0, -2.0 ); // invalid
        const QVector<QPointF> values = d_data8->values;
        d_data8->values.clear();
        d_data8->values.reserve( values.size() );
        int index;
        for ( index = values.size() - 1; index >= 0; index-- )
        {
            if ( values[index].x() < limit )
                break;
        }
        if ( index > 0 )
            d_data8->append( values[index++] );
        while ( index < values.size() - 1 )
            d_data8->append( values[index++] );
        d_data8->lock.unlock();
    }
}

SignalData &SignalData::instance(int channel)
{
    switch (channel)
    {
      case 1:
       {
        static SignalData valueVector1;
        return valueVector1;
       }
       break;
      case 2:
        {
        static SignalData valueVector2;
        return valueVector2;
        }
        break;
      case 3:
        {
        static SignalData valueVector3;
        return valueVector3;
        }
        break;
      case 4:
        {
        static SignalData valueVector4;
        return valueVector4;
        }
        break;
      case 5:
        {
        static SignalData valueVector5;
        return valueVector5;
        }
        break;
      case 6:
        {
        static SignalData valueVector6;
        return valueVector6;
        }
        break;
      case 7:
        {
        static SignalData valueVector7;
        return valueVector7;
        }
        break;
      case 8:
        {
        static SignalData valueVector8;
        return valueVector8;
        }
        break;
      default:
        {
        static SignalData valueVector1;
        return valueVector1;
        }
        break;
      }
}
