#include "envelopewidget.h"

EnvelopeWidget::EnvelopeWidget(QWidget *parent)
    : QWidget{parent}
{
//    _antialiased = false;
    update();
}

void EnvelopeWidget::_onKnobChange(int) {
    update();
}

void EnvelopeWidget::_tryConnect() {
    if (!_knob_attack || !_knob_decay || !_knob_sustain || !_knob_release) {
        return;
    }

    connect(_knob_attack, &KnobWidget::valueChanged, this, &EnvelopeWidget::_onKnobChange);
    connect(_knob_decay, &KnobWidget::valueChanged, this, &EnvelopeWidget::_onKnobChange);
    connect(_knob_sustain, &KnobWidget::valueChanged, this, &EnvelopeWidget::_onKnobChange);
    connect(_knob_release, &KnobWidget::valueChanged, this, &EnvelopeWidget::_onKnobChange);
    _connected = true;

    update();
}



void EnvelopeWidget::paintEvent(QPaintEvent *) {
    int w = width();
    int h = height();

    if (!_connected) return;

    int sustain_amt = 255; // TODO: auto determine this!
    int max = _knob_attack->maximum() + _knob_decay->maximum() + _knob_release->maximum() + sustain_amt;

    QPainterPath path(QPointF(0,h)); // voice 0 filter painter
    path.lineTo(_knob_attack->value()*w/max,0);
    path.lineTo((_knob_attack->value()+_knob_decay->value())*w/max,
                (_knob_sustain->maximum() - _knob_sustain->value())*h/_knob_sustain->maximum());
    if(_knob_sustain->value() != _knob_sustain->minimum()) {
        path.lineTo((_knob_attack->value()+_knob_decay->value() + sustain_amt)*w/max,
                    (_knob_sustain->maximum() - _knob_sustain->value())*h/_knob_sustain->maximum());
        path.lineTo((_knob_attack->value()+_knob_decay->value() + sustain_amt + _knob_release->value())*w/max,
                    h);
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setWidthF(_linewidth);
    pen.setColor(_color);
    painter.strokePath(path, pen);
}
