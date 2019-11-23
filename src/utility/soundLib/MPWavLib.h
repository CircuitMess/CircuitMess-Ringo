#ifndef wavLibDef
#define wavLibDef

#include <SD.h>
#include <FS.h>

#include "Notes.h"

//#define PI 3.14159265358979323846

#define SINE 0
#define TRIANGLE 1
#define SQUARE 2
#define SAW 3
#define NOISE 4


class MPTrack
{
public:   
    MPTrack(char path[]);
    MPTrack(const char path[]);
    ~MPTrack();
    void setVolume(float vol);
    void play();
    void resume();//Resume playing if not finished
    void stop();
    void pause();
    void rewind();
    void seek(float pos);
    unsigned int getSamplePos();
    bool isPlaying();
    int loadSample(unsigned short i);
    int fetchSample();
    bool loadSamples();
    bool isFinished();
    int getSize();
    void setSpeed(float s);
    float getSpeed();
    void setRepeat(bool r);
    bool openFile();
    void closeFile();
    bool reloadFile(char path[]);

private:
    File trackFile;
    uint8_t reload = 0;
    char trackPath[32];
    float volume=2;
    bool playing=false;
    bool finished=false;
    char data[800];
    unsigned int size;
    float speed=1;
    double pos=0;
    unsigned int loaded=0;
    uint32_t readBytes = 0;
    bool repeat=false;
};

class Oscillator
{
public:
    Oscillator();
    Oscillator(byte wave);
    ~Oscillator(){}
    void setWaveform(byte wave);
    void setFrequency(double freq);
    void setVolume(float vol);
    void play();
    void stop();
    bool isPlaying();
    float fetchSample();
    void beep(double freq, float len);
    void note(byte note, float len);
    void setADSR(float a, float d, float s, float r);
    void setDutyCycle(float d);
    void waitFor();
    void waitForNotes(bool w);

private:
    char waveform=0;
    double period=0.001;
    float volume=2, notelen, duty=0.5;
    bool playing=false, wait=false;
    byte envStep=0;
    double pos=0, envpos=0;
    float attack=0.01, decay=0.01, sustain=0.8, release=0.1;
};


#endif