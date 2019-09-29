#include "MPWavLib.h"
#ifndef MPAudio
#define MPAudio

#define SAMPLE_RATE (44100)  //SAMPLE RATE

#define MAX_TRACKS 4

#define PIN_I2S_BCLK 13
#define PIN_I2S_LRC 15
#define PIN_I2S_DOUT 22

void initWavLib();
void setVolume(float vol);
bool addTrack(MPTrack *track);
bool removeTrack(MPTrack *track);
bool addOscillator(Oscillator *osc);
bool removeOscillator(Oscillator *osc);
void updateWav();


extern unsigned char volume;
extern MPTrack *tracks[MAX_TRACKS];
extern Oscillator *oscs[MAX_TRACKS];
extern unsigned char trackCount;
extern char dout[1600];
// extern short adcbuf[1600];
extern uint8_t reloadCo;
extern bool _SDinterruptError;
#endif
