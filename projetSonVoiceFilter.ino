#include <Audio.h>
#include "Filters.h"


Filters filters;
AudioOutputI2S out;
AudioControlSGTL5000 audioShield;
AudioConnection patchCord0(filters,0,out,0);
AudioConnection patchCord1(filters,0,out,1);
bool isDown;
bool isRelease;
int digitalReadConverted = 0;

void setup() {
  pinMode(0, INPUT);
  AudioMemory(2);
  audioShield.enable();
  audioShield.volume(0.2);
}

void loop() {
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

