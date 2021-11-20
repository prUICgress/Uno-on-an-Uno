//
// LCD
//
// File: LCD.cpp
//
// Allows for easy use of the lcd
//

#include "LCD.h"



// Default constructor
// Default values are equal to the pins from the LCD tutorial
//
LCD::LCD() {
    lcd = new LiquidCrystal(12,11,5,4,3,2);     // Create LC object
    lcd->begin(16,2);                           // Init LCD
}

// Parameterized object constructor
// Creates an lcd object
//
LCD::LCD(int rs, int en, int d4, int d5, int d6, int d7) {
    lcd = new LiquidCrystal(rs,en,d4,d5,d6,d7);     // Create LC object
    lcd->begin(16,2);                               // Init LCD
}

// clear
// Clears the LCD
//
void LCD::clear() {
    lcd->clear();
}

// display
// Displays the string, wraps to second line on overflow
//
void LCD::display(String text) {
    int strLen = text.length();

    if (strLen <= 16) {
      lcd->setCursor(0,0);
      lcd->print(text);
    } else {
      String l1 = text.substring(0,16);
      String l2 = text.substring(16,32);
      lcd->setCursor(0,0);
      lcd->print(l1);
      lcd->setCursor(0,1);
      lcd->print(l2);
    }
}

// displayLines
// Displays both lines separately
// Cuts off extra 
void LCD::displayLines(String line1, String line2) {
    lcd->setCursor(0,0);
    lcd->print(line1.substring(0,16));
    lcd->setCursor(0,1);
    lcd->print(line2.substring(0,16));
}

// displayCenter
// Displays each line at the center of the screen
//
void LCD::displayCenter(String line1, String line2) {
    int strLenL1 = line1.length();
    int strLenL2 = line2.length();

    // Check for >16 length of string
    if (strLenL1 > 16) {
        line1 = line1.substring((strLenL1-16)/2, 16 + (strLenL1-16)/2);
    }
    if (strLenL2 > 16) {
        line2 = line2.substring((strLenL2-16)/2, 16 + (strLenL2-16)/2);
    }

    // Build line1
    String out1;
    for (int space=0; space < (16 - strLenL1)/2; space++)
        out1 += " ";
    out1 += line1;

    // Build line2
    String out2;
    for (int space=0; space < (16 - strLenL2)/2; space++)
        out2 += " ";
    out2 += line2;

    // Output
    lcd->setCursor(0,0);
    lcd->print(out1);
    lcd->setCursor(0,1);
    lcd->print(out2);
}
