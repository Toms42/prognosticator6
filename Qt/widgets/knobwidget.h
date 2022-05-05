#ifndef KNOBWIDGET_H
#define KNOBWIDGET_H

#include <QWidget>
#include <QDial>
#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <cmath>
#include <QFont>
#include <QRect>
#include <QTime>
#include <QElapsedTimer>

class KnobWidget : public QDial
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ getColor WRITE setColor)
    Q_PROPERTY(double linewidth READ getLineWidth WRITE setLineWidth)
    Q_PROPERTY(bool showtext READ getShowText WRITE setShowText)
    Q_PROPERTY(QString unit READ getUnit WRITE setUnit)
    Q_PROPERTY(int numdigits READ getNumDigits WRITE setNumDigits)
    Q_PROPERTY(QString label READ getLabel WRITE setLabel)
    Q_PROPERTY(bool logarithmic READ getLogarithmic WRITE setLogarithmic)
    Q_PROPERTY(double valmin READ getValMin WRITE setValMin)
    Q_PROPERTY(double valmax READ getValMax WRITE setValMax)

public:
    explicit KnobWidget(QWidget *parent = nullptr);

    QColor getColor() const {return _color;};
    double getLineWidth() const {return _linewidth;};
    QString getUnit() const {return _unit;};
    int getNumDigits() const {return _numdigits;};
    bool getShowText() const {return _showtext;};
    QString getLabel() const {return _label;};
    double getValMin() const {return _valmin;};
    double getValMax() const {return _valmax;};
    bool getLogarithmic() const {return _logarithmic;};

    double realValue();

public slots:
    void setColor(const QColor color) {_color = color;};
    void setLineWidth(double w) {_linewidth = w;};
    void setShowText(bool w) {_showtext = w;};
    void setUnit(QString u) {_unit = u;};
    void setLabel(QString u) {_label = u;};
    void setNumDigits(int n) {_numdigits = n;};
    void setValMin(double x) {_valmin = x;};
    void setValMax(double x) {_valmax = x;};
    void setLogarithmic(bool x) {_logarithmic = x;};

    void setRealValue(double value);

    void setKnobPressed() {emit knobPressed(); _timer.start();};
    void setKnobReleased() {emit knobReleased(); if (_timer.elapsed() > 1000) {emit knobLongClicked();} else {emit knobClicked();}};

signals:
    void realValueUpdated(float value);
    void knobPressed();
    void knobReleased();
    void knobLongClicked();
    void knobClicked();

protected:
    void paintEvent(QPaintEvent *pe) override;

private:
    QElapsedTimer _timer;
    QColor _color = Qt::white;
    double _linewidth = 1;
    bool _showtext;
    QString _unit;
    int _numdigits;
    QString _label;

    double _valmin = 1;
    double _valmax = 10;
    double _realVal;
    bool _logarithmic;

    void _onValueChanged(int value);

};

#endif // KNOBWIDGET_H
