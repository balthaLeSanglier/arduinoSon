#include <Audio.h>
#include "Filters.h"


Filters filters;
AudioInputI2S in;
AudioOutputI2S out;
AudioControlSGTL5000 audioShield;

AudioAnalyzeNoteFrequency notefreq;
AudioOutputAnalog         dac;
AudioMixer4               mixer;

AudioConnection patchCord0(in,0,out,0);
AudioConnection patchCord1(in,0,out,1);

//AudioConnection patchCord0(in, 0, notefreq, 0);
//AudioConnection patchCord2(in, 0, out, 0);
//AudioConnection patchCord1(filters,0,out,1);

bool isDown;
bool isRelease;
int digitalReadConverted = 0;

void setup() {
  Serial.begin(9600);
  AudioMemory(6);
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.micGain(10); // in dB
  audioShield.volume(1);
  notefreq.begin(.15);
}

void loop() {
  if (notefreq.available()) {
        float note = notefreq.read();
        float prob = notefreq.probability();
        Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);
    }

  int digitalReadValue = digitalRead(0);
  
  if(digitalReadValue==1 && isDown==false) {
    isDown=true;
    digitalReadConverted++;
    if (digitalReadConverted > 2) {  
      digitalReadConverted = 0;
    }
    Serial.print("State: ");
    Serial.println(digitalReadConverted); 
  }
  else if(digitalReadValue== 0 && isDown==true) {
    isDown = false;
  }

  if (digitalReadConverted ==1) {
    filters.setParamValue("mode",0);
    Serial.println(filters.getParamValue("mode"));  
  }
  
  else if (digitalReadConverted == 2) {
    filters.setParamValue("mode",1);
    Serial.println(filters.getParamValue("mode"));
  }
  else if (digitalReadConverted == 3) {
  }

  delay(50);

}

