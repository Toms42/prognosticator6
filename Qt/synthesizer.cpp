#include "synthesizer.h"

#ifdef linux
#include <sys/time.h>
#include <unistd.h>
#else
#error linux only fam
#endif

Synthesizer::Synthesizer(QObject *parent)
    : QThread{parent}
{

}

void Synthesizer::run() {
    while(!isInterruptionRequested()) {
        update();

        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        long tus = (ts.tv_nsec%_period_us*1000)/1000;
        usleep(_period_us-tus);   // wait until next 5000us multiple
    }
}

void Synthesizer::busyboi(void) {
    double dude = 1E7;
    for(long i = 0; i < 1E4; i++) {
        dude /= 0.412;
    }
//    qInfo(qUtf8Printable(QString("busy boi was busy dude: %1").arg(QString::number(dude))));
}

void Synthesizer::update(void) {
//    qint64 t = QDateTime::currentMSecsSinceEpoch()*1000;
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    int diff = ts.tv_nsec - last.tv_nsec;
    last = ts;
    diff = diff < 0 ? diff + 1E9 : diff;

//    qInfo(qUtf8Printable(QString::number((diff/1000))));
    if (abs(diff/1000 - _period_us) > 150) {
//        qInfo(qUtf8Printable(QString("Greater than 150uS jitter: %1").arg(QString::number(diff/1000))));
    }


    // Update both voices.
    // Envelopes should update, and we can
    quint64 t = ts.tv_sec*1E6 + ts.tv_nsec/1E3;

    SynthState state;
    state.v0 = _v0.update(t);
    state.v1 = _v1.update(t);
    state.add = _voice_mode == Patch::VOICE_DUOPHONIC || _timbre_mode == Patch::TIMBRE_DUAL || _timbre_mode == Patch::TIMBRE_DUALMIDI || _timbre_mode == Patch::TIMBRE_SINGLE_STEREO;
    state.volume = 1.;

    QString msg = "notes: ";
    msg += state.v0.s0.amp > 0 ? " " + QString::number(state.v0.s0.freq/500) + ":" + QString::number(state.v0.s0.amp) : "";
    msg += state.v0.s2.amp > 0 ? " " + QString::number(state.v0.s2.freq/500) + ":" + QString::number(state.v0.s2.amp) : "";
    msg += state.v0.s4.amp > 0 ? " " + QString::number(state.v0.s3.freq/500) + ":" + QString::number(state.v0.s3.amp) : "";
    msg += state.v0.s6.amp > 0 ? " " + QString::number(state.v0.s5.freq/500) + ":" + QString::number(state.v0.s5.amp) : "";
    msg += state.v0.s8.amp > 0 ? " " + QString::number(state.v0.s7.freq/500) + ":" + QString::number(state.v0.s7.amp) : "";
    msg += state.v0.s10.amp > 0 ? " " + QString::number(state.v0.s9.freq/500) + ":" + QString::number(state.v0.s9.amp) : "";
    msg += "\n";
    qInfo(qUtf8Printable(msg));
}

// Trigger a note. This sends the note to the appropriate voice(s) based on timbre and voicing modes.
// Note that depending on voice modes the voices might behave differently when a new note is added.
void Synthesizer::_addNote(int note, int velocity, int channel) {
    switch (_voice_mode) {

    // MONO, SINGLE, and PARAPHONIC all use the voices in unison.
    case Patch::VOICE_MONO:
    case Patch::VOICE_SINGLE:
    case Patch::VOICE_PARAPHONIC:

        // DUALMIDI Timbre uses each voice for a separate midi channel, so map accordingly.
        if (_timbre_mode == Patch::TIMBRE_DUALMIDI) {
            if (channel == 0)
                _v0.add_note(note, velocity);
            if (channel == 1)
                _v1.add_note(note, velocity);
        } else {
            _v0.add_note(note, velocity);
            _v1.add_note(note, velocity);
        }
        break;

    // Duophonic only allows two notes at a time, one per voice, so pick the voice that isn't being used,
    // or reuse a voice if both voices are active. Note this mode is NOT compatible with TIMBRE_DUALMIDI.
    case Patch::VOICE_DUOPHONIC:
        if (_v0.gate_status() == Envelope::GATE_OFF)
            _v0.add_note(note, velocity);
        else if (_v1.gate_status() == Envelope::GATE_OFF)
            _v1.add_note(note, velocity);
        else if (_v0.gate_status() == Envelope::GATE_RELEASE && _v1.gate_status() == Envelope::GATE_ON)
            _v0.add_note(note, velocity);
        else if (_v1.gate_status() == Envelope::GATE_RELEASE && _v0.gate_status() == Envelope::GATE_ON)
            _v1.add_note(note, velocity);
        else {
            //both notes are in RELEASE or ON. Override the oldest note.
            if (_v0.age() > _v1.age())
                _v0.add_note(note, velocity);
            else
                _v1.add_note(note, velocity);
        }
        break;
    }
}

void Synthesizer::_releaseNote(int note, int channel) {
    // DUALMIDI Timbre uses each voice for a separate midi channel, so release accordingly.
    if (_timbre_mode == Patch::TIMBRE_DUALMIDI) {
        if (channel == 0)
            _v0.release_note(note);
        if (channel == 1)
            _v1.release_note(note);
    } else {
        _v0.release_note(note);
        _v1.release_note(note);
    }
}

void Synthesizer::_setMod(int amount) {
    _v0.set_mod(double(amount)/16384);
    _v1.set_mod(double(amount)/16384);
}

// Callback for midi event. Triggers note adds, mod changes, etc.
// Note this is not synchronized to "update."
void Synthesizer::midiEvent(quint32 message, quint32 timing) {
    auto event = QMidiEvent();
    event.setMessage(message);

    QMidiEvent::EventType type = event.type();
    int note = event.note();
    int velocity = event.velocity();
    int amount = event.amount();
    int channel = event.voice();

    switch (type) {
    case QMidiEvent::EventType::NoteOn:
        _addNote(note, velocity, channel);
        qInfo("key pressed %d", note);
        break;

    case QMidiEvent::EventType::NoteOff:
        _releaseNote(note, channel);
        break;

    case QMidiEvent::EventType::PitchWheel:
        break;

    case QMidiEvent::EventType::ChannelPressure:
        break;

    case QMidiEvent::EventType::ControlChange:
        _setMod(amount);
        break;

    default:
        break;
    }
}
