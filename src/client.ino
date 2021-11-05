#include <LiquidCrystal.h>
#include <CardList.h>
#include <Card.h>

struct GameInfo
{
    int firstPass;
    // put cards in here i guess lole
    int cardNum;
    int cardColor;
    bool uno;
    bool isCardPlayed; // just check only the first card in passedHand[] if true, that's the played card
    Card passedHand[7];
    int player; // 1, 2, or 3
} ginfo;
#define BYTE_SIZE sizeof(ginfo) // defines byte size to be written
#define cardX = 0               // TODO define to lcd position to update
#define cardY = 0               // TODO define to lcd position to update
const int prevButtonPin = 13;
const int selectButtonPin = 12;
const int nextButtonPin = 11;
const int unoButtonPin = 10;

char dataBuffer[BYTE_SIZE]; // sneaking struct into serial by type cast
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);
CardList myDeck = new CardList(7);
Card selectedCard; // will this cause shallow copy issue in arduino?
// maybe need card copy constructor
void led(int cardColor)
{
    switch (cardColor)
    {
    case 0:
        digitalWrite(A0, r);
        break;
    case 1:
        digitalWrite(A1, y);
        break;
    case 2:
        digitalWrite(A2, b);
        break;
    case 3:
        digitalWrite(A3, g);
        break;
    case 4: // wild card
        digitalWrite(A0, r);
        digitalWrite(A1, y);
        digitalWrite(A2, b);
        digitalWrite(A3, g);
        break;
    default:
        // sit down and cry,
    }

    void setup()
    {
        // begin init LEDS
        pinMode(A0, INPUT);
        pinMode(A1, INPUT);
        pinMode(A2, INPUT);
        pinMode(A3, INPUT);
        pinMode(A4, INPUT);
        // end init LEDS
        lcd.begin(16, 2); // init lcd, I've never had to do this
        Serial.begin(9600);
        // begin init buttons
        pinMode(prevButtonPin, INPUT);
        pinMode(selectButtonPin, INPUT);
        pinMode(nextButtonPin, INPUT);
        pinMode(unoButtonPin, INPUT);
        // end init button
    }

    void loop()
    {
        //digitalWrite(A0, 1);
        //digitalWrite(A1, 0);
        //digitalWrite(A2, 0);
        //digitalWrite(A3, 0);
        //delay(500);
        //digitalWrite(A0, 0);
        //digitalWrite(A1, 1);
        //digitalWrite(A2, 0);
        //digitalWrite(A3, 0);
        //delay(500);
        //digitalWrite(A0, 0);
        //digitalWrite(A1, 0);
        //digitalWrite(A2, 1);
        //digitalWrite(A3, 0);
        //delay(500);
        //digitalWrite(A0, 0);
        //digitalWrite(A1, 0);
        //digitalWrite(A2, 0);
        //digitalWrite(A3, 1);
        //delay(500);

        //lcd.print(1);

        if (Serial.available() >= BYTE_SIZE)
        {
            // Serial.println("in da loop");
            Serial.readBytes(dataBuffer, BYTE_SIZE);
            memcpy(&ginfo, &dataBuffer, BYTE_SIZE); // copying bytes into char array since that's the cast arduino requires
            // will be memcpy'd back into host ginfo
            //Serial.println(gameInfo.firstPass);
            // Serial.println(ginfo.firstPass);

            // TODO make changes to card list here depending on what gets passed
        }
        int val = analogRead(A4);

        if (val >= 0 && val <= 250)
            led(1, 0, 0, 0);
        else if (val > 250 && val <= 500)
            led(0, 1, 0, 0);
        else if (val > 500 && val <= 750)
            led(0, 0, 1, 0);
        else
            led(0, 0, 0, 1);
    }

    void unoButton()
    {
        // call uno here
        ginfo.Uno = true;
        memcpy(&dataBuffer, &ginfo, BYTE_SIZE);
        Serial.write(dataBuffer, BYTE_SIZE); //Read the serial data and store in var
        ginfo.Uno = false;                   // TODO change if needed to be
    }
    void nextButton()
    { // "next" button
        // call uno here
        myDeck++; // implement this operator, add an index to card list
        selectedCard = myDeck.selected;
        lcd.setCursor(0, 1);
        lcd.print(selectedCard.toString());
    }
    void selectCard()
    {
        ginfo.isCardPlayed = true;
        ginfo.passedHand[] = selectCard;
        memcpy(&dataBuffer, &ginfo, BYTE_SIZE);
        Serial.write(dataBuffer, BYTE_SIZE); //Read the serial data and store in var
        lcd.print(selectCard);
        lcd.print(" sent.");
    }
    void prevButton()
    { // "prev" button
        // call uno here
        myDeck--; // implement this operator, add an index to card list
        selectedCard = myDeck.selected;
        lcd.setCursor(0, 1);
        lcd.print(selectedCard.toString());
    }
