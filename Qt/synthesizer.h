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
    void setPeriod(int ms) {_period=ms;};
signals:

public slots:
    void midiEvent(quint32 message, quint32 timing);
    void update();
    void updatePatch(Patch::PatchData &patch);
    void busyboi(void);

private:
    void _addNote(int note, int velocity, int channel);
    void _releaseNote(int note, int channel);

    timespec last;
    int _period=10;
    Voice _v0;
    Voice _v1;
    Patch::VoiceMode _voice_mode;
    Patch::TimbreMode _timbre_mode;
};

#endif // SYNTHESIZER_H
