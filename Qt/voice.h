#ifndef VOICE_H
#define VOICE_H

#include <QObject>
#include <patch.h>
#include <pseudovoice.h>
#include <QTime>
#include <envelope.h>

class Voice : public QObject
{
    Q_OBJECT
public:
    explicit Voice(QObject *parent = nullptr);
    void update(qint64 t_ms);
    void add_note(int note, int velocity);
    Envelope::GateStatus gate_status();
    void release_note(int note);
    uint32_t age_ms();

signals:

private:
    Patch::OscShape _shape0;
    Patch::OscShape _shape1;
    double _detune1;
    uint8_t _octave1;
    PseudoVoice _pvoices[6];
    Patch::ChorusMode _chorus_mode;
    Patch::VoiceMode _voice_mode;
    Envelope _filter_env;
};

#endif // VOICE_H
