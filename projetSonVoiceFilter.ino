#include <Audio.h>
#include "Filters.h"


Filters filters;
AudioOutputI2S out;
AudioControlSGTL5000 audioShield;
AudioConnection patchCord0(filters,0,out,0);
AudioConnection patchCord1(filters,0,out,1);


void setup() {
  AudioMemory(2);
  audioShield.enable();
  audioShield.volume(0.2);
}

void loop() {
  filters.setParamValue("mode",1);
  Serial.println(filters.getParamValue("mode"));
  delay(1000);
  filters.setParamValue("mode",0);
  Serial.println(filters.getParamValue("mode"));
  delay(500);

}

