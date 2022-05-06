#include "voice.h"
#include "musicutil.h"

Voice::Voice(QObject *parent)
    : QObject{parent}
{

}

VoiceState Voice::update(quint64 t_us) {
    float env = _filter_env.eval(t_us);

    double cutoff = _cutoff_st + _filter_env_amt * 60 * env + _filter_mod_amt * _modval * 40 * _filter_mod_amt;

    VoiceState res;
    res.f.cutoff = cutoff;
    res.f.resonance = _resonance;
    res.s0 = _pvoices[0].update(t_us).s0;
    res.s1 = _pvoices[0].update(t_us).s1;
    res.s2 = _pvoices[1].update(t_us).s0;
    res.s3 = _pvoices[1].update(t_us).s1;
    res.s4 = _pvoices[2].update(t_us).s0;
    res.s5 = _pvoices[2].update(t_us).s1;
    res.s6 = _pvoices[3].update(t_us).s0;
    res.s7 = _pvoices[3].update(t_us).s1;
    res.s8 = _pvoices[4].update(t_us).s0;
    res.s9 = _pvoices[4].update(t_us).s1;
    res.s10 = _pvoices[5].update(t_us).s0;
    res.s11 = _pvoices[5].update(t_us).s1;

    return res;
}

void Voice::add_note(int note, int velocity) {
    // prune notes that are off:
    for (int n : note_voices) {
        if (_pvoices[n].gateStatus() == Envelope::GATE_OFF) {
            note_voices.remove(n);
        }
    }

    switch (_voice_mode) {
    // duophonic and single both just use one note at a time for this voice.
    // also reset the filter gate every time.
    case Patch::VOICE_SINGLE:
    case Patch::VOICE_DUOPHONIC:
        _filter_env.press();
        _pvoices[0].pressNote(note, velocity);
        break;

    case Patch::VOICE_MONO:
        if (note_stack.empty()) {
            _filter_env.press();
            _pvoices[0].pressNote(note, velocity);
        } else {
            _pvoices[0].morphNote(note);
        }
        note_stack.push_back(note);
        break;

    case Patch::VOICE_PARAPHONIC:
        if (note_voices.empty()) {
            _filter_env.press();
        }
        if (note_voices.size() > 6) {

            // check if a note is in release:
            int nmin = 0;
            float min = 10000E10;
            for (int n : note_voices) {
                if (_pvoices[note_voices[n]].getTimeLeft() < min) {
                    nmin = n;
                    min = _pvoices[note_voices[n]].getTimeLeft();
                }
            }
            int v = note_voices[nmin];
            note_voices.remove(nmin);
            note_voices.insert(note, v);
            _pvoices[v].pressNote(note, velocity);

        } else {
            // find an open voice
            auto vs = note_voices.values();
            for (int i = 0; i < 6; i++) {
                if(!vs.contains(i)) {
                    note_voices.insert(note, i);
                    _pvoices[i].pressNote(note, velocity);
                }
            }
        }
        break;
    }
}

Envelope::GateStatus Voice::gate_status() {

    Envelope::GateStatus status = Envelope::GATE_OFF;

    for (auto& v : _pvoices)
        if (v.gateStatus() > status)
            status = v.gateStatus();

    return status;
}

void Voice::release_note(int note) {
    // Mono works differently:
    switch (_voice_mode) {
    case Patch::VOICE_SINGLE:
    case Patch::VOICE_DUOPHONIC:
        _pvoices[0].releaseNote();
        _filter_env.release();
        break;
    case Patch::VOICE_MONO:
        // this mode will morph the pitch back into the old one if possible.

        // this note is not in the stack anymore.
        note_stack.removeAll(note);

        // last note!
        if (note_stack.empty()) {
            _filter_env.release();
            _pvoices[0].releaseNote();
        } else {
            int next = note_stack.back();
            _pvoices[0].morphNote(next);
        }
        break;
    case Patch::VOICE_PARAPHONIC:
        if (!note_voices.contains(note))
            return; // the note is already off.

        // release the note
        int v = note_voices[note];
        _pvoices[v].releaseNote();

        // check if all notes are released:
        bool allReleased = true;
        for (int i = 0; i < 6; i++) {
            allReleased &= (_pvoices[i].gateStatus() != Envelope::GATE_ON);
        }
        if (allReleased)
            _filter_env.release();
    }
}

double Voice::age() {
    return _filter_env.age(); // TODO idk
}
