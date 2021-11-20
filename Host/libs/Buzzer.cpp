//
// Buzzer
//
// File: Buzzer.cpp
//
// Compacts buzzer commands into a Buzzer object
//

#include "Buzzer.h"



// Parameterized object constructor
// Initialized with given pin
//
Buzzer::Buzzer(int pin) {
    this->pin = pin;
    pinMode(this->pin, OUTPUT);
}

// playTone
// Exactly the same as the original tone function
// Plays a tone
void Buzzer::playTone(int freq) {
    tone(pin, freq);
}

// off
// Turns off any playing tones
//
void Buzzer::off() {
    noTone(pin);
}

// playToneList
// Plays a predefined array of tones
//
void Buzzer::playToneList(String toneName) {
  // Match toneName to array
  int i=0;
  int match=-1;
  for (String searchName : toneNames) {
      if (searchName == toneName) {
          match = i;
      }
      i++;
  }
  if (match == -1)
    return;

  int toneAmount = toneCounts[match];
  for (int i=0; i<toneAmount; i++) {
      playTone(toneList[match][i].freq);    // Play note at frequency
      delay(toneList[match][i].len);        // Delay for 'len' ms
      off();                      // Stop tone
      delay(toneList[match][i].delayTime);  // Delay before next note     
  }
}

// playRange
// Plays given frequency range
// with given speed of playback
void Buzzer::playRange(int range1, int range2, int toneSpeed) {
    if (toneSpeed <= 0)
      return;

    int dir = 1;    // Direction adds or subtracts if range2 is lower than range1
    if (range1 > range2)
      dir = -1;

    if (dir == 1) {   // Range increases
        for (int currTone = range1; currTone <= range2; currTone += toneSpeed) {
            tone(pin, currTone);
            delay(10);
        }
    } else if (dir == -1) {
        for (int currTone = range1; currTone >= range2; currTone -= toneSpeed) {
            tone(pin, currTone);
            delay(10);
        }
    }
  
    off();    // Disable tone
}
