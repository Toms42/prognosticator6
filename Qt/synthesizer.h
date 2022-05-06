#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <QObject>
#include <QTimer>
#include <patch.h>
#include <voice.h>
#include <QMidiIn.h>
#include <QMidiFile.h>
#include <QThread>

class Synthesizer : public QThread
{
    Q_OBJECT
public:
    explicit Synthesizer(QObject *parent = nullptr);
    void run() override;
    void connectPatch(const Patch *patch) {
        connect(patch, &Patch::updated, this, &Synthesizer::updatePatch);
        _v0.connectPatch(patch);
        _v1.connectPatch(patch);
    }

signals:

public slots:
    void midiEvent(quint32 message, quint32 timing);
    void update();
    void updatePatch(Patch::PatchData patch) {
        _voice_mode = patch.voice_mode;
        _timbre_mode = patch.timbre_mode;
    }
    void busyboi(void);

private:
    void _addNote(int note, int velocity, int channel);
    void _releaseNote(int note, int channel);
    void _setMod(int amount);

    timespec last;
    int _period_us=500E3;
    Voice _v0;
    Voice _v1;
    Patch::VoiceMode _voice_mode;
    Patch::TimbreMode _timbre_mode;
};

#endif // SYNTHESIZER_H
