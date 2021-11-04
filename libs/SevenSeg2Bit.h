
//
// SevenSeg2Bit
//
// File: SevenSeg2Bit.h
//
// Creates a 7-segment 2-bit object for
// easier use
//

#pragma once
#include <Arduino.h>


class SevenSeg2Bit
{
private:
  // Pins
  int digit1, digit2;           // Enables each digit
  int a, b, c, d, e, f, g, dp;  // Segments

  // Each numeric output binary
  int out[16][7] =
  {
    {1,1,1,1,1,1,0},    // 0
    {0,1,1,0,0,0,0},    // 1
    {1,1,0,1,1,0,1},    // 2
    {1,1,1,1,0,0,1},    // 3
    {0,1,1,0,0,1,1},    // 4
    {1,0,1,1,0,1,1},    // 5
    {1,0,1,1,1,1,1},    // 6
    {1,1,1,0,0,0,0},    // 7
    {1,1,1,1,1,1,1},    // 8
    {1,1,1,1,0,1,1},    // 9
    {1,0,0,1,1,1,1},    // E - error
    {0,0,0,0,0,0,0},    // No output
  };
  int segPins[8];

  // Special display outputs
  int special[10][7] =
  {
    {0,1,1,0,0,0,1},    // + Left 
    {0,0,0,0,1,1,1},    // + Right
    {1,1,0,0,0,0,0},    // Reverse frame 1
    {0,0,1,1,0,0,0},    // Reverse frame 2
    {0,0,0,1,1,0,0},    // Reverse frame 3
    {1,0,0,0,0,1,0},    // Reverse frame 4
    {1,0,1,1,0,1,1},    // 'S' skip
    {1,1,0,0,1,1,1},    // 'P' skip
    {0,0,1,1,1,1,0},    // 'W' Left
    {0,1,1,1,1,0,0}     // 'W' Right
  };

  int delayTime;    // Time between digit switches

public:
  // Parameterized constructor
  // Initialize objects to pins and init private vars
  //
  SevenSeg2Bit(int digit1Pin, int digit2Pin, int a, int b, int c, int d, int e, int f, int g, int dp);

  // setDelay
  // Sets the delay time between setting each digit
  //
  void setDelay(int ms);

  // display
  // Displays the value on the 7-segment
  //
  void display(int value);

  // displayPlus
  // Displays a plus
  //
  void displayPlus();

  // displayReverse
  // Displays a reverse animation
  //
  void displayReverse(int frameDelay);

  // displaySkip
  // Displays a skip
  //
  void displaySkip();

  // displayW
  // Displays a 'W'
  //
  void displayW();

  // off
  // Turns off the display
  //
  void off();

  // test
  // tests the display
  //
  void test();

private:
  // output
  // Outputs the digits based on the inputs
  // val2 = digit2, val1 = digit1
  //
  void output(int val2, int val1);

};
