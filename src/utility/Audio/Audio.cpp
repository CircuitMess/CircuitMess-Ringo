#include "Audio.h"
void Audio::begin()
{
	mixer = new AudioOutputMixer(4096, out);
	stub[0] = mixer->NewInput();
	stub[0]->SetGain(0.3);
	wav = new AudioGeneratorWAV();
	stub[1] = mixer->NewInput();
	stub[1]->SetGain(0.4);
	sfx = new AudioGeneratorWAV();
    mp3 = new AudioGeneratorMP3();
	out = new AudioOutputI2S();
	out->SetOutputModeMono(1);
    
    
}

void Audio::playMP3(char * path)
{
    if(sfxrunning == 1 || wavrunning == 1)
    {
        Serial.println("Can't play mp3 and wav/sfx at the same time!");
        return;
    }
    if(mp3running == 0)
    {
        out->begin();
        mp3running = 1;
    }
       
    else if (mp3running == -1 && path != NULL)
    {
        mixer->~AudioOutputMixer();
        mp3File = new AudioFileSourceSD(path);
        mp3Buff = new AudioFileSourceBuffer(mp3File, 4096);
        mp3->begin(mp3File, out);
        out->SetRate(44100);
        mp3running = 1;
    }
}

void Audio::pauseMP3()
{
    if(mp3->isRunning())
        out->stop();
    mp3running = 0;
}
void Audio::stopMP3()
{
    if(mp3->isRunning())
    {
        out->stop();
    }
    mp3->~AudioGeneratorMP3();
    mp3File->~AudioFileSourceSD();
    mp3running = -1;
}
void Audio::setVolumeMP3(uint8_t gain)
{
    out->SetGain((float)(gain/10));
}
void Audio::setLoopMP3(bool loop)
{
    mp3loop = loop;
}
int8_t Audio::isRunningMP3()
{
    return mp3running;
}