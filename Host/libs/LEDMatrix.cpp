//
// LEDMatrix
//
// File: LEDMatrix.cpp
//
// Creates a LED Matrix object for
// easier use of predefined LED values
//
// Easily define new letter, symbols, and numbers by creating a new array
// and updating the letter/symbol counter variables
//

#include "LEDMatrix.h"


// Parameterized object constructor
//
LEDMatrix::LEDMatrix(int data, int clk, int cs) {
    // Create LedControl object
    matrix = new LedControl(data, clk, cs, 1);
    
    // Set defaults
    frameDelay = 100;
    intensity = 2;

    // Setup
    matrix->shutdown(0, false);          // Disable power off
    matrix->setIntensity(0,intensity);   // Set brightness
    matrix->clearDisplay(0);             // Clear display for next use
}

// test
// Tests the matrix by enabling all LEDs
//
void LEDMatrix::test() {
    for (int i=0; i<8;i++) {
        matrix->setColumn(0, i, allOn[i]);   // Display columns
    }
}

// setIntensity
// Sets the intensity to a value between 0-15
// Returns false if out of bounds
bool LEDMatrix::setIntensity(int value) {
    if (value < 0 || value > 15)
      return false;

    // Set value
    intensity = value;
    matrix->setIntensity(0, intensity);
    return true;
}

// display
// Displays the given type if it exists
//
bool LEDMatrix::display(String type) {
    // Loop through each type name
    int i=0;
    for (String typeName : typeNames) {
      if (typeName == type) {
        displayFrame(typeList[i]);
        return true;
      }
      i++;      
    }

    // Not found
    return false;
}

// displayScroll
// Displays a set of frames consecutively by scrolling, follows a specified
// delay time per frame
// Only functions with letters
void LEDMatrix::displayScroll(String text) {
    int letterCount = 0;   // Counts the displacement of added letters; begins off the screen

    // Build string
    byte* letterData[64];   // Max size is 64 letters
    
    int i;
    for (char c : text) {                             // Loop thru each char in input string
        i=0;
        for (char searchLetter : letterNames) {       // Search for letter in available chars
           if (c == searchLetter) {
              letterData[letterCount++] = letterList[i];
              break;
           }
           i++;
        }
    }

    // Begin output
    int currDisp = 8;                       // The current scroll displacement
    int maxDisp = letterCount * -8;         // The maximum displacement before the loop ends
    unsigned long prevTime = millis();      // U_long to prevent overflow

    while (currDisp > maxDisp) {
        // Display each letter
        for (int i=0; i<letterCount; i++)
          displayFramePos(letterData[i], currDisp + (8*i));     // Display letter + each previous letter's displacement

       
       if (millis() - prevTime >= frameDelay) {
          prevTime = millis();
          currDisp--;    // Decrement displacement
       }
    }
}

// setFrameDelay
// Sets the delay between frames on scrolling output
//
void LEDMatrix::setFrameDelay(int value) {
    frameDelay = value;
}


// clear
// Clears the screen
//
void LEDMatrix::clear() {
    matrix->clearDisplay(0);
}

// displayFrame
// Displays the value found from the given binary list
//
void LEDMatrix::displayFrame(byte binaryList[]) {
    for (int c=0; c<8; c++)
      matrix->setColumn(0,c, binaryList[c]);
}

// displayPos
// Displays the value found from the given binary list
// and given displacement
//
void LEDMatrix::displayFramePos(byte letter[], int disp) {
    if (disp <= -8 || disp >= 8)    // If outside range, do not render
      return;

    for (int c=0; c<8; c++) {       // Loop each column
      if (c + disp < 0 || c + disp > 7)     // If column is currently out of bounds, ignore it
        continue;
        
      matrix->setColumn(0, c+disp, letter[c]);   // Display columns
    }
}

// animate
// Animates a set of frames provided in the input string
// Separated by space
//
void LEDMatrix::animate(String frames, int delayTime) {
    int strLen = frames.length();
    if (delayTime < 0)
      delayTime = 0;
    
    // Parse string and output
    String strBuild;
    for(int i=0; i<strLen; i++) {
        if (frames[i] != ' ') {
            strBuild += frames[i];
        } else {
            // End of current string; output frame
            display(strBuild);
            delay(delayTime);
            strBuild = "";    // Reset strBuild
        }
    }

    // Display final strBuild
    display(strBuild);
    delay(delayTime);
    clear();
}
