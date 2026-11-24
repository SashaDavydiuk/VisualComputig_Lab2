#include <QPainter>
#include <QPainterPath>
#include <cmath>
#include "canvas.h"
#include <QMouseEvent>

Canvas::Canvas(QWidget *parent)
	: QFrame(parent),
      wMin(0, 0), wMax(1, 1), function(0)
{
	setFrameStyle(QFrame::Box);
}

Canvas::~Canvas()
{
}

QSize Canvas::minimumSizeHint() const
{
	return QSize(200, 200);
}

QSize Canvas::sizeHint() const
{
	return QSize(640, 480);
}

void Canvas::setWorld(const QPointF &min, const QPointF &max)
{
	wMin.rx() = min.x();
	wMin.ry() = min.y();

	wMax.rx() = max.x();
	wMax.ry() = max.y();
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);	// draws black border

    QPainter painter(this);
    QPainterPath pp;

    // first draw white background
    painter.fillRect(QRect(1, 1, width() - 2, height() - 2), Qt::white);

    // TODO; implement the following drawing functionality!
    // draw axis of abscissas (as given in world coords)
    // finally draw the function (also given in world coords)
    QPoint dc;
    int steps = width();;
    float x, y, dx;

    switch(function){
    case 0:{
        x = wMin.x();
        y = sin(x);

        dc = WC_to_DC(QPointF(x, y));
        pp.moveTo(dc);

        dx = (wMax.x() - wMin.x()) / steps;

        for (int i = 0; i < steps; i++) {
            x += dx;
            y = sin(x);
            dc = WC_to_DC(QPointF(x, y));
            pp.lineTo(dc);
        }
        break;
    }
    case 1:{
        QPointF p(0, 0);
        float phi;
        float radius = 1;

        for (int i=0; i<= steps; i++) {
            phi = i * (2 * M_PI / steps);
            x = radius * cos(phi) + p.x();
            y = radius * sin(phi) + p.y();
            //convert to pixel coords
            dc = WC_to_DC(QPointF(x, y));
            if (i == 0)
                pp.moveTo(dc);
            else
                pp.lineTo(dc);
        }
        break;
    }
    case 2:{
        QPointF p(0, 0);
        float t;

        for (int i=0; i<= steps; i++) {
            t = i * (2 * M_PI / steps);
            x = 16 * std::pow(sin(t),3) + p.x();
            y = 13*cos(t) - 5*cos(2*t) - 2*cos(3*t) - cos(4*t) + 2 + p.y();
            //convert to pixel coords
            dc = WC_to_DC(QPointF(x, y));
            if (i == 0)
                pp.moveTo(dc);
            else
                pp.lineTo(dc);
        }
        break;
    }
    }

    painter.setPen(Qt::red);
    painter.drawPath(pp);

    if(function == 2) {
        painter.fillPath(pp, Qt::red);
    }

    painter.setPen(Qt::black);
    QPainterPath axes;

    axes.moveTo(WC_to_DC(QPointF(wMin.x(), 0)));
    axes.lineTo(WC_to_DC(QPointF(wMax.x(), 0)));
    axes.moveTo(WC_to_DC(QPointF(0, wMin.y())));
    axes.lineTo(WC_to_DC(QPointF(0, wMax.y())));
    painter.drawPath(axes);
}

QPoint Canvas::WC_to_DC(const QPointF &wc)
{
    float wx0 = wMin.x(),
        wx1 = wMax.x(),
        wy0 = wMin.y(),
        wy1 = wMax.y();
    int dx0, dx1, dy0, dy1, x = 0, y = 0;

    dx1 = width();
    dx0 = 0;
    dy0 = height();
    dy1 = 0;

    // TODO; calculate window-viewport transformation for point (x, y)!
    x =  (wc.x() - wx0) / (wx1 - wx0 ) * (dx1 - dx0) + dx0;
    y =  (wc.y() - wy0) / (wy1 - wy0 ) * (dy1 - dy0) + dy0;


    return QPoint(x, y);
}

QPointF Canvas::DC_to_WC(const QPoint &dc)
{
	// TODO; calculate inverse transformation for (x, y)
    float wx0 = wMin.x(),
        wx1 = wMax.x(),
        wy0 = wMin.y(),
        wy1 = wMax.y();
    float dx0, dx1, dy0, dy1, x = 0, y = 0;

    dx1 = width();
    dx0 = 0;
    dy0 = height();
    dy1 = 0;

    x =  (dc.x() - dx0) / (dx1 - dx0 ) * (wx1 - wx0) + wx0;
    y =  (dc.y() - dy0) / (dy1 - dy0 ) * (wy1 - wy0)+ wy0;

    return QPointF(x, y);
}

void Canvas::setFunction(int f)
{
    function = f;
}

void Canvas::mousePressEvent(QMouseEvent *event){
    QPoint dc = event->position().toPoint();
    QPointF wc = DC_to_WC(dc);

    emit worldPosClicked("x = " + QString::number(wc.x(), 'f', 2) +
                         ", y = " + QString::number(wc.y(), 'f', 2));
}