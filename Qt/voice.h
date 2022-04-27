#ifndef VOICE_H
#define VOICE_H

#include <QObject>
#include <patch.h>
#include <pseudovoice.h>
#include <QTime>

class Voice : public QObject
{
    Q_OBJECT
public:
    explicit Voice(QObject *parent = nullptr);
    void update(QTime t);
    void reset_envelope();
    void clear_notes();
    void add_note(int note, int velocity);
    void release_note(int note);

signals:

private:
    Patch::OscShape shape0;
    Patch::OscShape shape1;
    double detune1;
    uint8_t octave1;
    PseudoVoice pvoices[6];
};

#endif // VOICE_H
