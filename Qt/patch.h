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
        OSC_SIN,
    };

    enum LfoShape {
        LFO_SQUARE,
        LFO_TRI,
        LFO_SAW,
        LFO_SIN,
    };

    enum TimbreMode {
        TIMBRE_SINGLE_MONO,     // Single timbre, only one voice is used.
        TIMBRE_SINGLE_STEREO,   // Single timbre, but both voices are used. (One for left, one for right). Panpot, phase offset are allowed in this mode.
        TIMBRE_DUAL,            // Dual timbre. Each voice is used for a different timbre. mono.
        TIMBRE_DUALMIDI,        // Dual timbre, but each timbre is on a different midi channel rather than stacked.
    };

    enum VoiceMode {
        VOICE_SINGLE,
        VOICE_MONO,
        VOICE_PARAPHONIC,
        VOICE_DUOPHONIC,
    };

    enum ChorusMode {
        CHORUS_NONE,
        CHORUS_ON
    };

    enum ModSink {
        MOD_OSC_DETUNE,
        MOD_OSC_PITCH,
        MOD_FILTER_CUTOFF,
        MOD_FILTER_RESONANCE,
        MOD_FILTER_AMP,
        MOD_CHORUS_DEPTH,
        MOD_STEREOPAN,
        MOD_STEREOPHASE,

        NUM_MOD_SINKS
    };

    enum ModSource {
        MODSR_VEL,
        MOD_LFO0,
        MOD_LFO1,
        MOD_ENV0,
        MOD_ENV1,

        NUM_MOD_SOURCES
    };

    struct TimbreData {
        // Oscillators:
        OscShape osc_shape0;
        OscShape osc_shape1;
        float osc_detune;
        int osc_octave;

        // Voices
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
    };

    struct PatchData {
        // Timbres
        TimbreData timbre0;
        TimbreData timbre1;

        //
        VoiceMode voice_mode;

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
