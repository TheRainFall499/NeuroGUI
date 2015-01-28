#include <qwt_sampling_thread.h>
#include "cbw.h"
#include "writedatathread.h"

class SamplingThread: public QwtSamplingThread
{
    Q_OBJECT

public:
    SamplingThread( QObject *parent = NULL );

    double frequency() const;
    double amplitude() const;
    void settriggerplot(bool inputvalue);
    void flush_data();
public Q_SLOTS:
    void setAmplitude( double );
    void setFrequency( double );

protected:
    virtual void sample( double elapsed );

private:
    virtual double value( double timeStamp ) const;

    double d_frequency;
    double d_amplitude;
    int StoredLocation;
    int LastChannel;
    bool FirstWrite;
    bool WroteSecond;
    bool WroteFirst;
    WriteDataThread datathread;
    bool PlotStarted;
    double starttime;
    bool StopWriting;
};
