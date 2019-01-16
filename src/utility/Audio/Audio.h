#include "ESP8266Audio/src/AudioFileSource.h"
#include "ESP8266Audio/src/AudioGeneratorMP3.h"
#include "ESP8266Audio/src/AudioFileSourceID3.h"
#include "ESP8266Audio/src/AudioOutputI2S.h"
#include "ESP8266Audio/src/AudioFileSourceSD.h"
#include "ESP8266Audio/src/AudioFileSourceBuffer.h"
#include "ESP8266Audio/src/AudioGeneratorWAV.h"
#include "ESP8266Audio/src/AudioOutputMixer.h"
#include "ESP8266Audio/src/AudioFileSourcePROGMEM.h"

class Audio 
{
    public:
        friend class MAKERphone;
        uint8_t volume = 5; // 1 to 10
        
        void begin();

        //MP3 functions
        void playMP3(char * path = NULL);
        void pauseMP3();
        void stopMP3();
        void setVolumeMP3(uint8_t gain);
        void setLoopMP3(bool loop = 1);
        int8_t isRunningMP3();

        //WAV functions
        void playWAV(char *path);
        void pauseWAV();
        void stopWAV();
        void setVolumeWAV(uint8_t gain);
        void setLoopWAV(bool loop = 1);
        bool isRunningWAV();

        //SFX functions
        void playSFX(char *path);
        void pauseSFX();
        void stopSFX();
        void setVolumeSFX(uint8_t gain);
        void setLoopSFX(bool loop = 1);
        bool isRunningSFX();

    protected:
        AudioGeneratorWAV *wav;
        AudioGeneratorWAV *sfx;
        AudioGeneratorMP3 *mp3;
        bool mp3loop = 0;
        bool wavloop = 0;
        bool sfxloop = 0;
        int8_t mp3running = -1;
        int8_t wavrunning = -1;
        int8_t sfxrunning = -1;
        AudioFileSourcePROGMEM *sfxFile;
        AudioFileSourceBuffer *mp3Buff;
        AudioFileSourceSD *wavFile;
        AudioFileSourceSD *mp3File;
        AudioOutputI2S *out;
        AudioOutputMixer *mixer;
        AudioOutputMixerStub *stub[2];


};