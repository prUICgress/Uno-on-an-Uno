//
// LEDMatrix
//
// File: LEDMatrix.h
//
// Creates a LED Matrix object for
// easier use of predefined LED values
//
// Easily define new letter, symbols, and numbers by creating a new array
// and updating the letter/symbol counter variables
//
#pragma once

#include <LedControl.h>



class LEDMatrix {
private:
  LedControl* matrix = nullptr;        // pins data, clk, cs, and 1 device  (from LedControl.h)
  int frameDelay;                      // Time between frames; used when scrolling text
  int intensity;                       // Intensity of the matrix  -- Default: 2

  // Binary values for LED output
  // Binary values are formatted for SETCOLUMN
  //
  // Tests
  byte allOn[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
  // Arrows
  byte leftArrow[8] = {B00011000,B00111100,B01111110,B11111111,B00011000,B00011000,B00011000,B00011000};
  byte downArrow[8] = {B00001000,B00001100,B00001110,B11111111,B11111111,B00001110,B00001100,B00001000};
  byte rightArrow[8] = {B00011000,B00011000,B00011000,B00011000,B11111111,B01111110,B00111100,B00011000};
  byte upArrow[8] = {B00010000,B00110000,B01110000,B11111111,B11111111,B01110000,B00110000,B00010000};
  // Arrow heads
  byte upArrowHead[8] = {B00011000,B00111000,B01111000,B11111000,B11111000,B01111000,B00111000,B00011000};
  byte downArrowHead[8] = {B00011000,B00011100,B00011110,B00011111,B00011111,B00011110,B00011100,B00011000};
  byte leftArrowHead[8] = {B00011000,B00111100,B01111110,B11111111,B11111111,B00000000,B00000000,B00000000};
  byte rightArrowHead[8] = {B00000000,B00000000,B00000000,B11111111,B11111111,B01111110,B00111100,B00011000};
  // Cards
  byte reverse_Frame1[8] = {B11000000,B11000000,B11000000,B11000000,B11010000,B11011000,B11111100,B11111110};
  byte reverse_Frame2[8] = {B00000000,B00000001,B00000011,B00000111,B00001111,B00000011,B11111111,B11111111};
  byte reverse_Frame3[8] = {B01111111,B00111111,B00011011,B00001011,B00000011,B00000011,B00000011,B00000011};
  byte reverse_Frame4[8] = {B11111111,B11111111,B11000000,B11110000,B11100000,B11000000,B10000000,B00000000};
  //
  byte reverse2_Frame1[8] = {B11111110,B11111100,B11011000,B11010000,B11000000,B11000000,B11000000,B11000000};
  byte reverse2_Frame2[8] = {B11111111,B11111111,B00000011,B00001111,B00000111,B00000011,B00000001,B00000000};
  byte reverse2_Frame3[8] = {B00000011,B00000011,B00000011,B00000011,B00001011,B00011011,B00111111,B01111111};
  byte reverse2_Frame4[8] = {B00000000,B10000000,B11000000,B11100000,B11110000,B11000000,B11111111,B11111111};
  //
  byte skip[8] = {B11111111,B11000111,B10001111,B10011101,B10111001,B11110001,B11100011,B11111111};
  // Letters
  byte letterU[8] = {B00000000,B11111111,B11111111,B00000011,B00000011,B11111111,B11111111,B00000000};
  byte letterN[8] = {B00000000,B11111111,B11111111,B00111000,B00011100,B11111111,B11111111,B00000000};
  byte letterO[8] = {B00000000,B11111111,B11111111,B11000011,B11000011,B11111111,B11111111,B00000000};
  byte letterP[8] = {B00000000,B11111111,B11111111,B11001100,B11001100,B11111100,B11111100,B00000000};
  byte letterW[8] = {B11111100,B11111111,B00000111,B11111111,B11111111,B00000111,B11111111,B11111100};
  byte letterI[8] = {B00000000,B11000011,B11000011,B11111111,B11111111,B11000011,B11000011,B00000000};
  byte letterS[8] = {B00000000,B11111011,B11111011,B11011011,B11011011,B11011111,B11011111,B00000000};
  byte letterSpace[8] = {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};
  // Numbers
  byte number1[8] = {B00000000,B00110011,B01110011,B11111111,B11111111,B00000011,B00000011,B00000000};
  byte number2[8] = {B00000000,B11011111,B11011111,B11011011,B11011011,B11111011,B11111011,B00000000};
  byte number3[8] = {B00000000,B11011011,B11011011,B11011011,B11011011,B11111111,B11111111,B00000000};
  byte number4[8] = {B00000000,B11111000,B11111000,B00011000,B00011000,B11111111,B11111111,B00000000};
  byte number5[8] = {B00000000,B11111011,B11111011,B11011011,B11011011,B11011111,B11011111,B00000000};
  byte number6[8] = {B00000000,B11111111,B11111111,B11011011,B11011011,B11011111,B11011111,B00000000};
  byte number7[8] = {B00000000,B11000000,B11000000,B11000000,B11000000,B11111111,B11111111,B00000000};
  byte number8[8] = {B00000000,B11111111,B11111111,B11011011,B11011011,B11111111,B11111111,B00000000};
  byte number9[8] = {B00000000,B11111000,B11111000,B11011000,B11011000,B11111111,B11111111,B00000000};
  byte number0[8] = {B00000000,B11111111,B11111111,B11000011,B11000011,B11111111,B11111111,B00000000};
  // Symbols
  byte letterPlus[8] = {B00011000,B00011000,B00011000,B11111111,B11111111,B00011000,B00011000,B00011000};

  // Number of typable names
  #define typeNamesCount 36
  // Number of chars (scrollable) (all are scrollable if they are given a char symbol)
  #define letterNamesCount 19

  // String names for each binary frame along with the corresponding binary lists with same index
  String typeNames[typeNamesCount] = {"leftArrow","rightArrow","upArrow","downArrow","leftArrowHead","rightArrowHead","upArrowHead","downArrowHead","letterU","letterN","letterO","letterP","number1","number2","number3","number4","letterW","letterI","letterS","letterSpace","reverse_Frame1","reverse_Frame2","reverse_Frame3","reverse_Frame4","reverse2_Frame1","reverse2_Frame2","reverse2_Frame3","reverse2_Frame4","skip","plus","number5","number6","number7","number8","number9","number0"};
  char letterNames[letterNamesCount] = {'U','N','O','P','1','2','3','4','W','I','S',' ','+','5','6','7','8','9','0'};
  byte* typeList[typeNamesCount] = {leftArrow,rightArrow,upArrow,downArrow,leftArrowHead,rightArrowHead,upArrowHead,downArrowHead,letterU,letterN,letterO,letterP,number1,number2,number3,number4,letterW,letterI,letterS,letterSpace,reverse_Frame1,reverse_Frame2,reverse_Frame3,reverse_Frame4,reverse2_Frame1,reverse2_Frame2,reverse2_Frame3,reverse2_Frame4,skip,letterPlus,number5,number6,number7,number8,number9,number0};
  byte* letterList[letterNamesCount] = {letterU,letterN,letterO,letterP,number1,number2,number3,number4,letterW,letterI,letterS,letterSpace,letterPlus,number5,number6,number7,number8,number9,number0};

public:
  // Parameterized object constructor
  //
  LEDMatrix(int data, int clk, int cs);

  // test
  // Tests the matrix by enabling all LEDs
  //
  void test();
  
  // setIntensity
  // Sets the intensity to a value between 0-15
  // Returns false if out of bounds
  //
  bool setIntensity(int value);

  // display
  // Displays the given type if it exists
  //
  bool display(String type);

  // displayScroll
  // Displays a set of frames consecutively by scrolling, follows a specified
  // delay time per frame
  // Only functions with letters
  //
  void displayScroll(String text);

  // setFrameDelay
  // Sets the delay between frames on scrolling output
  //
  void setFrameDelay(int value);

  // clear
  // Clears the screen
  //
  void clear();

  // animate
  // Animates a set of frames provided in the input string
  // Separated by space
  // Delays for 'delayTime' ms between frames
  //
  void animate(String frames, int delayTime);

private:
  // display
  // Displays the value found from the given predefined binary list
  //
  void displayFrame(byte binaryList[]);

  // displayPos
  // Displays the value found from the given predefined binary list
  // and given displacement
  //
  void displayFramePos(byte letter[], int disp);
};
