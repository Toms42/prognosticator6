#ifndef KNOBS
#define KNOBS
// encoder i2c addresses 
//
// **LEFT SIDE**
// oscillator
// 	-shape0 0x10
// 	-shape1 0x11
// 	-detune 0x12
// 	-octave 0x13
// voices
// 	-mode 0x14
// 	-glide 0x15
// 	-chorus 0x16
// mixer
// 	-mix0 0x17
// 	-mix1 0x18
// midi_effects
// 	-arpegio 0x19
// 	-chord 0x1A
// control
// 	-matrix 0x1B
// 	-patch 0x1C
//
// **RIGHT SIDE**
// filter
// 	-cutoff 0x1D
// 	-resonance 0x1E
// prognosis
// 	-volume 0x1F
// lfo_0
// 	-freq 0x20
// 	-shape 0x21
// lfo_1
// 	-freq 0x22
// 	-shape 0x23
// envelope_0
// 	-attack 0x24
// 	-decay 0x25
// 	-sustain 0x26
// 	-release 0x27
// envelope_1
// 	-attack 0x28
// 	-decay 0x29
// 	-sustain 0x2A
// 	-release 0x2B

// run it once at startup, returns 0 if happy
int openBus ();

// run it once for each encoder. addy from 0x10 to 0x2B
int configEncoder (int addy);

int wasPushed (int addy);

int wasLongPushed (int addy);

// returns encoder val
int getVal (int addy);

#endif
