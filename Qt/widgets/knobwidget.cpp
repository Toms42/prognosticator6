#include "knobwidget.h"

#define PI 3.14159265

KnobWidget::KnobWidget(QWidget *parent)
    : QDial{parent}
{
    connect(this, &KnobWidget::valueChanged, this, &KnobWidget::_onValueChanged);
    emit realValueUpdated(realValue());
}

void KnobWidget::setRealValue(double value) {
    if (_logarithmic) {
//        assert(fabs(_valmin) > 0.0001);
        setValue(log(value/_valmin)/log(_valmax/_valmin)*(maximum()-minimum())+minimum());
    } else {
        setValue((value - _valmin)/(_valmax-_valmin) * (maximum() - minimum()) + minimum());
    }
    emit realValueUpdated(value);
    update();
}

double KnobWidget::realValue() {
    if (_logarithmic) {
//        assert(fabs(_valmin) > 0.0001);
        return pow(_valmax/_valmin, (double(value())-minimum())/(maximum()-minimum()))*_valmin;
    } else {
        return double(value() - minimum())/(maximum()-minimum()) * (_valmax - _valmin) + _valmin;
    }
}

void KnobWidget::_onValueChanged(int value) {
    emit realValueUpdated(realValue());
}

void KnobWidget::paintEvent(QPaintEvent *pe) {
    int w = width();
    int h = height();
    int hdial = h;

//    if (_label.length() > 0) hdial -= 18;
//    if (_showtext > 0) hdial -= 18;
    hdial -= 2 * 18;
    hdial = std::min(hdial, w);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QFont font = painter.font();
    font.setPointSize(9); // TODO: property
    painter.setFont(font);
    QPen pen;
    pen.setWidth(_linewidth);
    pen.setColor(_color);
    painter.setPen(pen);

    if (_label.length() > 0) {
        QRect bbox = QRect(0, h/2-(hdial/2)-20, w, 16);
        painter.drawText(bbox,Qt::AlignCenter, _label);
    }
    if (_showtext) {
        QRect bbox = QRect(0, h/2+(hdial/2)+2, w, 16);
        QString content = QString::number(realValue(), 'f', _numdigits) + _unit;
        painter.drawText(bbox, Qt::AlignCenter, content);
    }

    QRect dial = QRect(w/2-hdial/2, h/2-hdial/2, hdial, hdial);
//    painter.drawRect(dial);
    int dtotal = (360-40)*16;
    int dfill = (double(value())-minimum())/(maximum()-minimum())*dtotal;
    pen.setColor(QColor(100,100,100));
    painter.setPen(pen);
    painter.drawArc(dial, -110*16-dfill, -(dtotal-dfill));
    int linex = cos((dfill/16+20+90) * PI / 180.) * hdial/2;
    int liney = sin((dfill/16+20+90) * PI / 180.) * hdial/2;
    pen.setColor(_color);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    painter.drawLine(w/2,h/2,w/2+linex,h/2+liney);
    pen.setCapStyle(Qt::FlatCap);
    painter.setPen(pen);
    painter.drawArc(dial, -110*16, -dfill);


}
