#include "pseudovoice.h"

PseudoVoice::PseudoVoice(QObject *parent)
    : QObject{parent}
{
}

DualOscillatorState PseudoVoice::update(quint64 t_us) {
    float env = _amp_env.eval(t_us);

    DualOscillatorState res;
    res.s0.freq = _freq0 * 500;
    res.s1.freq = _freq1 * 500;
    res.s0.phase = _phase0;
    res.s1.phase = _phase1;
    res.s0.amp = _mix0 * env * (_vel_mult) * 255;
    res.s1.amp = _mix1 * env * (_vel_mult) * 255;
    res.s0.shape = _shape0;
    res.s1.shape = _shape1;
    return res;
}

void PseudoVoice::pressNote(int note, int velocity) {
    _freq0 = note;
    _freq1 = note + _detune + 12 * _octave;
    _amp_env.press();
    _vel_mult = pow(velocity, _modVelAmp);
}

void PseudoVoice::releaseNote() {
    _amp_env.release();
}

void PseudoVoice::morphNote(int note) {
    _freq0 = note;
    _freq1 = note + _detune + 12 * _octave;
}

Envelope::GateStatus PseudoVoice::gateStatus() {
    return _amp_env.gateStatus();
}
