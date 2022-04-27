#include "synthesizer.h"

Synthesizer::Synthesizer(QObject *parent)
    : QObject{parent}
{

}

void Synthesizer::update(void) {
    auto t = QTime::currentTime();

    // Update both voices.
    // Envelopes should update, and we can
    _v0.update(t);
    _v1.update(t);
}

void Synthesizer::_addNote(int note, int velocity) {
    switch (_voice_mode) {
    case Patch::VoiceMode::VOICE_SINGLE:
        _v0.reset_envelope();
        _v1.reset_envelope();
        _v0.clear_notes();
        _v1.clear_notes();
        break;
    case Patch::VoiceMode::VOICE_MONO:
        break;
    case Patch::VoiceMode::VOICE_PARAPHONIC:
        break;
    case Patch::VoiceMode::VOICE_DUOPHONIC:
        break;
    }
}

void Synthesizer::_removeNote(int note) {

}

void Synthesizer::midiEvent(quint32 message, quint32 timing) {
    auto event = QMidiEvent();
    event.setMessage(message);

    QMidiEvent::EventType type = event.type();
    int note = event.note();
    int velocity = event.velocity();
    int amount = event.amount();

    switch (type) {
    case QMidiEvent::EventType::NoteOn:
        _addNote(note, velocity);

    case QMidiEvent::EventType::NoteOff:
        break;

    case QMidiEvent::EventType::PitchWheel:
        break;

    case QMidiEvent::EventType::ChannelPressure:
        break;

    default:
        break;
    }
}
