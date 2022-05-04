#include "oscillatorwidget.h"

OscillatorWidget::OscillatorWidget(QWidget *parent)
    : QWidget{parent}
{

}
inline quint8 sampleTri(quint32 t) {
}

inline quint8 sampleSaw(quint32 t) {
    return ((t + PERIOD/2)%PERIOD)*255 / PERIOD;
}

inline quint8  sampleSquare(quint32 t) {
    return ((t + PERIOD/2)%PERIOD) > PERIOD/2 ? 255 : 0;
}

inline quint8 sampleSin(quint32 t) {

}

double sampleOscillator(double t, int shape) {
    quint32 tu = quint32(t * 44100);

}

void OscillatorWidget::paintEvent(QPaintEvent *) {
    int w = width();
    int h = height();

}
