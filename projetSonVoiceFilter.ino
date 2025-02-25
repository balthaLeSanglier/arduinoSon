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

float fToP(float frequency){
  return ((frequency-250)/15);
}

void setup() {
  Serial.begin(9600);
  AudioMemory(60);
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.micGain(10); // in dB
  audioShield.volume(0.7);
  notefreq.begin(0.15);
}

void loop() {
  int digitalReadValue = digitalRead(0);
  int readyToListen = digitalRead(1);


  if (notefreq.available() && (readyToListen==1)) {
        Serial.println("available");
        float note = notefreq.read();
        float prob = notefreq.probability();
        //Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);
        filters.setParamValue("FreqHigh", note*5); //20 Permet de set la fréquence de coupure dans des intervalles intéressants
        filters.setParamValue("shift", fToP(note)); //20 Permet de set la fréquence de coupure dans des intervalles intéressants
        Serial.println(fToP(note));
    }



  if(digitalReadValue==1 && isDown==false) {
    isDown=true;
    digitalReadConverted++;
    if (digitalReadConverted >= 3) {  
      digitalReadConverted = 0;
    }
  }<
  else if(digitalReadValue== 0 && isDown==true) {
    isDown = false;
  }
  if (digitalReadConverted ==1) {
    filters.setParamValue("mode",0);
    Serial.println("Passe-haut");
  }
  else if (digitalReadConverted == 2) {
    filters.setParamValue("mode",1);
    Serial.println("Passe-bas");

  }
  else if (digitalReadConverted == 0) {
    filters.setParamValue("mode",2);
    Serial.println("Pitch-Shifter");
  }

  delay(50);
}