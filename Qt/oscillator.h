#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <QObject>
#include <patch.h>

class Oscillator : public QObject
{
    Q_OBJECT
public:
    explicit Oscillator(QObject *parent = nullptr);

public slots:
    void setFrequency(double freq);
    void setAmplitude(double amp);

private:
    double freq;
    double amplitude;
    Patch::OscShape shape;
    double phase;

signals:

};

#endif // OSCILLATOR_H
