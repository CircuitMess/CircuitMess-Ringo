#include "MPWavLib.h"
#include "MPAudioDriver.h"
#include <Arduino.h>
#include <SPI.h>
#include "driver/i2s.h"
// #include "driver/adc.h"
// #include "esp_adc_cal.h"

float masterVolume;
MPTrack *tracks[MAX_TRACKS] = {NULL, NULL, NULL, NULL};
Oscillator *oscs[MAX_TRACKS] = {NULL, NULL, NULL, NULL};
unsigned char trackCount;
char dout[1600];
// short adcbuf[1600]={0};
// short adcWr=0, adcRd=800;
uint8_t reloadCo = 0;
hw_timer_t * timert = NULL;
bool _SDinterruptError = 0;


// void IRAM_ATTR onTimer() {
//   adc1_get_raw(ADC1_CHANNEL_5); 
// }

void I2S_Init() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),//| I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = 0,
    .dma_buf_count = 16,
    .dma_buf_len = 60,
    .use_apll = 1,
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = PIN_I2S_BCLK,
    .ws_io_num = PIN_I2S_LRC,
    .data_out_num = PIN_I2S_DOUT,
    .data_in_num = 33
  };
  i2s_driver_install((i2s_port_t)0, &i2s_config, 0, NULL);
  //i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_5);
  //i2s_adc_enable((i2s_port_t)0);
  //init ADC pad
	 //i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_0);
  i2s_set_pin((i2s_port_t)0, &pin_config);
  i2s_set_clk((i2s_port_t)0, SAMPLE_RATE, (i2s_bits_per_sample_t)32, I2S_CHANNEL_MONO);
}

void I2S_Write(char* data, int numData) {
    size_t bytes_written;
    (void)i2s_write((i2s_port_t)0, (const char *)data, numData, &bytes_written, portMAX_DELAY);
}

// #define ADC1_TEST_CHANNEL (ADC1_CHANNEL_5)
// char i2s_read_buff[1600];
// void adc_read_task(void* arg)
// {
//    size_t bytes_read, bytes_written;
   
//    while (1)
//     i2s_read((i2s_port_t)0, (void*) i2s_read_buff, 1600, &bytes_read, portMAX_DELAY);
// }

void initWavLib()
{
    masterVolume=300;
    trackCount=0;
    for(unsigned char i=0;i<MAX_TRACKS;i++)
        tracks[i]=nullptr;
    pinMode(PIN_I2S_BCLK,OUTPUT);
    pinMode(PIN_I2S_LRC,OUTPUT);
    pinMode(PIN_I2S_DOUT,OUTPUT);
    digitalWrite(PIN_I2S_LRC,0);
    I2S_Init();

    // adc1_config_width(ADC_WIDTH_BIT_12);
    // adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_0db); ADC1 channel 5 is GPIO33 (neopixels)

    //xTaskCreate(adc_read_task, "adc_read_task", 1024 * 2, NULL, 5, NULL);
}

void updateWav()
{
    //size_t bytes_read, bytes_written;
    for(unsigned short i=0;i<sizeof(dout)/2;i+=2)
    {
        *((int*)(dout + i * 2)) = 0;
        //(*((int*)(&dout[i*2])))=((*((int*)(&i2s_read_buff[i*2])))*2)/3;
        for(unsigned char tr=0;tr<MAX_TRACKS;tr++)
        {
            if(tracks[tr] != nullptr)
                if(tracks[tr]->isPlaying())
                    *((int*)(dout + i * 2)) += (int)(tracks[tr]->fetchSample()*masterVolume);
       
            if(oscs[tr] != nullptr)
                if(oscs[tr]->isPlaying())
                    *((int*)(dout + i * 2)) += (int)(oscs[tr]->fetchSample()*masterVolume)*65536;
        }
    }
    I2S_Write(dout, sizeof(dout));
    // i2s_read((i2s_port_t)0, (void*) i2s_read_buff, 1600, &bytes_read, portMAX_DELAY);
    vTaskDelay(1);
}

bool addTrack(MPTrack *track)//returns if the track is added successfully
{
    bool err;
    for(unsigned char i=0;i<MAX_TRACKS;i++)
        if(tracks[i] == nullptr)
        {
            err=track->openFile();
            if(!err)
                return false;
            tracks[i]=track;
            return true;
        }

    return false;
}

bool removeTrack(MPTrack *track)//returns if the track is removed successfully
{
    for(unsigned char i=0;i<MAX_TRACKS;i++)
        if(tracks[i] == track)
        {
            track->closeFile();
            tracks[i]=nullptr;
            return true;
        }
    return false;
}

bool addOscillator(Oscillator *osc)//returns if the oscillator is added successfully
{
    for(unsigned char i=0;i<MAX_TRACKS;i++)
        if(oscs[i] == nullptr)
        {
            oscs[i]=osc;
            return true;
        }

    return false;
}

bool removeOscillator(Oscillator *osc)//returns if the oscillator is removed successfully
{
    for(unsigned char i=0;i<MAX_TRACKS;i++)
        if(oscs[i] == osc)
        {
            oscs[i]=nullptr;
            return true;
        }
    return false;
}

void setVolume(float vol)
{
    if(vol<0)
        vol=0;
    if(vol>1000)
        vol=1000;
    masterVolume=vol;
}
