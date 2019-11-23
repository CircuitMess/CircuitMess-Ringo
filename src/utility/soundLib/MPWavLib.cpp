#include "MPWavLib.h"
#include "MPAudioDriver.h"

MPTrack::MPTrack(char path[])
{
    strcpy(trackPath,path);
}
MPTrack::MPTrack(const char path[])
{
    strcpy(trackPath,path);
}

MPTrack::~MPTrack()
{
    
}

bool MPTrack::openFile()
{
    trackFile=SD.open(trackPath);
    if(trackFile)
    {
        size=trackFile.size();
        trackFile.seek(0x2C);
        pos=0;
        return true;
    }
    else
        return false;
}

void MPTrack::closeFile()
{
    trackFile.close();
}

void MPTrack::setVolume(float vol)
{
    if(vol>300)
        vol=300;
    if(vol<0)
        vol=0;
    volume=vol;
}

bool MPTrack::isFinished()
{
    return finished;
}

void MPTrack::play()
{
    // const TickType_t xDelay = 1 / portTICK_PERIOD_MS;
    // uint16_t stepsNumber = 200;
    // float step = volume / stepsNumber;
    // volume = 0;
    // vTaskDelay(xDelay);

    // for(uint16_t i = 0; i < stepsNumber;i++)
    // {
    //     vTaskDelay(xDelay);
    //     volume += step;
    // }
    // volume = step*stepsNumber;
    if(finished)
        rewind();
    playing=true;
    finished=false;
}

void MPTrack::stop()
{
    // uint16_t stepsNumber = 200;
    // const TickType_t xDelay = 1 / portTICK_PERIOD_MS;
    // float step = volume / stepsNumber;
    // vTaskDelay(xDelay);

    // for(uint16_t i = 0; i < stepsNumber;i++)s
    // {
    //     vTaskDelay(xDelay);
    //     volume -= step;
    // }
    // volume = step*stepsNumber;
    playing=false;
    finished=false;
    trackFile.seek(0x2C);
    loaded=0;
    pos=0;
}

void MPTrack::pause()
{
    playing=false;
}

void MPTrack::rewind()
{ 
    trackFile.seek(0x2C);
    finished=false;
    loaded=0;
    pos=0;
}

void MPTrack::seek(float tpos)
{
    if(tpos<0)
        tpos=0;
    pos=tpos*SAMPLE_RATE;
    tpos*=SAMPLE_RATE*2;
    tpos+=0x2C;
    if(tpos>size)
    {
        rewind();
        finished=true;
        playing=false;
    }
    else
    {
        trackFile.seek(tpos);
        loadSamples();
        finished=false;
    }     
}

bool MPTrack::isPlaying()
{
    return playing;
}

void MPTrack::resume()
{
    if(!finished)
        playing=true;
}

unsigned int MPTrack::getSamplePos()
{
    return trackFile.position()-0x2C;
}

int MPTrack::loadSample(unsigned short i)
{
    int sample;
    sample = *((short*)(data + i));
    sample*=volume;
    return sample;
}

int MPTrack::fetchSample()
{
    if(digitalRead(33) && !_SDinterruptError)
        return 0;
    int p, sample;
    if(!playing)
        return 0;

    if(pos>=loaded)
        loadSamples();
    
    p=((int)pos)%(sizeof(data)>>1);
    sample = *((short*)(data +(p << 1)));
    sample*=volume;
    pos+=speed;
    if(pos>=((size-0x2C)>>1))
    {
        // float temp = volume;
        // volume = 0;
        if(repeat)   
            rewind();
        else
        {
            loaded=0;
            finished=true;
            playing=false;
        }
        // updateWav();
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
        // volume = temp;
    }
    return sample;
}
uint32_t failCounter = 0;
bool MPTrack::loadSamples()
{
   
    reload=trackFile.readBytes(data, sizeof(data));

    if(trackFile.position()>this->size||reload==0)
    {
        if(digitalRead(33) && !_SDinterruptError)
            return 0;
        while(reloadCo)
            vTaskDelay(100);
        reloadCo++;
        Serial.println("BRICK");
        do
        {
            failCounter++;
            trackFile.close();
            if(!(trackFile = SD.open(this->trackPath, "r")))
            {
                SD.end();           
                //reload=reloadCo;
                vTaskDelay(10);
                do
                {
                    if(digitalRead(33) && !_SDinterruptError)
                        return 0;
                    vTaskDelay(10);
                    Serial.println("errorb");

                }while(!SD.begin(5, SPI, 8000000));
                
                while(!(trackFile = SD.open(this->trackPath, "r")))
                {
                    vTaskDelay(10);
                    Serial.println("error");
                }
            }
            trackFile.seek(readBytes);
            Serial.println(failCounter);
        
        } while (!trackFile.readBytes(data, sizeof(data)));
        reloadCo--;
    }
    readBytes = trackFile.position();
    // Serial.println(read);
    loaded=(readBytes-0x2C)/2;
    return true;
}

int MPTrack::getSize()
{
    return size;
}

void MPTrack::setSpeed(float s)
{
    if(s>5)
        s=5;
    if(s<0.1)
        s=0.1;
    speed=s;
}

float MPTrack::getSpeed()
{
    return speed;
}

void MPTrack::setRepeat(bool r)
{
    repeat=r;
}

bool MPTrack::reloadFile(char path[])
{
    playing=false;
    finished=false;
    strcpy(trackPath,path);
    trackFile.close();
     while(!SD.begin(5, SPI, 8000000))
         Serial.println("SD ERROR");
    trackFile=SD.open(trackPath);
    if(trackFile)
    {
        size=trackFile.size();
        trackFile.seek(0x2C);
        loaded=0;
        pos=0;
        playing=false;
        return true;
	}
	else
		return false;
}

// ------------------------- OSC --------------------------------

Oscillator::Oscillator()
{
    waveform=SINE;
    period=1.0/1000.0;
}

Oscillator::Oscillator(byte wave)
{
    waveform=wave;
    period=1.0/1000.0;
}

void Oscillator::setWaveform(byte wave)
{
    waveform=wave;
}

void Oscillator::setFrequency(double freq)
{
    if(freq<1)
        freq=1;
    if(freq>SAMPLE_RATE/2)
        freq=SAMPLE_RATE/2;
    period=1.0/freq;
}

void Oscillator::setVolume(float vol)
{
    if(vol>128)
        vol=128;
    if(vol<0)
        vol=0;
    volume=vol;
}

void Oscillator::play()
{
    playing=true;
}

void Oscillator::stop()
{
    playing=false;
}

bool Oscillator::isPlaying()
{
    return playing;
}

double fast_sin(double x) //From Orson Peters, github
{
	const double INVPI =  0.31830988618379067153776752674502872;
	const double A	 =  0.00735246819687011731341356165096815;
	const double B	 = -0.16528911397014738207016302002888890;
	const double C	 =  0.99969198629596757779830113868360584;
	int32_t k;
	double x2;
	k = round(INVPI * x);
	x -= k * PI;
	x2 = x * x;
	x = x*(C + x2*(B + A*x2));
	if (k % 2) x = -x;
	return x;
}

float Oscillator::fetchSample()
{
    float vol;
    vol=volume;
    pos+=(1.0/SAMPLE_RATE)/period;
    if(pos>=1)
        pos=0;

    switch(envStep)//Envelope generator
    {
        case 1:
            vol*=envpos/attack;
            envpos+=(1.0/SAMPLE_RATE);
            if(envpos>=attack)
            {
                envStep++;
                envpos=0;
            }
            break;
        case 2:
            vol*=1.0-((envpos/decay)*(1.0-sustain));
            envpos+=(1.0/SAMPLE_RATE);
            if(envpos>=decay)
            {
                envStep++;
                envpos=0;
            }
            break;
        case 3:
            vol*=sustain;
            envpos+=(1.0/SAMPLE_RATE);
            if(envpos>=notelen)
            {
                envStep++;
                envpos=0;
            }
            break;
        case 4:
            vol*=sustain-(sustain*(envpos/release));
            envpos+=(1.0/SAMPLE_RATE);
            if(envpos>=release)
            {
                envStep=0;
                envpos=0;
                playing=false;
            }
            break;
        case 0:
            break;
    }

    switch(waveform)
    {
        case 0:
            return fast_sin(2*PI*pos)*vol;//Sine
        case 1:
            if(pos<0.5)
                return (pos-0.25)*2*vol;//Triangle
            return (0.75-pos)*2*vol;
        case 2:
            if(pos>duty)
                return vol/2.0;//Square
            return 0-vol/2.0;            
        case 3:
            return (pos-0.5)*2*vol;//Saw
        case 4:
            return ((((float)esp_random()/(float)UINT32_MAX)*2.0)-1.0)*vol;//Noise
        default:
            return 0;
    }
    
}


void Oscillator::beep(double freq, float len)
{
    envpos=0;
    period=1.0/freq;
    envStep=1;
    notelen=len;
    playing=true;    
}

void Oscillator::note(byte note, float len)
{
    beep(pow(2,(note-69)/12.0)*440.0, len);
    if(wait)
        waitFor();
}

void Oscillator::setADSR(float a, float d, float s, float r)
{
    if(a<0)a=0;
    if(d<0)d=0;
    if(s<0)s=0;
    if(r<0)r=0;
    if(a>10000)a=10000;
    if(d>10000)d=10000;
    if(s>1)s=1;
    if(r>10000)r=10000;
    attack=a/1000.0;
    decay=d/1000.0;
    sustain=s;
    release=r/1000.0;
}

void Oscillator::setDutyCycle(float d)
{
    if(d>0.999)
        d=0.999;
    if(d<0.001)
        d=0.001;
    duty=d;
}

void Oscillator::waitFor()
{
    while(playing&&envStep)
        vTaskDelay(1);
}

void Oscillator::waitForNotes(bool w)
{
    wait=w;
}
