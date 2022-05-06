#ifndef STATE_H
#define STATE_H

#include <stdlib.h>
#include <stdint.h>

struct OscillatorState {
    uint16_t freq; // 1/500th of a semitone
    uint8_t phase; // 0-255
    uint8_t amp;   // 0-255
    uint8_t shape; //0-255
};

struct DualOscillatorState {
    OscillatorState s0;
    OscillatorState s1;
};

struct FilterState {
    double cutoff;
    double resonance;
};

struct VoiceState {
    FilterState f;
    OscillatorState s0;
    OscillatorState s1;
    OscillatorState s2;
    OscillatorState s3;
    OscillatorState s4;
    OscillatorState s5;
    OscillatorState s6;
    OscillatorState s7;
    OscillatorState s8;
    OscillatorState s9;
    OscillatorState s10;
    OscillatorState s11;
};

struct SynthState {
    VoiceState v0;
    VoiceState v1;
    bool add;
    double volume;
};

#endif // STATE_H
