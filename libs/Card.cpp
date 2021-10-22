//
// Card
//
// A single UNO card
//

#include "Card.h"

// Default constructor
// Initializes cards to unbound negative values
//
Card::Card() {
	cardNumber = -1;
	cardColor = -1;
}

// Parameterized constructor
// Initializes to given values
//
Card::Card(int cardNumber, int cardColor) {
	this->cardNumber = cardNumber;
	this->cardColor = cardColor;
}

// setColor
//
void Card::setColor(int color) {
	cardColor = color;
}

// setNumber
//
void Card::setNumber(int number) {
	cardNumber = number;
}

// getColor
//
int Card::getColor() const {
	return cardColor;
}

// getNumber
//
int Card::getNumber() const {
	return cardNumber;
}