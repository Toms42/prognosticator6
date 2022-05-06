#ifndef PATCH_H
#define PATCH_H

#include <QObject>
#include <QDir>
#include <QtNumeric>


class Patch : public QObject
{
    Q_OBJECT
public:
    explicit Patch(QDir patch_dir, QObject *parent = nullptr);

    enum LfoShape {
        LFO_SQUARE,
        LFO_TRI,
        LFO_SAW,
        LFO_SIN,
        LFO_RAND,
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

    enum ModSink {
        MOD_OSC_DETUNE,
        MOD_OSC_PITCH,
        MOD_FILTER_CUTOFF,
        MOD_FILTER_RESONANCE,
        MOD_AMP,
        MOD_CHORUS_DEPTH,
        MOD_STEREOPAN,
        MOD_STEREOPHASE,
        MOD_LFO0,
        MOD_LFO1,

        NUM_MOD_SINKS
    };

    enum ModSource {
        MODSRC_VEL,
        MODSRC_LFO0,
        MODSRC_LFO1,
        MODSRC_ENV0,
        MODSRC_ENV1,
        MODSRC_MOD,
        MODSRC_PB,

        NUM_MOD_SOURCES
    };

    enum ArpMode {
        ARP_OFF,
        ARP_UP,
        ARP_DOWN,
        ARP_UPDOWN,
        ARP_RAND,
    };

    enum ChordMode {
        CHORD_OFF,
        CHORD_MAJOR,
        CHORD_MINOR,
        CHORD_KEY3,
        CHORD_KEY4
    };

    struct TimbreData {
        // Oscillators:
        double osc_shape0;
        double osc_shape1;
        double osc_detune;
        int osc_octave;

        // Voices
        double voice_glide;
        int voice_chorus;

        // Filter:
        double filter_freq;
        double filter_resonance;

        // Mixer:
        double mixer_amount0;
        double mixer_amount1;

        // Envelope 0
        double env0_attack;
        double env0_decay;
        double env0_sustain;
        double env0_release;

        // Envelope 1
        double env1_attack;
        double env1_decay;
        double env1_sustain;
        double env1_release;
    };

    struct PatchData {
        // Voicing
        TimbreData timbre[2];
        VoiceMode voice_mode;
        TimbreMode timbre_mode;
        int activeTimbre;   // not really patch data but visualizers need this.

        // Midi
        ArpMode midi_arp_mode;
        int midi_arp_steps;
        ChordMode midi_chord_mode;
        int midi_key;

        // LFO 0
        double lfo0_freq;
        LfoShape lfo0_shape;

        // LFO 1
        double lfo1_freq;
        LfoShape lfo1_shape;

        // Mod Matrix
        qint8 mod_matrix[ModSource::NUM_MOD_SOURCES][ModSink::NUM_MOD_SINKS];
    };

    int activeTimbre() {return _p.activeTimbre;};

    void refresh() {_onChange(); _onReload();};

public slots:
    void save(uint8_t idx);
    void load(uint8_t idx);

    void updateVoiceMode(int m) {_p.voice_mode = VoiceMode(m); _onChange();};
    void updateTimbreMode(int m) {_p.timbre_mode = TimbreMode(m); _onChange();}
    void updateTimbre(int i) {_p.activeTimbre = i; _onReload(); _onChange();};

    void updateArpMode(int m) {_p.midi_arp_mode = ArpMode(m); _onChange();};
    void updateArpSteps(int m) {_p.midi_arp_steps = m; _onChange();};
    void updateChordMode(int m) {_p.midi_chord_mode = ChordMode(m); _onChange();};
    void updateKey(int m) {_p.midi_key = m; _onChange();};

    void updateLfoFreq0(double f) {_p.lfo0_freq = f; _onChange();};
    void updateLfoFreq1(double f) {_p.lfo1_freq = f; _onChange();};
    void updateLfoShape0(int s) {_p.lfo0_shape = LfoShape(s); _onChange();};
    void updateLfoShape1(int s) {_p.lfo1_shape = LfoShape(s); _onChange();};

    void updateOscShape0(double s) {_p.timbre[_p.activeTimbre].osc_shape0 = s; _onChange();};
    void updateOscShape1(double s) {_p.timbre[_p.activeTimbre].osc_shape1 = s; _onChange();};
    void updateOscDetune(double s) {_p.timbre[_p.activeTimbre].osc_detune = s; _onChange();};
    void updateOscOctave(int o) {_p.timbre[_p.activeTimbre].osc_octave = o; _onChange();};

    void updateVoiceGlide(double g) {_p.timbre[_p.activeTimbre].voice_glide = g; _onChange();};
    void updateChorus(int m) {_p.timbre[_p.activeTimbre].voice_chorus = m; _onChange();};

    void updateFilterFreq(double f) {_p.timbre[_p.activeTimbre].filter_freq = f; _onChange();};
    void updateFilterResonance(double r) {_p.timbre[_p.activeTimbre].filter_resonance = r; _onChange();};

    void updateMixer0(double a) {_p.timbre[_p.activeTimbre].mixer_amount0 = a/100.; _onChange();};
    void updateMixer1(double a) {_p.timbre[_p.activeTimbre].mixer_amount1 = a/100.; _onChange();};

    void updateEnv0Attack(double s) {_p.timbre[_p.activeTimbre].env0_attack = s; _onChange();};
    void updateEnv0Decay(double s) {_p.timbre[_p.activeTimbre].env0_decay = s; _onChange();};
    void updateEnv0Sustain(double s) {_p.timbre[_p.activeTimbre].env0_sustain = s/100.; _onChange();};
    void updateEnv0Release(double s) {_p.timbre[_p.activeTimbre].env0_release = s; _onChange();};

    void updateEnv1Attack(double s) {_p.timbre[_p.activeTimbre].env1_attack = s; _onChange();};
    void updateEnv1Decay(double s) {_p.timbre[_p.activeTimbre].env1_decay = s; _onChange();};
    void updateEnv1Sustain(double s) {_p.timbre[_p.activeTimbre].env1_sustain = s/100.; _onChange();};
    void updateEnv1Release(double s) {_p.timbre[_p.activeTimbre].env1_release = s; _onChange();};

    // mod matrix
    void updateModCell(Patch::ModSource source, Patch::ModSink sink, qint8 amount) {
        _p.mod_matrix[source][sink] = amount;
        _onChange();
    }

    // special patch load/save stuff
    void patchButtonClicked();
    void patchButtonLongClicked();
    void patchButtonValueChanged(int n);
private:
    void _onChange() {
        emit updated(_p);
    };
    void _onReload() {
        emit reloaded(_p);
        emit updated(_p);
    }

    uint8_t current_idx;
    uint8_t hover_idx;
    QDir folder;
    Patch::PatchData _p;

    bool saveActive = false;
    bool loadActive = false;
    bool yesSelected = false;
    int patchButtonValue = 0;
    int lastPatchButtonValue = 0;

signals:
    void updated(const Patch::PatchData &data);
    void reloaded(const Patch::PatchData &data);
    void patchSavingState(int current_idx, int hover_idx, bool saveActive, bool loadActive, bool yesSelected);
};

#endif // PATCH_H
