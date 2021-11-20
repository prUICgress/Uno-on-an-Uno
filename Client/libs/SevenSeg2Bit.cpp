//
// SevenSeg2Bit
//
// File: SevenSeg2Bit.cpp
//
// Creates a 7-segment 2-bit object for
// easier use
//

#include "SevenSeg2Bit.h"


// Parameterized constructor
// Initialize objects to pins and init private vars
//
SevenSeg2Bit::SevenSeg2Bit(int digit1Pin, int digit2Pin, int a, int b, int c, int d, int e, int f, int g, int dp) {
    // Set pins
    digit1 = digit1Pin;
    digit2 = digit2Pin;
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
    this->e = e;
    this->f = f;
    this->g = g;
    if (dp != -1)
      this->dp = dp;

    // Setup segPins
    segPins[0] = a;
    segPins[1] = b;
    segPins[2] = c;
    segPins[3] = d;
    segPins[4] = e;
    segPins[5] = f;
    segPins[6] = g;
    segPins[7] = dp;


    // Init pins
    pinMode(digit1, OUTPUT);
    pinMode(digit2, OUTPUT);
    for (int pin : segPins)
      pinMode(pin, OUTPUT);

    // Set delay
    delayTime = 5;
}

// setDelay
// Sets the delay time between setting each digit
//
void SevenSeg2Bit::setDelay(int ms) {
    delayTime = ms;
}

// display
// Displays the value on the 7-segment
//
void SevenSeg2Bit::display(int value) {
    if (value < 0)
      output(10,10);    // Display EE when underflowing
    else if (value > 99)
      output(9,9);      // Display 99 when overflowing
    else {
      output( value/10 , value%10 );
    }
}

// off
// Turns off the display
//
void SevenSeg2Bit::off() {
  output(-1,-1);
}

// displayPlus
// Displays a plus
//
void SevenSeg2Bit::displayPlus() {
    // Display right digit
    digitalWrite(digit1, 1);    // Right digit
    digitalWrite(digit2, 0);
  
    for (int i=0; i<7; i++)
      digitalWrite(segPins[i], special[1][i]);

    delay(delayTime);

    // Display left digit
    digitalWrite(digit1, 0);    // Right digit
    digitalWrite(digit2, 1);
  
    for (int i=0; i<7; i++)
      digitalWrite(segPins[i], special[0][i]);

    delay(delayTime);
}

// displayReverse
// Displays a reverse animation
//
void SevenSeg2Bit::displayReverse(int frameDelay) {
    // Display right digit
    digitalWrite(digit1, 1);    // Right digit
    digitalWrite(digit2, 0);
  
    for (int i=0; i<7; i++)
      digitalWrite(segPins[i], special[2][i]);
    delay(frameDelay);
    for (int i=0; i<7; i++)
      digitalWrite(segPins[i], special[3][i]);
    delay(frameDelay);

    // Display left digit
    digitalWrite(digit1, 0);    // Right digit
    digitalWrite(digit2, 1);
  
    for (int i=0; i<7; i++)
      digitalWrite(segPins[i], special[4][i]);
    delay(frameDelay);
    for (int i=0; i<7; i++)
      digitalWrite(segPins[i], special[5][i]);
    delay(frameDelay);
}

// displaySkip
// Displays a skip
//
void SevenSeg2Bit::displaySkip() {
    // Display right digit
    digitalWrite(digit1, 1);    // Right digit
    digitalWrite(digit2, 0);
  
    for (int i=0; i<7; i++)
      digitalWrite(segPins[i], special[7][i]);

    delay(delayTime);

    // Display left digit
    digitalWrite(digit1, 0);    // Right digit
    digitalWrite(digit2, 1);
  
    for (int i=0; i<7; i++)
      digitalWrite(segPins[i], special[6][i]);

    delay(delayTime);
}

// displayW
// Displays a 'W'
//
void SevenSeg2Bit::displayW() {
    // Display right digit
    digitalWrite(digit1, 1);    // Right digit
    digitalWrite(digit2, 0);
  
    for (int i=0; i<7; i++)
      digitalWrite(segPins[i], special[9][i]);

    delay(delayTime);

    // Display left digit
    digitalWrite(digit1, 0);    // Right digit
    digitalWrite(digit2, 1);
  
    for (int i=0; i<7; i++)
      digitalWrite(segPins[i], special[8][i]);

    delay(delayTime);
}

// output
// Outputs the digits based on the inputs
// val2 = digit2, val1 = digit1
//
void SevenSeg2Bit::output(int val2, int val1) {
    if (val1 == -1 && val2 == -1) {
      digitalWrite(digit1, 0);
      digitalWrite(digit2, 0);
      return;
    }

    // Display digit2
    digitalWrite(digit1, 0);
    digitalWrite(digit2, 1);

    if (val2 < 0 || val2 > 9) {
      int i=0;
      for (int segBit : out[10])
        digitalWrite(segPins[i++], segBit);         // Output error
    }

    int i=0;
    for (int segBit : out[val2])
      digitalWrite(segPins[i++], segBit);           // Output value

    delay(delayTime);   // Delay


  
    // Display digit1
    digitalWrite(digit1, 1);
    digitalWrite(digit2, 0);

    if (val1 < 0 || val1 > 9) {
      int i=0;
      for (int segBit : out[10])
        digitalWrite(segPins[i++], segBit);         // Output error
    }

    i=0;
    for (int segBit : out[val1])
      digitalWrite(segPins[i++], segBit);           // Output value

    delay(delayTime);   // Delay

}

// test
// tests the display
//
void SevenSeg2Bit::test() {
  for (int d = 1; d<=2; d++) {

    if (d==1) {
      digitalWrite(digit1, 1);
      digitalWrite(digit2, 0);
    } else {
      digitalWrite(digit1, 0);
      digitalWrite(digit2, 1);
    }

    
    for (int currOn = 0; currOn<7; currOn++) {
        for (int i=0; i<7; i++) {
            digitalWrite(segPins[i], 0);
        }
        digitalWrite(segPins[currOn], 1);
        delay(200);
    }
    digitalWrite(segPins[6], 0);
    delay(200);
  }
  digitalWrite(digit2, 0);
}
