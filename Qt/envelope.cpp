#include "envelope.h"
#include <cmath>

Envelope::Envelope(QObject *parent)
    : QObject{parent}
{

}

inline float f1(float x) {return powf(x,1./3);}
inline float f2(float x) {return powf(x,3.);}
inline float g1(float x, float l, float u) {return f1((x-l)/(u-l))*(u-l)+l;}
inline float g2(float x, float l, float u) {return f2((x-l)/(u-l))*(u-l)+l;}

// from https://www.desmos.com/calculator/nduy9l2pez
float Envelope::eval(quint64 t) {
    _t = t;
    if (_gate == GATE_OFF) return 0.;
    if (_gate == GATE_ON) {
        float x = float(t-_tp)/1E6;
        if(x <= _a) {
            return f1(x/_a);
        } else if (x <= _a + _d){
            return g2((_s-1)/_d*(x-_a)+1,_s,1.);
        } else {
            return _s;
        }
    }
    if (_gate == GATE_RELEASE) {
        float x = float(t-_tr)/1E6;
        if (x <= _r) {
            return g2((-_s/_r)*x+_s,0,_s);
        }
        _gate = GATE_OFF;
    }
    return 0.;
}
