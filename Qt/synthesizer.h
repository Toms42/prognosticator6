#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <QObject>
#include <QTimer>
#include <patch.h>
#include <voice.h>
#include <QMidiIn.h>
#include <QMidiFile.h>

class Synthesizer : public QObject
{
    Q_OBJECT
public:
    explicit Synthesizer(QObject *parent = nullptr);

signals:

public slots:
    void midiEvent(quint32 message, quint32 timing);
    void update();
    void updatePatch(Patch::PatchData &patch);

private:
    void _addNote(int note, int velocity, int channel);
    void _releaseNote(int note, int channel);

    QTimer _timer;
    Voice _v0;
    Voice _v1;
    Patch::VoiceMode _voice_mode;
    Patch::TimbreMode _timbre_mode;
};

#endif // SYNTHESIZER_H
