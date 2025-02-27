#include <Audio.h>
#include "Filters.h"

Filters filters;
AudioInputI2S in;
AudioOutputI2S out;
AudioControlSGTL5000 audioShield;
AudioAnalyzeFFT1024    myFFTin;
//AudioAnalyzeNoteFrequency notefreqIn;  // Fréquence d'entrée
//AudioAnalyzeNoteFrequency notefreqOut; // Fréquence de sortie

AudioAnalyzeNoteFrequency notefreq;
AudioOutputAnalog         dac;
AudioMixer4               mixer;

// AudioConnection patchCord1(in,0,out,0);
// AudioConnection patchCord2(in,0,out,1);


//AudioConnection patchCord1(in, 0, notefreqIn, 0);   // Entrée brute -> notefreqIn
//AudioConnection patchCord4(filters, 0, notefreqOut, 0); // Sortie filtre -> notefreqOut
AudioConnection patchCord0(in, 0, notefreq, 0);
AudioConnection patchCord2(filters, 0, out, 1);
AudioConnection patchCord3(filters, 0, out, 0);
AudioConnection patchCord1(filters, 0, myFFTin, 0);


// AudioConnection patchCord1(filters, 0, notefreq, 0);
//AudioConnection patchCord2(in, 0, out, 0);
//AudioConnection patchCord1(filters,0,out,1);

bool isDown;
bool isRelease;
int digitalReadConverted = 0;

float scale = 60.0;
float level[16];

float fToP(float frequency){
  return ((frequency-250)/15);
}

void setup() {
  Serial.begin(9600);
  AudioMemory(120);
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.micGain(10); // in dB
  audioShield.volume(0.7);
  notefreq.begin(0.15);
  //notefreqIn.begin(0.15);  // Initialiser les deux analyseurs
  //notefreqOut.begin(0.15);
}
void loop() {
  int digitalReadValue = digitalRead(0);
  int readyToListen = digitalRead(1);
  static float inputFreq = 0.0f;
  static float outputFreq = 0.0f;
  float n;
  int i;
  if (myFFTin.available()) {
    // Lecture des niveaux de fréquence
    level[0] =  myFFTin.read(0);
    level[1] =  myFFTin.read(1);
    level[2] =  myFFTin.read(2, 3);
    level[3] =  myFFTin.read(4, 6);
    level[4] =  myFFTin.read(7, 10);
    level[5] =  myFFTin.read(11, 15);
    level[6] =  myFFTin.read(16, 22);
    level[7] =  myFFTin.read(23, 32);
    level[8] =  myFFTin.read(33, 46);
    level[9] =  myFFTin.read(47, 66);
    level[10] = myFFTin.read(67, 93);
    level[11] = myFFTin.read(94, 131);
    level[12] = myFFTin.read(132, 184);
    level[13] = myFFTin.read(185, 257);
    level[14] = myFFTin.read(258, 359);
    level[15] = myFFTin.read(360, 511);

    // Envoi des données au Serial Plotter
    // Serial.print(0); // To freeze the lower limit
    // Serial.print(3); // To freeze the lower limit
    // for (int i=0; i<16; i++) {
    //   Serial.print(i);
    //   Serial.print(":");
    //   Serial.print(level[i] * scale);
    //   Serial.print("\t");
    //   Serial.println("");
    // }
//     Serial.print("0Hz:"); Serial.print(level[0]);
// Serial.print("\t");
// Serial.print("43Hz:"); Serial.print(level[1]);
// // Serial.print("\t");
// Serial.print("86-129Hz:"); Serial.print(level[2]);
// Serial.print("\t");
// // Serial.print("172-258Hz:"); Serial.print(level[3]);
// // Serial.print("\t");
// // Serial.print("301-430Hz:"); Serial.print(level[4]);
// // Serial.print("\t");
// // Serial.print("473-645Hz:"); Serial.print(level[5]);
// // Serial.print("\t");
// Serial.print("688-946Hz:"); Serial.print(level[6]);
// Serial.print("\t");
// // Serial.print("989-1378Hz:"); Serial.print(level[7]);
// // Serial.print("\t");
// // Serial.print("1421-1978Hz:"); Serial.print(level[8]);
// // Serial.print("\t");
// // Serial.print("2021-2842Hz:"); Serial.print(level[9]);
// // Serial.print("\t");
// // Serial.print("2885-4000Hz:"); Serial.print(level[10]);
// // Serial.print("\t");
// // Serial.print("4043-5642Hz:"); Serial.print(level[11]);
// // Serial.print("\t");
// Serial.print("5685-7922Hz:"); Serial.print(level[12]);
// // Serial.print("\t");
// // Serial.print("7965-11063Hz:"); Serial.print(level[13]);
// // Serial.print("\t");
// // Serial.print("11106-15455Hz:"); Serial.print(level[14]);
// // Serial.print("\t");
// // Serial.print("15498-22050Hz:"); Serial.print(level[15]);
// Serial.println(); // Nouvelle ligne après chaque affichage

// Calcul des niveaux pour les 3 courbes
float bass = level[0] + level[1] + level[2] + level[3] + level[4]; // 0 - 430 Hz
float mids = level[5] + level[6] + level[7] + level[8] + level[9] + level[10]; // 473 - 4000 Hz
float highs = level[11] + level[12] + level[13] + level[14] + level[15]; // 4043 - 22050 Hz

// Affichage sur le moniteur série
Serial.print("Bass:"); Serial.print(bass);
Serial.print("\t");
Serial.print("Mids:"); Serial.print(mids);
Serial.print("\t");
Serial.print("Highs:"); Serial.print(highs);
Serial.println(); // Nouvelle ligne pour chaque cycle d'affichage


  }
  // if (notefreqIn.available()) {
  //   inputFreq = notefreqIn.read();
  // }
  
  // if (notefreqOut.available()) {
  //   outputFreq = notefreqOut.read();
  // }

  if (notefreq.available() && (readyToListen == 1)) {
    float note = notefreq.read(); // Mise à jour de la variable
    float prob = notefreq.probability();
    filters.setParamValue("FreqHigh", note * 5);
    // Serial.println(note*5);
    filters.setParamValue("shift", fToP(note));
  }

  // Partie affichage
  // if (myFFTin.available()) {
  //   // each time new FFT data is available
  //   // print it all to the Arduino Serial Monitor
  //   // Serial.print("FFT: ");
  //   for (i=0; i<40; i++) {
  //     n = myFFTin.read(i);
  //     if (n >= 0.01) {
  //     //   Serial.print(n);
  //     //   Serial.print(" ");
  //     // } else {
  //     //   Serial.print("  -  "); // don't print "0.00"
  //     }
  //   }
  //   Serial.println();
  // }

  if(digitalReadValue==1 && isDown==false) {
    isDown=true;
    digitalReadConverted++;
    if (digitalReadConverted >= 3) {  
      digitalReadConverted = 0;
    }
  }
  else if(digitalReadValue== 0 && isDown==true) {
    isDown = false;
  }
  if (digitalReadConverted ==1) {
    filters.setParamValue("mode",0);
    // Serial.println("Passe-haut");
  }
  else if (digitalReadConverted == 2) {
    filters.setParamValue("mode",1);
    // Serial.println("Passe-bas");

  }
  else if (digitalReadConverted == 0) {
    filters.setParamValue("mode",2);
    // Serial.println("Pitch-Shifter");
  }

  delay(150);
}
