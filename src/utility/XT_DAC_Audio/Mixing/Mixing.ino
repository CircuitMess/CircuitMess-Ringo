// Demo's how to mix sounds together, these can be wavs' or wavs and musical scores
// or any items that can be played through the DacAudio object.
// Here we demo some wavs, the main one plays and then if you type in a "1" into the
// serial monitor then it will mix in another sound, enter a "2" and another 
// different wav will be mixed in
// See www.xtronical.com for write ups on sound and the hardare required

#include "SoundData.h";
#include "XT_DAC_Audio.h";

#define TIME_BETWEEN_MIXES 4000       // mix every 4 secs

XT_DAC_Audio_Class DacAudio(25,0);    // Set up DacAudio object, DAC on pn 25, using timer 0 for interrupts

XT_Wav_Class Cantina(CantinaWav);     // Create three WAV objects, passing in the WAV data
XT_Wav_Class Blaster(BlasterWav);
XT_Wav_Class ForceWithYou(ForceWav);

uint32_t LastTime;

void setup() {
  Cantina.RepeatForever=true;         // Play the main background musci constantly
  DacAudio.Play(&Cantina);            // Set it to play
  LastTime=millis();                  // Record initial time, samples will play every TIME_BETWEEN_MIXES
}

void loop() {
  DacAudio.FillBuffer();
  if(millis()>LastTime+TIME_BETWEEN_MIXES)    // Is it time to mix in a new sample?
  {    
    LastTime=millis();                        // Yes, record this new time ready for next time
    if(random(2)==1)
      DacAudio.Play(&ForceWithYou,true);      // the true means mix this sound with any currently playing and output together
    else
      DacAudio.Play(&Blaster,true);     
  }
}
