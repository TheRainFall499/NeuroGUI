#include <qwt_plot.h>
#include <qwt_interval.h>
#include <qwt_system_clock.h>

class QwtPlotCurve;
class QwtPlotMarker;
class QwtPlotDirectPainter;

class Plot: public QwtPlot
{
    Q_OBJECT

public:
    Plot( QWidget * = NULL );
    virtual ~Plot();

    void start();
    virtual void replot();

    virtual bool eventFilter( QObject *, QEvent * );

    void resetplot();

public Q_SLOTS:
    void setIntervalLength( double );

protected:
    virtual void showEvent( QShowEvent * );
    virtual void resizeEvent( QResizeEvent * );
    virtual void timerEvent( QTimerEvent * );

private:
    void updateCurve();
    void incrementInterval();
    void customizeCurve(QwtPlotCurve *d_curve, int curveNum);
    void updateCurveData(QwtPlotCurve *d_curve, int chanNum);
    void replotCurveData(QwtPlotCurve *d_curve, int chanNum);

    QwtPlotMarker *d_origin;
    QwtPlotCurve *d_curve1;
    QwtPlotCurve *d_curve2;
    QwtPlotCurve *d_curve3;
    QwtPlotCurve *d_curve4;
    QwtPlotCurve *d_curve5;
    QwtPlotCurve *d_curve6;
    QwtPlotCurve *d_curve7;
    QwtPlotCurve *d_curve8;
    int d_paintedPoints;

    QwtPlotDirectPainter *d_directPainter;

    QwtInterval d_interval;
    int d_timerId;

    QwtSystemClock d_clock;
};
