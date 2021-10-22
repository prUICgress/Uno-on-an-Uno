
#include "Buzzer.h"
#include "LCD.h"
#include "LEDMatrix.h"
#include "SevenSeg2Bit.h"
#include "Unogame.h"

// Declare objects
Buzzer buzzer(7);
LCD lcd(13,12,11,10,9,8);
LEDMatrix matrix(49,51,53);   // Data, clk, cs
SevenSeg2Bit topCard(3,4,25,35,33,23,31,27,29,-1);         // Right 7-seg
SevenSeg2Bit cardCounter(5,6,32,22,24,28,26,34,30,-1);     // Left 7-seg
UnoGame* game = nullptr;
  

// Extra pins
int rgb[3] = {37, 39, 41};
int drawButton = 36;
int startButton = 43;         // Used for testing, pairing, and starting the game

// Constants
const int initialCardCount = 7;


void setRGB(int r, int g, int b);
void displayColor(int color);
void displayCard(int number, int color);
void testComponents();
int pairPlayers();
void(* resetFunc) (void) = 0;            // Resets the program
void displayTopChange();                 // Displays the top card on the matrix and buzzer
void updateLCDInfo();                   // Displays game info on the LCD


void setup() {  
    // Setup pins
    pinMode(drawButton, INPUT);
    pinMode(startButton, INPUT);

    Serial.setTimeout(-1);
    Serial.begin(9600);               // Init serial
    randomSeed(analogRead(0));        // Randomize seed

    lcd.displayCenter("Press start","to begin");
    matrix.clear();
    
    // Wait for input
    while (!digitalRead(startButton));    // Wait for press

    // Check for held button
    // If start button is held for 2 seconds, run tests and reset
    // Else begin game
    int pressTime = 0;
    while (digitalRead(startButton)) {
        if (pressTime >= 2000) {
            testComponents();
            break;
        }
        delay(1);
        pressTime++;
    }
    lcd.clear();


    // Pair players
    int playerCount = pairPlayers();

    // Initialize game
    game = new UnoGame(playerCount, initialCardCount);


    // Display first state
    buzzer.playToneList("playCard");

    // Show top card
    displayColor(game->getTop().getColor());
    for (int rep=0; rep<200; rep++) {
      topCard.display(game->getTop().getNumber());
      delay(5);
    }
    topCard.off();
}

void loop() {
  while (!game->isGameComplete()) {
    buzzer.playToneList("playCard");
    Serial.println();

    // Reset UNO call for current player
    game->setCalledUno(false);

    // Get input
    char cmd;
    int index;
    int done = 0;
    char readIn[3];
    
    // Display current state
    game->dumpGameState();
    lcd.clear();
    updateLCDInfo(); 

    Serial.print("Top: ");
    displayCard(game->getTop().getNumber(), game->getTop().getColor());
    Serial.println();
    Serial.print("Current player: ");
    Serial.println(game->getCurrentPlayer()+1); 
    displayTurnMatrix();
    for (int i=0; i<200; i++)
      cardCounter.display(game->playerAt(game->getCurrentPlayer()).getCardCount());
    cardCounter.off();


    // Loop until player's turn is complete
    while (!done) {
      // Reset 
      Serial.print("Input: ");
      Serial.readBytesUntil('\n', readIn, 3);
      cmd = readIn[0];
      index = (int)readIn[2] - '0';

      // Display input
      Serial.print(cmd);
      Serial.print(" ");
      Serial.println(index);

      // COMMAND
      // Play card
      if (cmd == 'p') {
        Card c = game->playerAt(game->getCurrentPlayer()).cardAt(index);  
        if (game->canPlayCard(c.getNumber(), c.getColor())) {
          // Play card
          game->playCard(game->getCurrentPlayer(), index);
          done = 1;

          // If WILD, Get COLOR
          if (c.getColor() == C_WILD) {
            Serial.print("Color? ");
            char inputC[1];
            int chosen = 0;
            while (chosen == 0) {
              Serial.readBytesUntil('\n', inputC, 1);
              char input = inputC[0];
              chosen = 1;
              if (input == 'r') {
                game->setTop(c.getNumber(), C_RED);
                matrix.clear();
              } else if (input == 'y') {
                game->setTop(c.getNumber(), C_YELLOW);
                matrix.clear();
              } else if (input == 'g') {
                game->setTop(c.getNumber(), C_GREEN);
                matrix.clear();
              } else if (input == 'b') {
                game->setTop(c.getNumber(), C_BLUE);
                matrix.clear();
              } else {
                buzzer.playToneList("invalidTone");
                Serial.println("Invalid color");
                chosen = 0;
              }

            }
          } else {
            matrix.clear();
            game->setTop(c.getNumber(), c.getColor());
          }
   

          // Apply cards
          if (c.getNumber() == CARD_DRAW_2) {
            game->nextPlayer();
            for (int i=0; i<2; i++)
              game->playerAt(game->getCurrentPlayer()).draw();

            // If >2 players, skip player
            if (game->getPlayerCount() > 2)
              game->nextPlayer();
            
          } else if (c.getNumber() == CARD_WILD_4) {
            game->nextPlayer();
            for (int i = 0; i < 4; i++)
              game->playerAt(game->getCurrentPlayer()).draw();

            // If >2 players, skip player
            if (game->getPlayerCount() > 2)
              game->nextPlayer();
            
          } else if (c.getNumber() == CARD_REVERSE) {
            if (game->getPlayerCount() == 2) {
              // Next player is self
              game->toggleReverse();
            } else {
              // Next player is previous player
              game->toggleReverse();
              game->nextPlayer();
            }
          } else if (c.getNumber() == CARD_SKIP) {
            if (game->getPlayerCount() == 2) {
              // Next player is self
            }
            else {
              // Next player is 2 players ahead
              game->nextPlayer();
              game->nextPlayer();
            }
          } else {
            // Other cards just increment the current turn
            game->nextPlayer();
          }
        }
        else {
          buzzer.playToneList("invalidTone");
          Serial.println("Card cannot be played");
        }
      } 
      // COMMAND
      // Call UNO
      else if (cmd == 'u') {
        if (game->canHaveUno(game->getCurrentPlayer())) {
          buzzer.playToneList("unoTone");
          game->setCalledUno(true);
          Serial.print("Player ");
          Serial.print(game->getCurrentPlayer() + 1);
          Serial.println(" called UNO!");
        }
        else {
          buzzer.playToneList("invalidTone");
          Serial.println("You do not have uno.");
        }
      } 
      // COMMAND
      // Draw card
      else if (cmd == 'd') {
        buzzer.playToneList("drawCard");
        game->playerAt(game->getCurrentPlayer()).draw();
        done = 1;
        game->nextPlayer();
      } else {
        buzzer.playToneList("invalidTone");
        Serial.println ("Invalid input");
        Serial.print ("Received: [");
        Serial.print(cmd);
        Serial.println("]");
      }
    }

    // Check for UNO gotcha - place before each nextplayer?
    // Serial.println("Waiting for uno GOTCHA");

    // Prepare for next turn
    Serial.print("\n\n");
    
    // Show top card
    matrix.clear();
    buzzer.playToneList("playCard");
    displayColor(game->getTop().getColor());
    int num = game->getTop().getNumber();
      
    if (num >= 0 && num <= 9) {               // Display 0-9
      
      for (int rep=0; rep<200; rep++)
        topCard.display(num);
        
    } else if (num == CARD_SKIP) {            // Display skip

      for (int rep=0; rep<200; rep++)
        topCard.displaySkip();
      
    } else if (num == CARD_DRAW_2) {          // Display +2
      
      for (int repIn=0; repIn<50; repIn++)
        topCard.displayPlus();
      for (int repIn=0; repIn<50; repIn++)
        topCard.display(2);
        
    } else if (num == CARD_WILD_4) {          // Display +4
      
      for (int repIn=0; repIn<50; repIn++)
        topCard.displayPlus();
      for (int repIn=0; repIn<50; repIn++)
        topCard.display(4);
        
    } else if (num == CARD_WILD) {

      for (int repIn=0; repIn<50; repIn++) {
        topCard.displayW();
        delay(5);
      }
      
    } else if (num == CARD_REVERSE) {

      for (int repIn=0; repIn<5; repIn++) {
        topCard.displayReverse(50);
        delay(50);
      }
      
    }

    
    topCard.off();


    // Display output to top card 7seg
    // Update matrix display to show card for a moment
    matrix.clear();
    if (cmd == 'p')
      displayTopChange(); 
  }
  
  matrix.clear();
  buzzer.playToneList("winTone");
  Serial.print("Game is complete! Player ");
  Serial.print(game->getWinner() + 1);
  Serial.println(" is the winner");
  lcd.clear();
  String line = "Player ";
  line += String(game->getWinner() + 1);
  lcd.displayCenter(line, "wins");
  game->addScore(game->getWinner(), game->getScoreEarned());


  // Display score
  delay(1000);
  lcd.clear();
  delay(1000);
  String line1,line2;
  line1 += "1:";
  line1 += game->playerAt(0).getScore();
  line1 += " 2:";
  line1 += game->playerAt(1).getScore();
  if (game->getPlayerCount() > 2) {
    line2 += " 3:";
    line2 += game->playerAt(2).getScore();
  }
  if (game->getPlayerCount() > 3) {
    line2 += " 4:";
    line2 += game->playerAt(3).getScore();    
  }
  lcd.displayCenter(line1, line2);
  
  // Wait for next game
  while (!digitalRead(startButton));
  game->newRound();
}








// Function definitions
//

// testComponents
// Tests each component separately
//
void testComponents() {
    lcd.clear();
    lcd.displayCenter("Running", "tests");
    delay(1000);


    // Test draw button
    lcd.displayCenter("Press \"DRAW\"", "button");
    while (!digitalRead(drawButton));
    while (digitalRead(drawButton));

    
    // Test LCD
    lcd.clear();
    lcd.displayCenter("Press DRAW if", "LCD works");
    while (!digitalRead(drawButton));
    while (digitalRead(drawButton));
    lcd.clear();


    // Test left 7seg
    lcd.displayCenter("Testing", "left 7seg");
    for (int i=0; i<99; i++)
      for (int rep=0; rep<5; rep++) {
        cardCounter.display(i);
        delay(1);
      }
    cardCounter.off();
    lcd.displayCenter("Press DRAW if", "left 7seg works");
    while (!digitalRead(drawButton));
    while (digitalRead(drawButton));
    lcd.clear();


    // Test right 7seg
    lcd.displayCenter("Testing", "right 7seg");
    for (int i=0; i<99; i++)
      for (int rep=0; rep<5; rep++) {
        topCard.display(i);
        delay(1);
      }
    topCard.off();
    lcd.displayCenter("Press DRAW if", "right 7seg works");
    while (!digitalRead(drawButton));
    while (digitalRead(drawButton));
    lcd.clear();


    // Test LED matrix
    lcd.displayCenter("Testing", "LED Matrix");
    matrix.test();
    delay(1000);
    matrix.display("number1");
    lcd.displayCenter("Press DRAW if", "LED matrix works");
    while (!digitalRead(drawButton));
    while (digitalRead(drawButton));
    lcd.clear();
    matrix.clear();


    // Test Buzzer
    lcd.displayCenter("Testing", "buzzer");
    buzzer.playRange(0,500,5);
    lcd.displayCenter("Press DRAW if", "buzzer works");
    while (!digitalRead(drawButton));
    while (digitalRead(drawButton));
    lcd.clear();


    // Test RGB LED
    lcd.displayCenter("Testing", "RGB LED");
    setRGB(255,255,255);
    lcd.displayCenter("Press DRAW if", "LED works");
    while (!digitalRead(drawButton));
    while (digitalRead(drawButton));
    setRGB(0,0,0);
    lcd.clear();

    
    // All tests complete
    lcd.displayCenter("Testing","Complete");
    // Reset
    delay(2000);
    lcd.clear();
    resetFunc();    // Reset program
}

// pairPlayers
// Checks the connections and pairs players
//
int pairPlayers() {
    return 4;
}

// setRGB
// Sets the RGB value of the led
//
void setRGB(int r, int g, int b) {
    analogWrite(rgb[0], r);
    analogWrite(rgb[1], g);
    analogWrite(rgb[2], b); 
}

// displayColor
// Sets the RGB value to the specified color
//
void displayColor(int color) {
    if (color == C_RED)
      setRGB(255,0,0);
    else if (color == C_YELLOW)
      setRGB(255,140,0);
    else if (color == C_GREEN)
      setRGB(0,255,0);
    else if (color == C_BLUE)
      setRGB(0,0,255);
    else
      setRGB(0,0,0);
}

// displayCard
// Displays a card in a readable form in serial
//
void displayCard(int number, int color) {
  Serial.print("[");

  // Number
  if (number >= 0 && number <= 9)
    Serial.print(number);
  else if (number == CARD_DRAW_2)
    Serial.print("draw2");
  else if (number == CARD_REVERSE)
    Serial.print("rev");
  else if (number == CARD_SKIP)
    Serial.print("skip");
  else if (number == CARD_WILD)
    Serial.print("wild");
  else if (number == CARD_WILD_4)
    Serial.print("wild4");
  else {
    Serial.print("?");
    Serial.print(number);
  }

  Serial.print(",");

  // Color
  if (color == C_RED)
    Serial.print("RED");
  else if (color == C_YELLOW)
    Serial.print("YELLOW");
  else if (color == C_GREEN)
    Serial.print("GREEN");
  else if (color == C_BLUE)
    Serial.print("BLUE");
  else if (color == C_WILD)
    Serial.print("WILD");
  else {
    Serial.print("?");
    Serial.print(color);
  }

  Serial.print("] ");
}

// displayTopChange
// Displays the top card change on the matrix and buzzer
//
void displayTopChange() {
    int cardNumber = game->getTop().getNumber();
    
    if (cardNumber >= 0 && cardNumber <= 9) {                 // 0-9
        matrix.display("number"+String(cardNumber));
        buzzer.playToneList("playCard");
        delay(1000);
    } else if (cardNumber == CARD_SKIP) {                     // Skip
        matrix.display("skip");
        buzzer.playToneList("skip");
        delay(1000);
    } else if (cardNumber == CARD_REVERSE) {                  // Reverse
        buzzer.playRange(750,250,5);
        buzzer.playRange(250,750,5);
        if (game->getIsReversed())
            matrix.animate("reverse_Frame1 reverse_Frame2 reverse_Frame3 reverse_Frame4", 250);
        else
            matrix.animate("reverse2_Frame1 reverse2_Frame2 reverse2_Frame3 reverse2_Frame4", 250);
        delay(100);
    } else if (cardNumber == CARD_DRAW_2) {                   // Draw 2
        for (int i=0; i<2; i++) {
            buzzer.playToneList("drawCard");
        }
        matrix.displayScroll("+2");
        delay(100);
    } else if (cardNumber == CARD_WILD_4) {                   // Wild 4
        for (int i=0; i<4; i++) {
            buzzer.playToneList("drawCard");
        }
        matrix.displayScroll("+4");
        delay(100);
    } else if (cardNumber == CARD_WILD) {                     // Wild
        buzzer.playToneList("playCard");
        matrix.display("letterW");
        delay(1000);
    }
}

// displayTurnMatrix
// Displays the current player's turn with an arrow 
//
void displayTurnMatrix() {
    switch (game->getCurrentPlayer()) {
        case 0:
            matrix.display("downArrow");
            break;
        case 1:
            matrix.display("leftArrow");
            break;
        case 2:
            matrix.display("upArrow");
            break;
        case 3:
            matrix.display("rightArrow");
            break;
    }
}

// updateLCDInfo
// Displays game info on the LCD
//
void updateLCDInfo() {
    String line1 = "Top: ";
    String line2;

    // Build line 1
    int color = game->getTop().getColor();
    if (color == C_RED)
      line1 += "Red ";
    else if (color == C_YELLOW)
      line1 += "Yellow ";
    else if (color == C_GREEN)
      line1 += "Green ";
    else if (color == C_BLUE)
      line1 += "Blue ";
    else
      line1 += "Unk ";

    int num = game->getTop().getNumber();
    if (num >= 0 && num <= 9)
      line1 += num;
    else if (num == CARD_SKIP)
      line1 += "Skip";
    else if (num == CARD_REVERSE)
      line1 += "Rev";
    else if (num == CARD_DRAW_2)
      line1 += "+2";
    else if (num == CARD_WILD)
      line1 += "Wild";
    else if (num == CARD_WILD_4)
      line1 += "+4";
    else
      line1 += "Unk";
      

    // Build line 2
    for (int i=0; i<game->getPlayerCount(); i++) {
      line2 += game->playerAt(i).getCardCount();
      if (i<game->getPlayerCount()-1)
        line2 += " ";
    }

    // Display
    lcd.displayCenter(line1, line2);
}
