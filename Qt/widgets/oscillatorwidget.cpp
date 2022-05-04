#include "oscillatorwidget.h"
OscillatorWidget::OscillatorWidget(QWidget *parent)
    : QWidget{parent}
{

}

inline double  sampleSquare(double t) {
    return t < 0.5 ? 1. : -1.;
}

inline double sampleSaw(double t) {
    return t < 0.5 ? 2*t : 2*t-2;
}

inline double sampleTri(double t) {
    if (t < 0.25)
        return t * 4;
    if (t <= 0.75)
        return 1-4*(t-0.25);
    return 4*(t-0.75)-1;
}

inline double sampleSin(double t) {
    return sin(t*2*M_PI);
}

inline double sampleOscillator(double t, double shape) {
    double interp;
    double s;
    if (shape < 1./3.) {
        interp = shape * 3;
        s = interp*sampleTri(t) + (1-interp)*sampleSin(t);
    } else if (shape < 2./3.) {
        interp = (shape - 1./3.) * 3;
        s = interp*sampleSaw(t) + (1-interp)*sampleTri(t);
    } else {
        interp = (shape - 2./3.) * 3;
        s = interp*sampleSquare(t) + (1-interp)*sampleSaw(t);
    }
}

void OscillatorWidget::paintEvent(QPaintEvent *) {
    int w = width();
    int h = height();

    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setWidthF(_linewidth);
    pen.setColor(QColor(100,100,100));

    assert(_numWaves >= 2);

    // draw background waves
    for (int i = 0; i < _numWaves; i++) {
        double shape = double(i) / (_numWaves-1);
        double waveCenter = h - _linewidth - (h-2*_linewidth)*(shape * (1-2*_waveAmplitude) + _waveAmplitude);

        double s = sampleOscillator(0, shape);
        QPainterPath path(QPointF(0, -s*_waveAmplitude*h+waveCenter));
        for (int x = 1; x < w; x++) {
            double t = double(x)/w;
            double s = sampleOscillator(t, shape);
            path.lineTo(QPointF(x, -s*_waveAmplitude*h+waveCenter));
        }
        painter.strokePath(path, pen);
    }

    // draw wave0
    double waveCenter0 = h - _linewidth - (h-2*_linewidth)*(_shape0 * (1-2*_waveAmplitude) + _waveAmplitude);
    double s0 = sampleOscillator(0, _shape0);
    QPainterPath path0(QPointF(0, -s0*_waveAmplitude*h+waveCenter0));
    pen.setColor(_color0);

    for (int x = 1; x < w; x++) {
        double t = double(x)/w;
        double s = sampleOscillator(t, _shape0);
        path0.lineTo(QPointF(x, -s*_waveAmplitude*h+waveCenter0));
    }
    painter.strokePath(path0, pen);

    // draw wave1
    double waveCenter1 = h - _linewidth - (h-2*_linewidth)*(_shape1 * (1-2*_waveAmplitude) + _waveAmplitude);
    double s1 = sampleOscillator(0, _shape1);
    QPainterPath path1(QPointF(0, -s1*_waveAmplitude*h+waveCenter1));
    pen.setColor(_color1);

    for (int x = 1; x < w; x++) {
        double t = double(x)/w;
        double s = sampleOscillator(t, _shape1);
        path1.lineTo(QPointF(x, -s*_waveAmplitude*h+waveCenter1));
    }
    painter.strokePath(path1, pen);

}
