#include "Audio.h"
void Audio::begin()
{
	
	
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
        mp3->stop();
        out->stop();
    }
    mp3->~AudioGeneratorMP3();
    mp3Buff->~AudioFileSourceBuffer();
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

        
        wav = new AudioGeneratorWAV();
        wavFile = new AudioFileSourceSD(path);
        wavBuff = new AudioFileSourceBuffer(wavFile, 4056);
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
    if(wavrunning)
    {
        wav->stop();
        wavStub->stop();
    }
    wav->~AudioGeneratorWAV();
    wavFile->~AudioFileSourceSD();
    wavStub->~AudioOutputMixerStub();
    wavBuff->~AudioFileSourceBuffer();
    wavrunning = -1;
}
void Audio::setVolumeWAV(uint8_t gain)
{
    wavStub->SetGain((float)(gain/10));
}
void Audio::setLoopWAV(bool loop)
{
    wavloop = loop;
}
int8_t Audio::isRunningWAV()
{
    return wavrunning;
}

void Audio::playSFX(const unsigned char *path, uint32_t len)
{
    if(mp3running == 1)
    {
        Serial.println("Can't play mp3 and wav/sfx at the same time!");
        return;
    }
    if(sfxrunning == 1 && path == NULL)
        return;
    else if(sfxrunning == 0) 
    {
        out->begin();
        sfxrunning = 1;
    }
    
    if (mp3running == -1 && path != NULL)
    {
        if(sfxrunning == 1)
            stopSFX();

        // mp3->~AudioGeneratorMP3();
        
        sfx = new AudioGeneratorWAV();
        sfxFile = new AudioFileSourcePROGMEM(path, len);
        sfxBuff = new AudioFileSourceBuffer(sfxFile, 10000);
        sfx->begin(sfxFile, out);
        sfxStub->SetRate(8000);
        sfxrunning = 1;
    }
}
void Audio::pauseSFX()
{
    if(sfx->isRunning())
        out->stop();
    sfxrunning = 0;
}
void Audio::stopSFX()
{
    if(sfxrunning == 1)
    {
        sfxStub->stop();
        sfx->stop();
    }
    
    sfx->~AudioGeneratorWAV();
    sfxFile->~AudioFileSourcePROGMEM();
    sfxBuff->~AudioFileSourceBuffer();
    sfxrunning = -1;
}
void Audio::setVolumeSFX(uint8_t gain)
{
    sfxStub->SetGain((float)(gain/10));
}
void Audio::setLoopSFX(bool loop)
{
    sfxloop = loop;
}
int8_t Audio::isRunningSFX()
{
    return wavrunning;
}