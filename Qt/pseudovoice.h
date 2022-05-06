#ifndef PSEUDOVOICE_H
#define PSEUDOVOICE_H

#include <QObject>
#include <QTime>
#include <patch.h>
#include <QVector>
#include "state.h"
#include <cmath>
#include "envelope.h"

class PseudoVoice : public QObject
{
    Q_OBJECT
public:
    explicit PseudoVoice(QObject *parent = nullptr);
    DualOscillatorState update(quint64 t_us);
    double getFreq();
    void pressNote(int note, int velocity);
    void morphNote(int note);
    void releaseNote();
    Envelope::GateStatus gateStatus();
    double getTimeLeft() {return _amp_env.timeLeft();};

public slots:
//    void updateEnvelope(double amp);
//    void updateFreq(double freq);
    void updatePatch(Patch::PatchData data) {
        _shape0 = data.timbre[data.activeTimbre].osc_shape0;
        _shape1 = data.timbre[data.activeTimbre].osc_shape1;
        _detune = data.timbre[data.activeTimbre].osc_detune;
        _octave = data.timbre[data.activeTimbre].osc_octave;
        _mix0 = data.timbre[data.activeTimbre].mixer_amount0;
        _mix1 = data.timbre[data.activeTimbre].mixer_amount1;

        double a = data.timbre[data.activeTimbre].env0_attack;
        double d = data.timbre[data.activeTimbre].env0_decay;
        double s = data.timbre[data.activeTimbre].env0_sustain;
        double r = data.timbre[data.activeTimbre].env0_release;
        _amp_env.setADSR(a,d,s,r);

        _modVelAmp = double(data.mod_matrix[Patch::MODSRC_VEL][Patch::MOD_AMP]) / 128;
        _pbAmt = double(data.mod_matrix[Patch::MODSRC_PB][Patch::MOD_OSC_PITCH]) / 128;
//        _pbAmt = data.mod_matrix[Patch::MODSRC_ENV1][Patch::MOD_OSC_];
//        _modVelAmp = data.mod_matrix[Patch::MODSRC_LFO0][Patch::MOD_AMP];
    }

private:
    Envelope _amp_env;
    double _phase0;
    double _phase1;
    double _freq0; // semitones
    double _freq1; // semitones
    double _shape0;
    double _shape1;
    double _vel_mult;

    // mod matrix stuff:
    double _modVelAmp = 0;
    double _pbAmt = 0;

    // patch stuff
    double _detune=0;
    int _octave = 0;
    double _mix0 = 0;
    double _mix1 = 0;
signals:
};

#endif // PSEUDOVOICE_H
