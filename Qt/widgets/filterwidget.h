#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>
#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <cmath>

const float MAXDB = 100;
const float MAXFREQ = 30000;
const float MINFREQ = 10;
const int NUMPOLES = 4;
// Rollof is db per maxfreq, aka 20 log_10(MAXFREQ) times the number of poles.
const float DB_PER_MAXFREQ = 20.*log(MAXFREQ)/log(10.)*NUMPOLES;

class FilterWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor color0 READ getColor0 WRITE setColor0)
    Q_PROPERTY(QColor color1 READ getColor1 WRITE setColor1)
    Q_PROPERTY(double linewidth READ getLineWidth WRITE setLineWidth)
public:
    explicit FilterWidget(QWidget *parent = nullptr);

    QColor getColor0() const {return _color0;};
    QColor getColor1() const {return _color1;};
    double getLineWidth() const {return _linewidth;};
    void setCutoff(double freq) {_freq = freq; update();};
    void setResonance(double Q) {_q = Q; update();};

protected:
    void paintEvent(QPaintEvent *event) override;

public slots:
    void setCutoff0(double freq) {_freq0 = freq; update();};
    void setResonance0(double Q) {_q0 = Q; update();};
    void setCutoff1(double freq) {_freq1 = freq; update();};
    void setResonance1(double Q) {_q1 = Q; update();};

    void setColor0(QColor c) {_color0 = c;};
    void setColor1(QColor c) {_color1 = c;};
    void setLineWidth(double w) {_linewidth = w;};

signals:

private:
    QColor _color0 = Qt::white;
    QColor _color1 = Qt::white;
    double _linewidth = 1;

    float _freq = 800;
    float _q = 20;
    float _freq0 = 300;
    float _q0 = 4;
    float _freq1 =8000;
    float _q1 = 20;
    bool _antialiased;

    QPen _pen;
};

#endif // FILTERWIDGET_H
