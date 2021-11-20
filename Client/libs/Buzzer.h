//
// Buzzer
//
// File: Buzzer.h
//
// Compacts buzzer commands into a Buzzer object
//

#pragma once

#include <Arduino.h>




class Buzzer {
private:
  int pin;

  // A single note
  // Contains the frequency, note length, and delay before the next note
  // Also stores the max notes in the list
  struct note {
      int freq;       // The pitch of the note
      int len;        // The length of the note
      int delayTime;  // The time between this note and the next
  };

  // List of tone arrays
  note unoTone[2] = {{1000,200,0},{750,150,0}};
  note invalidTone[2] = {{400,200,50},{400,200,0}};
  note winTone[18] = {{0,100,0},{100,100,0},{200,100,0},{300,100,0},{400,100,0},{500,100,0},{600,100,0},{700,100,0},{800,100,0},{900,100,0},{1000,100,0},{1100,100,0},{1200,100,0},{1300,100,0},{1400,100,0},{1500,350,50},{1500,100,50},{1500,100,0}};
  note playCard[1] = {{500,300,0}};
  note skip[2] = {{300,200,0},{200,150,0}};
  note drawCard[2] = {{750,300,0},{900,100,0}};

  // List of tones
  #define toneCount 6   // Update this value when adding a tone
  String toneNames[toneCount] = {"unoTone","invalidTone","winTone","playCard","skip","drawCard"};
  int toneCounts[toneCount] =   {2,2,18,1,2,2};
  note* toneList[toneCount] =   {unoTone,invalidTone,winTone,playCard,skip,drawCard};
  

public:
  // Parameterized object constructor
  // Initialized with given pin
  //
  Buzzer(int pin);

  // playTone
  // Exactly the same as the original tone function
  // Plays a tone
  void playTone(int freq);

  // off
  // Turns off any playing tones
  //
  void off();

  // playToneList
  // Plays a predefined array of tones
  // Uses a separate array of delay times between tones
  //
  void playToneList(String toneName);

  // playRange
  // Plays given frequency range
  // with given speed of playback
  void playRange(int range1, int range2, int toneSpeed);
};
