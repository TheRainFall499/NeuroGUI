#include "plot.h"
#include "curvedata.h"
#include "signaldata.h"
#include "mainwindow.h"
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_directpainter.h>
#include <qwt_curve_fitter.h>
#include <qwt_painter.h>
#include <qevent.h>
#include <qlayout.h>
#include <qspinbox.h>

class Canvas: public QwtPlotCanvas
{
public:
    Canvas( QwtPlot *plot = NULL ):
        QwtPlotCanvas( plot )
    {
        // The backing store is important, when working with widget
        // overlays ( f.e rubberbands for zooming ).
        // Here we don't have them and the internal
        // backing store of QWidget is good enough.

        setPaintAttribute( QwtPlotCanvas::BackingStore, false );
        setBorderRadius( 1 );

        if ( QwtPainter::isX11GraphicsSystem() )
        {
#if QT_VERSION < 0x050000
            // Even if not liked by the Qt development, Qt::WA_PaintOutsidePaintEvent
            // works on X11. This has a nice effect on the performance.

            setAttribute( Qt::WA_PaintOutsidePaintEvent, true );
#endif

            // Disabling the backing store of Qt improves the performance
            // for the direct painter even more, but the canvas becomes
            // a native window of the window system, receiving paint events
            // for resize and expose operations. Those might be expensive
            // when there are many points and the backing store of
            // the canvas is disabled. So in this application
            // we better don't disable both backing stores.

            if ( testPaintAttribute( QwtPlotCanvas::BackingStore ) )
            {
                setAttribute( Qt::WA_PaintOnScreen, true );
                setAttribute( Qt::WA_NoSystemBackground, true );
            }
        }

        setupPalette();
    }

private:
    void setupPalette()
    {
        QPalette pal = palette();

#if QT_VERSION >= 0x040400
        QLinearGradient gradient;
        gradient.setCoordinateMode( QGradient::StretchToDeviceMode );
        gradient.setColorAt( 0.0, QColor( 0, 49, 110 ) );
        gradient.setColorAt( 1.0, QColor( 0, 87, 174 ) );

        pal.setBrush( QPalette::Window, QBrush( gradient ) );
#else
        pal.setBrush( QPalette::Window, QBrush( color ) );
#endif

        // QPalette::WindowText is used for the curve color
        pal.setColor( QPalette::WindowText, Qt::green );

        setPalette( pal );
    }
};

Plot::Plot( QWidget *parent ):
    QwtPlot( parent ),
    d_paintedPoints( 0 ),
    d_interval( 0.0, 1.0 ),
    d_timerId( -1 )
{
    d_directPainter = new QwtPlotDirectPainter();

    setAutoReplot( false );
    setCanvas( new Canvas() );

    plotLayout()->setAlignCanvasToScales( true );

    setAxisTitle( QwtPlot::xBottom, "Time [s]" );
    setAxisScale( QwtPlot::xBottom, d_interval.minValue(), d_interval.maxValue() );
    setAxisScale( QwtPlot::yLeft, -10, 10 );
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen( Qt::gray, 0.0, Qt::DotLine );
    grid->enableX( true );
    grid->enableXMin( true );
    grid->enableY( true );
    grid->enableYMin( false );
    grid->attach( this );
    d_origin = new QwtPlotMarker();
    d_origin->setLineStyle( QwtPlotMarker::Cross );
    d_origin->setValue( d_interval.minValue() + d_interval.width() / 2.0, 0.0 );
    d_origin->setLinePen( Qt::gray, 0.0, Qt::DashLine );
    d_origin->attach( this );

    extern int channels;

    if (channels >=1)
    {
        d_curve1 = new QwtPlotCurve();
        d_curve1->setStyle( QwtPlotCurve::Lines );
        d_curve1->setPen( canvas()->palette().color( QPalette::WindowText ) );
        d_curve1->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        d_curve1->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
        d_curve1->setData( new CurveData(1) );
        d_curve1->attach( this );
    }
    if (channels >=2)
    {
        d_curve2 = new QwtPlotCurve();
        d_curve2->setStyle( QwtPlotCurve::Lines );
        d_curve2->setPen( canvas()->palette().color( QPalette::WindowText ) );
        d_curve2->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        d_curve2->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
        d_curve2->setData( new CurveData(2) );
        d_curve2->attach( this );
    }
    if (channels >=3)
    {
        d_curve3 = new QwtPlotCurve();
        d_curve3->setStyle( QwtPlotCurve::Lines );
        d_curve3->setPen( canvas()->palette().color( QPalette::WindowText ) );
        d_curve3->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        d_curve3->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
        d_curve3->setData( new CurveData(3) );
        d_curve3->attach( this );
    }
    if (channels >=4)
    {
        d_curve4 = new QwtPlotCurve();
        d_curve4->setStyle( QwtPlotCurve::Lines );
        d_curve4->setPen( canvas()->palette().color( QPalette::WindowText ) );
        d_curve4->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        d_curve4->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
        d_curve4->setData( new CurveData(4) );
        d_curve4->attach( this );
    }
    if (channels >=5)
    {
        d_curve5 = new QwtPlotCurve();
        d_curve5->setStyle( QwtPlotCurve::Lines );
        d_curve5->setPen( canvas()->palette().color( QPalette::WindowText ) );
        d_curve5->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        d_curve5->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
        d_curve5->setData( new CurveData(5) );
        d_curve5->attach( this );
    }
    if (channels >=6)
    {
        d_curve6 = new QwtPlotCurve();
        d_curve6->setStyle( QwtPlotCurve::Lines );
        d_curve6->setPen( canvas()->palette().color( QPalette::WindowText ) );
        d_curve6->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        d_curve6->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
        d_curve6->setData( new CurveData(6) );
        d_curve6->attach( this );
    }
    if (channels >=7)
    {
        d_curve7 = new QwtPlotCurve();
        d_curve7->setStyle( QwtPlotCurve::Lines );
        d_curve7->setPen( canvas()->palette().color( QPalette::WindowText ) );
        d_curve7->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        d_curve7->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
        d_curve7->setData( new CurveData(7) );
        d_curve7->attach( this );
    }
    if (channels ==8)
    {
        d_curve8 = new QwtPlotCurve();
        d_curve8->setStyle( QwtPlotCurve::Lines );
        d_curve8->setPen( canvas()->palette().color( QPalette::WindowText ) );
        d_curve8->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        d_curve8->setPaintAttribute( QwtPlotCurve::ClipPolygons, false );
        d_curve8->setData( new CurveData(8) );
        d_curve8->attach( this );
    }

    QwtPlotZoomer* d_zoomer = new QwtPlotZoomer(canvas(),true);
    d_zoomer->setRubberBandPen(QPen(Qt::red, 2, Qt::DotLine));
    d_zoomer->setTrackerPen(QPen(Qt::red));
}

Plot::~Plot()
{
    extern int channels;
    if (channels >= 1)
        SignalData::instance(1).resetDataValues(channels);
    if (channels >= 2)
        SignalData::instance(2).resetDataValues(channels);
    if (channels >= 3)
        SignalData::instance(3).resetDataValues(channels);
    if (channels >= 4)
        SignalData::instance(4).resetDataValues(channels);
    if (channels >= 5)
        SignalData::instance(5).resetDataValues(channels);
    if (channels >= 6)
        SignalData::instance(6).resetDataValues(channels);
    if (channels >= 7)
        SignalData::instance(7).resetDataValues(channels);
    if (channels >= 8)
        SignalData::instance(8).resetDataValues(channels);
    delete d_directPainter;

}

void Plot::start()
{
    d_clock.start();
    d_timerId = startTimer( 0.2 );
}

void Plot::replot()
{

    extern int channels;
    if (channels >=1)
    {
        CurveData *data = static_cast<CurveData *>( d_curve1->data() );
        data->values().lock(1);
        QwtPlot::replot();
        d_paintedPoints = data->size();
        data->values().unlock(1);
    }
    if (channels >=2)
    {
        CurveData *data = static_cast<CurveData *>( d_curve2->data() );
        data->values().lock(2);
        QwtPlot::replot();
        d_paintedPoints = data->size();
        data->values().unlock(2);
    }
    if (channels >=3)
    {
        CurveData *data = static_cast<CurveData *>( d_curve3->data() );
        data->values().lock(3);
        QwtPlot::replot();
        d_paintedPoints = data->size();
        data->values().unlock(3);
    }
    if (channels >=4)
    {
        CurveData *data = static_cast<CurveData *>( d_curve4->data() );
        data->values().lock(4);
        QwtPlot::replot();
        d_paintedPoints = data->size();
        data->values().unlock(4);
    }
    if (channels >=5)
    {
        CurveData *data = static_cast<CurveData *>( d_curve5->data() );
        data->values().lock(5);
        QwtPlot::replot();
        d_paintedPoints = data->size();
        data->values().unlock(5);
    }
    if (channels >=6)
    {
        CurveData *data = static_cast<CurveData *>( d_curve6->data() );
        data->values().lock(6);
        QwtPlot::replot();
        d_paintedPoints = data->size();
        data->values().unlock(6);
    }
    if (channels >=7)
    {
        CurveData *data = static_cast<CurveData *>( d_curve7->data() );
        data->values().lock(7);
        QwtPlot::replot();
        d_paintedPoints = data->size();
        data->values().unlock(7);
    }
    if (channels ==8)
    {
        CurveData *data = static_cast<CurveData *>( d_curve8->data() );
        data->values().lock(8);
        QwtPlot::replot();
        d_paintedPoints = data->size();
        data->values().unlock(8);
    }
}

void Plot::setIntervalLength( double interval )
{
    if ( interval > 0.0 && interval != d_interval.width() )
    {
        d_interval.setMaxValue( d_interval.minValue() + interval );
        setAxisScale( QwtPlot::xBottom,
            d_interval.minValue(), d_interval.maxValue() );

        replot();
    }
}

void Plot::updateCurve()
{
    extern int channels;
    if (channels >=1)
    {
        CurveData *data1 = static_cast<CurveData *>( d_curve1->data() );
        data1->values().lock(1);
        const int numPoints = data1->size();
        if ( numPoints > d_paintedPoints )
        {
            const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
            if ( doClip )
            {
                /*
                    Depending on the platform setting a clip might be an important
                    performance issue. F.e. for Qt Embedded this reduces the
                    part of the backing store that has to be copied out - maybe
                    to an unaccelerated frame buffer device.
                */
                const QwtScaleMap xMap = canvasMap( d_curve1->xAxis() );
                const QwtScaleMap yMap = canvasMap( d_curve1->yAxis() );
                QRectF br = qwtBoundingRect( *data1,d_paintedPoints - 1, numPoints - 1 );
                const QRect clipRect = QwtScaleMap::transform( xMap, yMap, br ).toRect();
                d_directPainter->setClipRegion( clipRect );
            }
            d_directPainter->drawSeries( d_curve1, d_paintedPoints - 1, numPoints - 1 );
            //printf("Curve1:%d\n",d_paintedPoints);
            if (channels==1)
                d_paintedPoints = numPoints;


        }
        data1->values().unlock(1);
    }
    if (channels >=2)
    {
        CurveData *data = static_cast<CurveData *>( d_curve2->data() );
        data->values().lock(2);
        const int numPoints = data->size();
        if ( numPoints > d_paintedPoints )
        {
            const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
            if ( doClip )
            {
                /*
                    Depending on the platform setting a clip might be an important
                    performance issue. F.e. for Qt Embedded this reduces the
                    part of the backing store that has to be copied out - maybe
                    to an unaccelerated frame buffer device.
                */
                const QwtScaleMap xMap = canvasMap( d_curve2->xAxis() );
                const QwtScaleMap yMap = canvasMap( d_curve2->yAxis() );
                QRectF br = qwtBoundingRect( *data, d_paintedPoints - 1, numPoints - 1 );
                const QRect clipRect = QwtScaleMap::transform( xMap, yMap, br ).toRect();
                d_directPainter->setClipRegion( clipRect );
            }
            d_directPainter->drawSeries( d_curve2, d_paintedPoints - 1, numPoints - 1 );
            //printf("Curve2:%d\n",d_paintedPoints);
            if (channels==2)
            d_paintedPoints = numPoints;
        }
        data->values().unlock(2);
    }
    //replot();
    if (channels >=3)
    {
        CurveData *data = static_cast<CurveData *>( d_curve3->data() );
        data->values().lock(3);
        const int numPoints = data->size();
        if ( numPoints > d_paintedPoints )
        {
            const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
            if ( doClip )
            {
                /*
                    Depending on the platform setting a clip might be an important
                    performance issue. F.e. for Qt Embedded this reduces the
                    part of the backing store that has to be copied out - maybe
                    to an unaccelerated frame buffer device.
                */
                const QwtScaleMap xMap = canvasMap( d_curve3->xAxis() );
                const QwtScaleMap yMap = canvasMap( d_curve3->yAxis() );
                QRectF br = qwtBoundingRect( *data,
                    d_paintedPoints - 1, numPoints - 1 );
                const QRect clipRect = QwtScaleMap::transform( xMap, yMap, br ).toRect();
                d_directPainter->setClipRegion( clipRect );
            }
            d_directPainter->drawSeries( d_curve3,
                d_paintedPoints - 1, numPoints - 1 );
            if (channels==3)
                d_paintedPoints = numPoints;
        }
        data->values().unlock(3);
    }
    if (channels >=4)
    {
        CurveData *data = static_cast<CurveData *>( d_curve4->data() );
        data->values().lock(4);
        const int numPoints = data->size();
        if ( numPoints > d_paintedPoints )
        {
            const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
            if ( doClip )
            {
                /*
                    Depending on the platform setting a clip might be an important
                    performance issue. F.e. for Qt Embedded this reduces the
                    part of the backing store that has to be copied out - maybe
                    to an unaccelerated frame buffer device.
                */
                const QwtScaleMap xMap = canvasMap( d_curve4->xAxis() );
                const QwtScaleMap yMap = canvasMap( d_curve4->yAxis() );
                QRectF br = qwtBoundingRect( *data,
                    d_paintedPoints - 1, numPoints - 1 );
                const QRect clipRect = QwtScaleMap::transform( xMap, yMap, br ).toRect();
                d_directPainter->setClipRegion( clipRect );
            }
            d_directPainter->drawSeries( d_curve4,
                d_paintedPoints - 1, numPoints - 1 );
            if (channels==4)
            d_paintedPoints = numPoints;
        }
        data->values().unlock(4);
    }
    if (channels >=5)
    {
        CurveData *data = static_cast<CurveData *>( d_curve5->data() );
        data->values().lock(5);
        const int numPoints = data->size();
        if ( numPoints > d_paintedPoints )
        {
            const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
            if ( doClip )
            {
                /*
                    Depending on the platform setting a clip might be an important
                    performance issue. F.e. for Qt Embedded this reduces the
                    part of the backing store that has to be copied out - maybe
                    to an unaccelerated frame buffer device.
                */
                const QwtScaleMap xMap = canvasMap( d_curve5->xAxis() );
                const QwtScaleMap yMap = canvasMap( d_curve5->yAxis() );
                QRectF br = qwtBoundingRect( *data,
                    d_paintedPoints - 1, numPoints - 1 );
                const QRect clipRect = QwtScaleMap::transform( xMap, yMap, br ).toRect();
                d_directPainter->setClipRegion( clipRect );
            }
            d_directPainter->drawSeries( d_curve5,
                d_paintedPoints - 1, numPoints - 1 );
            if (channels==5)
                d_paintedPoints = numPoints;
        }
        data->values().unlock(5);
    }
    if (channels >=6)
    {
        CurveData *data = static_cast<CurveData *>( d_curve6->data() );
        data->values().lock(6);
        const int numPoints = data->size();
        if ( numPoints > d_paintedPoints )
        {
            const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
            if ( doClip )
            {
                /*
                    Depending on the platform setting a clip might be an important
                    performance issue. F.e. for Qt Embedded this reduces the
                    part of the backing store that has to be copied out - maybe
                    to an unaccelerated frame buffer device.
                */
                const QwtScaleMap xMap = canvasMap( d_curve6->xAxis() );
                const QwtScaleMap yMap = canvasMap( d_curve6->yAxis() );
                QRectF br = qwtBoundingRect( *data,
                    d_paintedPoints - 1, numPoints - 1 );
                const QRect clipRect = QwtScaleMap::transform( xMap, yMap, br ).toRect();
                d_directPainter->setClipRegion( clipRect );
            }
            d_directPainter->drawSeries( d_curve6,
                d_paintedPoints - 1, numPoints - 1 );
            if (channels==6)
                d_paintedPoints = numPoints;
        }
        data->values().unlock(6);
    }
    if (channels >=7)
    {
        CurveData *data = static_cast<CurveData *>( d_curve7->data() );
        data->values().lock(7);
        const int numPoints = data->size();
        if ( numPoints > d_paintedPoints )
        {
            const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
            if ( doClip )
            {
                /*
                    Depending on the platform setting a clip might be an important
                    performance issue. F.e. for Qt Embedded this reduces the
                    part of the backing store that has to be copied out - maybe
                    to an unaccelerated frame buffer device.
                */
                const QwtScaleMap xMap = canvasMap( d_curve7->xAxis() );
                const QwtScaleMap yMap = canvasMap( d_curve7->yAxis() );
                QRectF br = qwtBoundingRect( *data,
                    d_paintedPoints - 1, numPoints - 1 );
                const QRect clipRect = QwtScaleMap::transform( xMap, yMap, br ).toRect();
                d_directPainter->setClipRegion( clipRect );
            }
            d_directPainter->drawSeries( d_curve7,
                d_paintedPoints - 1, numPoints - 1 );
            if (channels==7)
                d_paintedPoints = numPoints;
        }
        data->values().unlock(7);
    }
    if (channels ==8)
    {
        CurveData *data = static_cast<CurveData *>( d_curve8->data() );
        data->values().lock(8);
        const int numPoints = data->size();
        if ( numPoints > d_paintedPoints )
        {
            const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
            if ( doClip )
            {
                /*
                    Depending on the platform setting a clip might be an important
                    performance issue. F.e. for Qt Embedded this reduces the
                    part of the backing store that has to be copied out - maybe
                    to an unaccelerated frame buffer device.
                */
                const QwtScaleMap xMap = canvasMap( d_curve8->xAxis() );
                const QwtScaleMap yMap = canvasMap( d_curve8->yAxis() );
                QRectF br = qwtBoundingRect( *data,
                    d_paintedPoints - 1, numPoints - 1 );
                const QRect clipRect = QwtScaleMap::transform( xMap, yMap, br ).toRect();
                d_directPainter->setClipRegion( clipRect );
            }
            d_directPainter->drawSeries( d_curve8,
                d_paintedPoints - 1, numPoints - 1 );
            if (channels==8)
                d_paintedPoints = numPoints;
        }
        data->values().unlock(8);
    }

}

void Plot::incrementInterval()
{
    d_interval = QwtInterval( d_interval.maxValue(),
        d_interval.maxValue() + d_interval.width() );
extern int channels;
if (channels >= 1)
{
    CurveData *data = static_cast<CurveData *>( d_curve1->data() );
    data->values().clearStaleValues( d_interval.minValue() );
}
if (channels >= 2)
{
    CurveData *data = static_cast<CurveData *>( d_curve2->data() );
    data->values().clearStaleValues( d_interval.minValue() );
}
if (channels >= 3)
{
    CurveData *data = static_cast<CurveData *>( d_curve3->data() );
    data->values().clearStaleValues( d_interval.minValue() );
}
if (channels == 4)
{
    CurveData *data = static_cast<CurveData *>( d_curve4->data() );
    data->values().clearStaleValues( d_interval.minValue() );
}
if (channels == 5)
{
    CurveData *data = static_cast<CurveData *>( d_curve5->data() );
    data->values().clearStaleValues( d_interval.minValue() );
}
if (channels == 6)
{
    CurveData *data = static_cast<CurveData *>( d_curve6->data() );
    data->values().clearStaleValues( d_interval.minValue() );
}
if (channels == 7)
{
    CurveData *data = static_cast<CurveData *>( d_curve7->data() );
    data->values().clearStaleValues( d_interval.minValue() );
}
if (channels == 8)
{
    CurveData *data = static_cast<CurveData *>( d_curve8->data() );
    data->values().clearStaleValues( d_interval.minValue() );
}


    // To avoid, that the grid is jumping, we disable
    // the autocalculation of the ticks and shift them
    // manually instead.

    QwtScaleDiv scaleDiv = axisScaleDiv( QwtPlot::xBottom );
    scaleDiv.setInterval( d_interval );

    for ( int i = 0; i < QwtScaleDiv::NTickTypes; i++ )
    {
        QList<double> ticks = scaleDiv.ticks( i );
        for ( int j = 0; j < ticks.size(); j++ )
            ticks[j] += d_interval.width();
        scaleDiv.setTicks( i, ticks );
    }
    setAxisScaleDiv( QwtPlot::xBottom, scaleDiv );

    d_origin->setValue( d_interval.minValue() + d_interval.width() / 2.0, 0.0 );

    d_paintedPoints = 0;
    replot();
}

void Plot::timerEvent( QTimerEvent *event )
{
    //qDebug() << "PlotFired";
    if ( event->timerId() == d_timerId )
    {
        updateCurve();

        const double elapsed = d_clock.elapsed() / 1000.0;
        if ( elapsed > d_interval.maxValue() )
            incrementInterval();

        return;
    }

    QwtPlot::timerEvent( event );
}

void Plot::resizeEvent( QResizeEvent *event )
{
    d_directPainter->reset();
    QwtPlot::resizeEvent( event );
}

void Plot::showEvent( QShowEvent * )
{
    replot();
}

bool Plot::eventFilter( QObject *object, QEvent *event )
{
    extern int channels;
    if ( object == canvas() && 
        event->type() == QEvent::PaletteChange )
    {
        if (channels>=1)
        d_curve1->setPen( canvas()->palette().color( QPalette::WindowText ) );
        if (channels>=2)
        d_curve2->setPen( canvas()->palette().color( QPalette::WindowText ) );
        if (channels>=3)
        d_curve3->setPen( canvas()->palette().color( QPalette::WindowText ) );
        if (channels>=4)
        d_curve4->setPen( canvas()->palette().color( QPalette::WindowText ) );
        if (channels>=5)
        d_curve5->setPen( canvas()->palette().color( QPalette::WindowText ) );
        if (channels>=6)
        d_curve6->setPen( canvas()->palette().color( QPalette::WindowText ) );
        if (channels>=7)
        d_curve7->setPen( canvas()->palette().color( QPalette::WindowText ) );
        if (channels>=8)
        d_curve8->setPen( canvas()->palette().color( QPalette::WindowText ) );

    }

    return QwtPlot::eventFilter( object, event );
}
