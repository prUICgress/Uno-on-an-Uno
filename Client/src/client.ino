#include "LCD.h"
#include "SerialHandler.h"
#include "CardList.h"

#define potR 256
#define potY 512
#define potB 768
#define potG 1024





LCD lcd(9,8,7,6,5,4);
SerialHandler sh;
CardList cards;



// Pins
const int buttonPins[4] = {13,12,11,10};   // Prev, select, next, uno
const int potPin = A4;
const int ledPins[4] = {A0,A1,A2,A3};      // R Y B G


// Card display
int cardSelection = 0;



void displayCards();      // Displays cards to lcd
void displayColor(bool r, bool y, bool b, bool g);      // Displays each color on the LEDs
int chooseColor();       // Prompts the player to choose a color




void setup() {
    // Init pins
    for (int i=0; i<4; i++) {
        pinMode(buttonPins[i], INPUT);
        pinMode(ledPins[i], OUTPUT);
    }
    pinMode(potPin, INPUT);
    
    Serial.begin(9600);     // Init serial

    // Get initial card list from host
    lcd.displayCenter("Waiting for","connection");
    state s = sh.receiveFromHost();

    

    // Parse data
    for (int c = 0; c < s.cardCount; c++) {
        Card cd = s.cards[c];
        cards.addCard(cd.getNumber(), cd.getColor());
    }
    lcd.clear();
}


void loop() {
    // Wait until it is this player's turn
    // Also process any card draws
    state wait;
    while (wait.cardCount != -1) {
        // Update display
        delay(50);
        lcd.clear();
        displayCards();

        // Get input but do not allow selection or uno (won't be received anyway)
        if (digitalRead(buttonPins[0])) {
            delay(300);               // Delay rather than wait to release to avoid player stalling game by holding button
            cardSelection--;
            if (cardSelection < 0) cardSelection = cards.getCardCount()-1;
            
        } else if (digitalRead(buttonPins[2])) {
            delay(300);               // Delay rather than wait to release to avoid player stalling game by holding button
            cardSelection++;
            if (cardSelection == cards.getCardCount()) cardSelection = 0;
        }

        // Draw cards if nonzero
        if (wait.cardCount > 0) {
            lcd.displayCenter("Drawing cards:",String(wait.cardCount));
            // Parse data
            for (int c = 0; c < wait.cardCount; c++) {
                cards.addCard(wait.cards[c].getNumber(), wait.cards[c].getColor());
            }
            delay(500);
            lcd.clear();
            wait.cardCount = 0;     // Reset state so it isnt read again

        } else if (wait.cardCount == -3) {      // If -3, another player has not called UNO, check for gotcha
            if (digitalRead(buttonPins[3])) {
                state gotcha;
                gotcha.cardCount = -3;
                sh.sendToHost(gotcha);
            }
          
        }

        // Get state if available
        if (Serial.available() >= dataSize)
            wait = sh.receiveFromHost();
    }

    // It is now the player's turn
    // Allow inputs -- Host will send (-2) when processed and turn is complete
    state process;
    while (process.cardCount != -2) {
        // Get input
        if (digitalRead(buttonPins[0])) {                                        // Previous
            while (digitalRead(buttonPins[0]));   // Wait for release
            cardSelection--;
            if (cardSelection < 0) cardSelection = cards.getCardCount()-1;

            
        } else if (digitalRead(buttonPins[1])) {                                 // Select
           while (digitalRead(buttonPins[1]));   // Wait for release
           state s;
           s.cardCount = 1;
           s.cards[0] = cards[cardSelection];
           if (s.cards[0].getColor() == C_WILD)     // If wild, get the color
              s.cards[0].setColor(chooseColor());
           sh.sendToHost(s);

           delay(100);
           if (Serial.available() >= dataSize) process = sh.receiveFromHost();
           if (process.cardCount == -2) {
              cards.playCard(cardSelection);      // Received '-2' -- Remove card since it was played
              cardSelection--;                  // Reset position on played card
              if (cardSelection < 0) cardSelection = 0;
           }

           
        } else if (digitalRead(buttonPins[2])) {                                 // Next
            while (digitalRead(buttonPins[2]));   // Wait for release
            cardSelection++;
            if (cardSelection == cards.getCardCount()) cardSelection = 0;

            
        } else if (digitalRead(buttonPins[3])) {                                 // Call uno
            while (digitalRead(buttonPins[3]));   // Wait for release
            state s;
            s.calledUno = true;
            sh.sendToHost(s);

            
        }


        // Update display
        delay(50);
        lcd.clear();
        displayCards();
        

        // Update status if available
        if (Serial.available() >= dataSize)
            process = sh.receiveFromHost();

        // Check if status is drawCard
        if (process.cardCount == 1) {
            process.cardCount = 0;          // Reset back to 0 for future runs
            cards.addCard(process.cards[0].getNumber(), process.cards[0].getColor());
        
        }
    }
    
    

    
}





// displayCards
// Displays the cards with selection at the current index
// If index is 0, displays -1 and -2 also
//
void displayCards() {
    char list[6] = "-----";
    char list2[6] = "-----";

    int currPos = 0;      // list index
    for (int i=cardSelection-2; i<=cardSelection+2; i++) {
        if (i<0 || i>=cards.getCardCount()) {
          currPos++;
          continue;     // Ignore out of bounds
        }

        Card c = cards.at(i);
        int num = c.getNumber();
        int col = c.getColor();

        // number                 
        if (num == CARD_WILD) {
          list[currPos] = 'W';
        } else if (num == CARD_SKIP) {
          list[currPos] = 'S';
        } else if (num == CARD_REVERSE) {
          list[currPos] = 'R';
        } else if (num == CARD_WILD_4) {
          list[currPos] = '*';
        } else if (num == CARD_DRAW_2) {
          list[currPos] = 'D';
        } else {
          list[currPos] = char(num) + '0';
        }

        // color                    -- not sure why; card colors are incorrect (fixed)
        if (col == C_RED)
          list2[currPos] = 'R';
        else if (col == C_YELLOW)
          list2[currPos] = 'Y';
        else if (col == C_GREEN)
          list2[currPos] = 'G';
        else if (col == C_BLUE)
          list2[currPos] = 'B';
        else list2[currPos] = 'W';
        
        currPos++;
    }

    // Build string
    String top = "";
    String btm = "";

    for (int i=0; i<5; i++) {
      if (i==2) {
        top += "|";
        top += list[i];
        top += "| ";

        btm += "|";
        btm += list2[i];
        btm += "| ";
        
      } else {
          top += list[i];
          top += " ";

          btm += list2[i];
          btm += " ";
      }
    }

    // Output on lcd
    lcd.displayCenter(top, btm);
}


// displayColor
// Displays the specified colors on the LEDs
//
void displayColor(bool r, bool y, bool b, bool g) {
    digitalWrite(ledPins[0], r);
    digitalWrite(ledPins[1], y);
    digitalWrite(ledPins[2], b);
    digitalWrite(ledPins[3], g);
}


// chooseColor
// Prompts the player to choose a color
//
int chooseColor() {
    int color = C_RED;    // Default choice: red
    while (!digitalRead(buttonPins[1])) {     // Wait until selection
        int v = analogRead(potPin);

        // Read potentiometer value
        if (v >= 0 && v < potR) {
            color = C_RED;
            displayColor(1,0,0,0);
        } else if (v >= potR && v < potY) {
            color = C_YELLOW;
            displayColor(0,1,0,0);
        } else if (v >= potY && v < potB) {
            color = C_BLUE;
            displayColor(0,0,1,0);
        } else if (v >= potB && v <= potG) {
            color = C_GREEN;
            displayColor(0,0,0,1);
        }

        
    }

    displayColor(0,0,0,0);
    return color;
}
