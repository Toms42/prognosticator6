#ifndef ENVELOPEWIDGET_H
#define ENVELOPEWIDGET_H

#include <QWidget>
#include "knobwidget.h"
#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <cmath>

class EnvelopeWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ getColor WRITE setColor)
    Q_PROPERTY(double linewidth READ getLineWidth WRITE setLineWidth)
    Q_PROPERTY(QString knobattack READ getKnobAttack WRITE setKnobAttack)
    Q_PROPERTY(QString knobdecay READ getKnobDecay WRITE setKnobDecay)
    Q_PROPERTY(QString knobsustain READ getKnobSustain WRITE setKnobSustain)
    Q_PROPERTY(QString knobrelease READ getKnobRelease WRITE setKnobRelease)

public:
    explicit EnvelopeWidget(QWidget *parent = nullptr);
    QColor getColor() const {return _color;};
    double getLineWidth() const {return _linewidth;};

    QString getKnobAttack() const {if (_knob_attack) return _knob_attack->objectName(); else return tr("");}
    QString getKnobDecay() const {if (_knob_decay) return _knob_decay->objectName(); else return tr("");}
    QString getKnobSustain() const {if (_knob_sustain) return _knob_sustain->objectName(); else return tr("");}
    QString getKnobRelease() const {if (_knob_release) return _knob_release->objectName(); else return tr("");}

public slots:
    void setColor(QColor c) {_color = c;};
    void setLineWidth(double w) {_linewidth = w;};
    void setKnobAttack(QString k) {_knob_attack = (parent()->findChild<KnobWidget*>(k)); _tryConnect();};
    void setKnobDecay(QString k) {_knob_decay = (parent()->findChild<KnobWidget*>(k)); _tryConnect();};
    void setKnobSustain(QString k) {_knob_sustain = (parent()->findChild<KnobWidget*>(k)); _tryConnect();};
    void setKnobRelease(QString k) {_knob_release = (parent()->findChild<KnobWidget*>(k)); _tryConnect();};

protected:
    virtual void paintEvent(QPaintEvent *pe) override;

signals:

private:
    void _tryConnect();
    bool _connected = false;
    void _onKnobChange(int);

    KnobWidget *_knob_attack = nullptr;
    KnobWidget *_knob_decay = nullptr;
    KnobWidget *_knob_sustain = nullptr;
    KnobWidget *_knob_release = nullptr;

    QColor _color = Qt::white;
    double _linewidth = 1;
};

#endif // ENVELOPEWIDGET_H
