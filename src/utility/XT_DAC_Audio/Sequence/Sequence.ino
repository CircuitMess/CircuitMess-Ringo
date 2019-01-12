// Demo's how to play multiple sounds one after the other, whether those sounds are WAVs
// or Music Score's or a mixture, here we demo a mixture, a wav followed by a music score.
// wav score will play twice (once plus 1 repeat) then the music score once
// and the entire sequence will repeat once, check the use of repeats below to do this

// See www.xtronical.com for write ups on sound and the hardare required

#include "MusicDefinitions.h";
#include "SoundData.h";
#include "XT_DAC_Audio.h";

// Data for the melody. Note followed by optional change in playing length in 1/4 beats. See documentation for more details
int8_t PROGMEM TwinkleTwinkle[] = {
  NOTE_C5,NOTE_C5,NOTE_G5,NOTE_G5,NOTE_A5,NOTE_A5,NOTE_G5,BEAT_2,
  NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,NOTE_D5,NOTE_C5,BEAT_2,
  NOTE_G5,NOTE_G5,NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,BEAT_2,
  NOTE_G5,NOTE_G5,NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,BEAT_2,
  NOTE_C5,NOTE_C5,NOTE_G5,NOTE_G5,NOTE_A5,NOTE_A5,NOTE_G5,BEAT_2,
  NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,NOTE_D5,NOTE_C5,BEAT_4,  
  NOTE_SILENCE,BEAT_5,SCORE_END
};
                                      
XT_DAC_Audio_Class DacAudio(25,0);          // Create the main player class object. Use GPIO 25, one of the 2 DAC pins and timer 0

XT_Wav_Class ForceWithYou(Force);           // create WAV object and pass in the WAV data
                                      
XT_MusicScore_Class Music(TwinkleTwinkle,TEMPO_ALLEGRO,INSTRUMENT_PIANO);  // The music score object, pass in the Music data

XT_Sequence_Class Sequence;                // The sequence object, you add your sounds above to this object (see setup below)


void setup() {
  Sequence.AddPlayItem(&ForceWithYou);      // Add the first sound item, this will play first
  Sequence.AddPlayItem(&Music);             // Add the music score, this will play after the first item
  DacAudio.Play(&Sequence);                 // Play the sequence, will play just the once and then stop
}


void loop() {
  DacAudio.FillBuffer();          // This needs only be in your main loop once, suggest here at the top.
  
  // put whatever code you want here that you would normally have on your loop
}
