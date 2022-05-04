#ifndef LFOWIDGET_H
#define LFOWIDGET_H

#include <QWidget>
#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <cmath>
#include "../simplerandom.h"
#include "../patch.h"

class LfoWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ getColor WRITE setColor)
    Q_PROPERTY(double linewidth READ getLineWidth WRITE setLineWidth)
public:
    explicit LfoWidget(QWidget *parent = nullptr);
    QColor getColor() const {return _color;};
    double getLineWidth() const {return _linewidth;};

public slots:
    void setColor(QColor c) {_color = c;};
    void setLineWidth(double w) {_linewidth = w;};
    void updateShape(int i) {_shape = Patch::LfoShape(i); update();};
    void updateT(double t) {_t = t; update();};

protected:
    virtual void paintEvent(QPaintEvent *pe) override;

signals:
private:
    QColor _color = Qt::white;
    double _linewidth = 1;
    double _t = 4.4; //current period

    Patch::LfoShape _shape = Patch::LFO_SIN;
};

#endif // LFOWIDGET_H
