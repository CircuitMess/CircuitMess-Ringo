#include "Audio.h"
void Audio::begin()
{
	
	
	sfx = new AudioGeneratorWAV();
    // mp3 = new AudioGeneratorMP3();
	out = new AudioOutputI2S();
    mixer = new AudioOutputMixer(32, out);
	// out->SetOutputModeMono(1);
    
    
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
        mp3 = new AudioGeneratorMP3();
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
void Audio::playWAV(char * path)
{
    
    if(mp3running == 1)
    {
        Serial.println("Can't play mp3 and wav/sfx at the same time!");
        return;
    }
    if(wavrunning == 1)
        return;
    else if(wavrunning == 0)
    {
        out->begin();
        wavrunning = 1;
    }
       
    else if (mp3running == -1 && path != NULL)
    {

        // mp3->~AudioGeneratorMP3();
        wavStub = mixer->NewInput();
        wavStub->SetGain(0.3);
        wav = new AudioGeneratorWAV();
        wavFile = new AudioFileSourceSD(path);
        wav->begin(wavFile, wavStub);
        out->SetRate(8000);
        wavrunning = 1;
    }
}

void Audio::pauseWAV()
{
    if(wav->isRunning())
        out->stop();
    wavrunning = 0;
}
void Audio::stopWAV()
{
    if(wav->isRunning())
    {
        out->stop();
    }
    wav->~AudioGeneratorWAV();
    wavFile->~AudioFileSourceSD();
    wavrunning = -1;
}
void Audio::setVolumeWAV(uint8_t gain)
{
    out->SetGain((float)(gain/10));
}
void Audio::setLoopWAV(bool loop)
{
    wavloop = loop;
}
int8_t Audio::isRunningWAV()
{
    return wavrunning;
}