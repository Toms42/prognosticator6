#include "synthesizer.h"

Synthesizer::Synthesizer(QObject *parent)
    : QObject{parent}
{

}

void Synthesizer::run() {
    while(!isInterruptionRequested()) {

    }
}

void Synthesizer::update(void) {
    qint64 t = QDateTime::currentMSecsSinceEpoch();

    // Update both voices.
    // Envelopes should update, and we can
    _v0.update(t);
    _v1.update(t);
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
            if (_v0.age_ms() > _v1.age_ms())
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

    default:
        break;
    }
}

void Synthesizer::updatePatch(Patch::PatchData &patch) {
    _voice_mode = patch.voice_mode;
}
