#ifndef OSCILLATORWIDGET_H
#define OSCILLATORWIDGET_H

#include <QWidget>
#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <cmath>

const quint32 PERIOD = 1000;

class OscillatorWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor color0 READ getColor0 WRITE setColor0)
    Q_PROPERTY(QColor color1 READ getColor1 WRITE setColor1)
    Q_PROPERTY(double linewidth READ getLineWidth WRITE setLineWidth)
    Q_PROPERTY(int numwaves READ getNumWaves WRITE setNumWaves)
    Q_PROPERTY(double waveamplitude READ getWaveAmplitude WRITE setWaveAmplitude)

public:
    explicit OscillatorWidget(QWidget *parent = nullptr);
    QColor getColor0() const {return _color0;};
    QColor getColor1() const {return _color1;};
    double getLineWidth() const {return _linewidth;};
    int getNumWaves() const {return _numWaves;};
    double getWaveAmplitude() const {return _waveAmplitude;};

protected:
    virtual void paintEvent(QPaintEvent *pe) override;

public slots:
    void setColor0(QColor c) {_color0 = c;};
    void setColor1(QColor c) {_color1 = c;};
    void setLineWidth(double w) {_linewidth = w;};
    void setNumWaves(int n) {_numWaves = n;};
    void setWaveAmplitude(double a) {_waveAmplitude = a;};

    void setShape0(double s) {_shape0 = s; update();};
    void setShape1(double s) {_shape1 = s; update();};

private:
    QColor _color0 = Qt::white;
    QColor _color1 = Qt::white;
    double _linewidth = 1;
    int _numWaves = 4;
    double _waveAmplitude = 0.2;

    double _shape0 = 0;
    double _shape1 = 0;

};

#endif // OSCILLATORWIDGET_H
