#include "MPWavLib.h"
#ifndef MPAudio
#define MPAudio

#define SAMPLE_RATE (44100)  //SAMPLE RATE

#define MAX_TRACKS 4

#define PIN_I2S_BCLK 26
#define PIN_I2S_LRC 25
#define PIN_I2S_DOUT 22

void initWavLib();
void setVolume(unsigned char vol);
bool addTrack(MPTrack *track);
bool removeTrack(MPTrack *track);
bool addOscillator(Oscillator *osc);
bool removeOscillator(Oscillator *osc);
void updateWav();

extern unsigned char volume;
extern MPTrack *tracks[MAX_TRACKS];
extern unsigned char trackCount;
extern char dout[1600];
extern short adcbuf[1600];
#endif
