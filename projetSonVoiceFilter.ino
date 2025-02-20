#include <Audio.h>
#include "Filters.h"

Filters filters;
AudioInputI2S in;
AudioOutputI2S out;
AudioControlSGTL5000 audioShield;

AudioAnalyzeNoteFrequency notefreq;
AudioOutputAnalog         dac;
AudioMixer4               mixer;

// AudioConnection patchCord1(in,0,out,0);
// AudioConnection patchCord2(in,0,out,1);

AudioConnection patchCord0(in, 0, notefreq, 0);
AudioConnection patchCord2(filters, 0, out, 1);
AudioConnection patchCord3(filters, 0, out, 0);


// AudioConnection patchCord1(filters, 0, notefreq, 0);
//AudioConnection patchCord2(in, 0, out, 0);
//AudioConnection patchCord1(filters,0,out,1);

bool isDown;
bool isRelease;
int digitalReadConverted = 0;

float mtof(float note){
  return pow(2.0,(note-69.0)/12.0)*440.0;
}

void setup() {
  Serial.begin(9600);
  AudioMemory(30);
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.micGain(10); // in dB
  audioShield.volume(0.3);
  notefreq.begin(0.15);
}

void loop() {
  int digitalReadValue = digitalRead(0);
  int readyToListen = digitalRead(1);


  if (notefreq.available() && (readyToListen==0)) {
        Serial.println("available");
        float note = notefreq.read();
        float prob = notefreq.probability();
        //Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);
        filters.setParamValue("FreqHigh", note*25); //20 Permet de set la fréquence de coupure dans des intervalles intéressants
        //Serial.println(mtof(note));
        Serial.println(note*20);


    }

  

  
  if(digitalReadValue==1 && isDown==false) {
    isDown=true;
    digitalReadConverted++;
    if (digitalReadConverted > 2) {  
      digitalReadConverted = 0;
    }
  }
  else if(digitalReadValue== 0 && isDown==true) {
    isDown = false;
  }
  if (digitalReadConverted ==1) {
    filters.setParamValue("mode",0);
  }
  else if (digitalReadConverted == 2) {
    filters.setParamValue("mode",1);
  }
  else if (digitalReadConverted == 3) {
  }

  delay(50);
}