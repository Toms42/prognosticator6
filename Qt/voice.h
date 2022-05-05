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
    qint64 age_ms();

signals:

private:
    double _shape0;
    double _shape1;
    double _detune1;
    uint8_t _octave1;
    PseudoVoice _pvoices[6];
    int _chorus_mode;
    Patch::VoiceMode _voice_mode;
    Envelope _filter_env;
};

#endif // VOICE_H
