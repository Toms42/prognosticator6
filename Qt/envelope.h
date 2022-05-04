#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <QObject>
#include <QTime>

class Envelope : public QObject
{
    Q_OBJECT
public:
    enum GateStatus {
        GATE_OFF,
        GATE_RELEASE,
        GATE_ON,
    };

    explicit Envelope(QObject *parent = nullptr);
    void update(QTime t);
    double getVal();
    double reset();
    double finished();
    GateStatus gateStatus();

public slots:
    void setADSR(double A, double D, double S, double R);

signals:

};

#endif // ENVELOPE_H
