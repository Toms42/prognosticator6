#include "filterwidget.h"

FilterWidget::FilterWidget(QWidget *parent)
    : QWidget{parent}
{
    _antialiased = false;

    _pen.setStyle(Qt::SolidLine);
    _pen.setJoinStyle(Qt::RoundJoin);
    _pen.setColor(QColor(127,127,127));
    update();
}

inline float db_to_h(float db, int h) {
    return h - ((db+50)/MAXDB)*h;
}
inline float f_to_x(float f, int w) {
    return log(f/MINFREQ)/log(MAXFREQ/MINFREQ)*w;
}
inline float x_to_f(int x, int w) {
    // logarithmic scale x= log(freq, MAXFREQ)
    return pow(MAXFREQ/MINFREQ, x/w)*MINFREQ;
}
inline float _dbgain(int x, int w, float q, float cutoff_x) {
    if (x < cutoff_x) {
        return fmax(0., (x - cutoff_x)*(DB_PER_MAXFREQ/w) + q);
    } else {
        return (cutoff_x - x)*(DB_PER_MAXFREQ/w) + q;
    }
}

void FilterWidget::paintEvent(QPaintEvent *) {
    int w = width();
    int h = height();

    float xco0 = f_to_x(_freq0, w);
    float xco1 = f_to_x(_freq1, w);


    QPainterPath path0(QPointF(0.,db_to_h(_dbgain(0, w, _q0, xco0),h))); // voice 0 filter painter
    QPainterPath path1(QPointF(0.,db_to_h(_dbgain(0, w, _q1, xco1),h))); // voice 1 filter painter

    for(int x = 0; x < w; x+=1) {
        float y0 = db_to_h(_dbgain(x, w, _q0, xco0), h);
        float y1 = db_to_h(_dbgain(x, w, _q1, xco1), h);
        if (y0 < h)
            path0.lineTo(x,y0);
        if (y1 < h)
            path1.lineTo(x,y1);
    }

    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    _pen.setWidthF(_linewidth);
    _pen.setColor(_color0);
    painter.strokePath(path1, _pen);
    _pen.setColor(_color1);
    painter.strokePath(path0, _pen);
    _pen.setColor(QColor(150, 150, 150));
    painter.setPen(_pen);
    painter.drawLine(0, h, w, h);

//    qInfo(QString("hello world %1,%2").arg(w,h));
//    qInfo("hello world: %d,%d",w,h);

}
