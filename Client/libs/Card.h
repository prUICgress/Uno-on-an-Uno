//
// Card
//
// A single UNO card
//

#pragma once

#include <Arduino.h>

//
// CardNumber
//
enum CardNumber {
	CARD_UNKNOWN = -1,
	CARD_0 = 0,
	CARD_1 = 1,
	CARD_2 = 2,
	CARD_3 = 3,
	CARD_4 = 4,
	CARD_5 = 5,
	CARD_6 = 6,
	CARD_7 = 7,
	CARD_8 = 8,
	CARD_9 = 9,
	CARD_WILD = 10,
	CARD_WILD_4 = 11,
	CARD_REVERSE = 12,
	CARD_SKIP = 13,
	CARD_DRAW_2 = 14
};

//
// CardColor
//
enum CardColor {
	C_UNKNOWN = -1,
	C_RED = 0,
	C_YELLOW = 1,
	C_GREEN = 2,
	C_BLUE = 3,
	C_WILD = 4
};

class Card
{
private:
	int cardNumber;
	int cardColor;

public:
	Card();
	Card(int number, int color);
	void setColor(int color);
	void setNumber(int number);
	int getColor() const;
	int getNumber() const;

};
