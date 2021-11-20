//
// LCD
//
// File: LCD.cpp
//
// Allows for easy use of the lcd
//

#pragma once

#include <LiquidCrystal.h>
#include <Arduino.h>




class LCD {
private:
  LiquidCrystal* lcd = nullptr;

public:
  // Default constructor
  // Default values are equal to the pins from the LCD tutorial
  //
  LCD();

  // Parameterized object constructor
  // Default values are equal to the pins from the LCD tutorial
  //
  LCD(int rs, int en, int d4, int d5, int d6, int d7);

  // clear
  // Clears the LCD
  //
  void clear();

  // display
  // Displays the string, wraps to second line on overflow
  //
  void display(String text);

  // displayLines
  // Displays both lines separately
  // Cuts off extra 
  void displayLines(String line1, String line2);

  // displayCenter
  // Displays each line at the center of the screen
  //
  void displayCenter(String line1, String line2);
};
