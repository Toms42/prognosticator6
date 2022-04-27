#ifndef PATCH_H
#define PATCH_H

#include <QObject>
#include <QDir>

class Patch : public QObject
{
    Q_OBJECT
public:
    explicit Patch(QDir patch_dir, QObject *parent = nullptr);

    enum OscShape {
        OSC_SQUARE,
        OSC_TRI,
        OSC_SAW,
        OSC_SIN
    };

    enum LfoShape {
        LFO_SQUARE,
        LFO_TRI,
        LFO_SAW,
        LFO_SIN
    };

    enum VoiceMode {
        VOICE_SINGLE,
        VOICE_MONO,
        VOICE_PARAPHONIC,
        VOICE_DUOPHONIC
    };

    enum ChorusMode {
        CHORUS_NONE,
        CHORUS_ON
    };

    enum ModSink {
        MOD_OSC_SHAPE0,
        MOD_OSC_SHAPE1,
        MOD_OSC_DETUNE,
        MOD_OSC_PITCH,
        MOD_FILTER_CUTOFF,
        MOD_FILTER_RESONANCE,
        MOD_FILTER_AMP,
        MOD_CHORUS_DEPTH,

        NUM_MOD_SINKS
    };

    enum ModSource {
        MODSR_VEL,
        MOD_LFO0,
        MOD_LFO1,
        MOD_ENV1,
        MOD_ENV2,
        MOD_ENV3,

        NUM_MOD_SOURCES
    };

    struct PatchData {
        // Oscillators:
        OscShape osc_shape0;
        OscShape osc_shape1;
        float osc_detune;
        int osc_octave;

        // Voices
        VoiceMode voice_mode;
        float voice_glide;
        ChorusMode voice_chorus;

        // Filter:
        float filter_freq;
        float filter_resonance;

        // Mixer:
        float mixer_amount0;
        float mixer_amount1;

        // Envelope 0
        float env0_attack;
        float env0_decay;
        float env0_sustain;
        float env0_release;

        // Envelope 1
        float env1_attack1;
        float env1_decay1;
        float env1_sustain1;
        float env1_release1;

        // LFO 0
        float lfo0_freq;
        LfoShape lfo0_shape;

        // LFO 1
        float lfo1_freq;
        LfoShape lfo1_shape;

        // Mod Matrix
        int8_t mod_matrix[ModSource::NUM_MOD_SOURCES][ModSink::NUM_MOD_SINKS];
    };

public slots:
    void save(uint16_t idx);
    void load(uint16_t idx);

private:
    uint16_t current_idx;
    QDir folder;
    Patch::PatchData data;

signals:
    void updated(Patch::PatchData data);
};

#endif // PATCH_H
