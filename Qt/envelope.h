#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <QObject>
#include <QTime>

class Envelope : public QObject
{
    Q_OBJECT
public:
    explicit Envelope(QObject *parent = nullptr);

    enum GateStatus {
        GATE_OFF,
        GATE_RELEASE,
        GATE_ON
    };

    float eval(quint64 t);
    void setADSR(double A, double D, double S, double R) {_a=A;_d=D;_s_nominal=S;_r=R;};
    void press() {_gate=GATE_ON; _tp=_t; _s = _s_nominal;};
    void release() {
        _s = eval(_t);
        _gate=GATE_RELEASE;
        _tr=_t;
    };
    float timeLeft() {
        if(_gate == GATE_RELEASE)
            return _r - float(_t - _tr);
        if (_gate == GATE_OFF)
            return 0;
        if (_gate == GATE_ON)
            return 10000E6; //eh
        return 0;
    }
    float age() {
        return _gate == GATE_OFF ? 0 : _t - _tp;
    }
    GateStatus gateStatus() {return _gate;};

signals:

private:
    GateStatus _gate=GATE_OFF;
    quint64 _tp=0;
    quint64 _tr=0;
    quint64 _t=0;
    float _a=0; //s
    float _d=0; //s
    float _s_nominal=0; // 0-1
    float _s=0; //0-1
    float _r=0; //s

};

#endif // ENVELOPE_H
