#ifndef VOICE_H
#define VOICE_H

#include <QObject>
#include <patch.h>
#include <pseudovoice.h>
#include <QTime>
#include <envelope.h>
#include <QMap>
#include "state.h"
#include "musicutil.h"

class Voice : public QObject
{
    Q_OBJECT
public:
    explicit Voice(QObject *parent = nullptr);
    VoiceState update(quint64 t_us);
    void add_note(int note, int velocity);
    void set_mod(double mod) {_modval = mod;}
    Envelope::GateStatus gate_status();
    void release_note(int note);
    double age();
    void connectPatch(const Patch *patch) {
        connect(patch, &Patch::updated, this, &Voice::updatePatch);
        for (int i = 0; i < 6; i++) {
            connect(patch, &Patch::updated, &_pvoices[i], &PseudoVoice::updatePatch);
        }
    }

public slots:
    void updatePatch(Patch::PatchData patch) {
        note_voices.clear();
        note_stack.clear();
        _voice_mode = patch.voice_mode;

        double a = patch.timbre[patch.activeTimbre].env1_attack;
        double d = patch.timbre[patch.activeTimbre].env1_decay;
        double s = patch.timbre[patch.activeTimbre].env1_sustain;
        double r = patch.timbre[patch.activeTimbre].env1_release;
        _filter_env.setADSR(a,d,s,r);


        _cutoff_st = freq_to_st(patch.timbre[patch.activeTimbre].filter_freq);
        _resonance = freq_to_st(patch.timbre[patch.activeTimbre].filter_resonance);
        _filter_env_amt = double(patch.mod_matrix[Patch::MODSRC_ENV1][Patch::MOD_FILTER_CUTOFF]) / 128;
        _filter_mod_amt = double(patch.mod_matrix[Patch::MODSRC_MOD][Patch::MOD_FILTER_CUTOFF]) / 128;
//        _pbAmt = double(data.mod_matrix[Patch::MODSRC_PB][Patch::MOD_OSC_PITCH]) / 128;
    }
signals:

private:
    PseudoVoice _pvoices[6];
//    int _chorus_mode;
    Patch::VoiceMode _voice_mode;
    Patch::TimbreMode _timbre_mode;
    Envelope _filter_env;
    double _modval;

    // patch stuff:
    double _cutoff_st = 69;
    double _resonance = 0;

    // mod matrix stuff
    double _filter_env_amt;
    double _filter_mod_amt;

    // note tracking stuff
    QMap<int,int> note_voices; // notes with gate open or releasing-> pseudovoice index
    QVector<int> note_stack; // for mono mode
};

#endif // VOICE_H
