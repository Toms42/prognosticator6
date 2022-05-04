#include "lfowidget.h"

LfoWidget::LfoWidget(QWidget *parent)
    : QWidget{parent}
{

}

inline double sampleRand(double t) {
    quint32 seed = floor(t);
    quint32 r =simpleRand(seed);
    return double(r)/UINT32_MAX*2-1.;
}

inline double  sampleSquare(double t) {
    t = t-floor(t);
    return t < 0.5 ? 1. : -1.;
}

inline double sampleSaw(double t) {
    t = t-floor(t);
    return t < 0.5 ? 2*t : 2*t-2;
}

inline double sampleTri(double t) {
    t = t-floor(t);
    if (t < 0.25)
        return t * 4;
    if (t <= 0.75)
        return 1-4*(t-0.25);
    return 4*(t-0.75)-1;
}

inline double sampleSin(double t) {
    return sin(t*2*M_PI);
}

inline double _sample(double t, Patch::LfoShape shape) {
    if (shape == Patch::LFO_RAND) {
        return sampleRand(t);
    }
    if (shape == Patch::LFO_SQUARE) {
        return sampleSquare(t);
    }
    if (shape == Patch::LFO_TRI) {
        return sampleTri(t);
    }
    if (shape == Patch::LFO_SAW) {
        return sampleSaw(t);
    }
    if (shape == Patch::LFO_SIN) {
        return sampleSin(t);
    }
    return 0;
}

void LfoWidget::paintEvent(QPaintEvent *) {
    int w = width();
    int h = height();

    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setWidthF(_linewidth);
    pen.setColor(QColor(100,100,100));
    painter.setPen(pen);

    // draw axi
    painter.drawLine(2,0,2,h);
    painter.drawLine(2,h/2,w,h/2);

    // draw indicator
    pen.setColor(_color);
    painter.setPen(pen);
    double v = (_sample(_t, _shape)/2*0.98 + .5) * h;
    painter.fillRect(QRectF(0,v-2,4,4), QBrush(QColor(_color)));

    int numPeriods = 3;

    // draw wave
    QPainterPath path(QPointF(2, v));
    for (int x = 3; x < w; x++) {
        double t = _t + (double(x-2)/w)*numPeriods;
        double v = (_sample(t, _shape)/2*0.98 + .5) * h;
        path.lineTo(QPointF(x,v));
    }

    painter.strokePath(path, pen);
}
