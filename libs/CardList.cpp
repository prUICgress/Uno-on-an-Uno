//
// CardList
//
// Holds a list of cards
//

#include "CardList.h"


// Default constructor
// Create empty cardList
//
CardList::CardList() {	
	cardCount = 0;
}

// Parameterized constructor - Initialized with 'n' cards drawn
//
CardList::CardList(int n) {
	cardCount = 0;
	for (int i=0; i<n; i++)
		draw();			// Draw n cards
}

// at
// Returns card at i
//
Card CardList::at(int i) {
	return cardList[i];
}

// draw
// Draws a random card
//
// A deck of UNO cards consists of (for each color) 2 cards of each 0-9, 2 draw-2s, reverses, and skips
//
Card CardList::draw() {
	// Make sure n cards can be drawn
	if (cardCount == 100)
		return Card(-1,-1);

	// Add cards
	int number = getRandomCardNumber();
	cardList[cardCount].setNumber(number);
	cardList[cardCount].setColor(getRandomColor(number));

	return cardList[cardCount++];
}

// playCard
// Plays card at i and removes from the cardList. Returns true if successful
//
bool CardList::playCard(int i) {
	// Check bounds
	if (i < 0 || i >= cardCount || cardCount == 0)
		return false;

	// Remove card by shifting cards
	for (int index = i; index < cardCount-1; index++)
		cardList[index] = cardList[index + 1];			// Shift cards
	cardCount--;
	cardList[cardCount].setColor(C_UNKNOWN);			// Reset last card		
	cardList[cardCount].setNumber(CARD_UNKNOWN);

	return true;
}

// findCardIndex
// Returns the index of the card if specified card exists in the cardList
//
int CardList::findCardIndex(int cardNumber, int cardColor) {
	for (int i = 0; i < cardCount; i++) {
		if (cardList[i].getNumber() == cardNumber && cardList[i].getColor() == cardColor)
			return i;
	}

	// Not found
	return -1;
}

// getCardCount
// Returns the number of cards in cardList
//
int CardList::getCardCount() const {
	return cardCount;
}

//
// Operators
//

// operator[]
// Access [] operator - returns card at i
//
Card CardList::operator[](int i) {
	return at(i);
}

//
// Helper functions
//

// getRandomCardNumber
// Returns a random card number, uses specified card weights
//
int CardList::getRandomCardNumber() {
	double rnd = random(28);		// sum of weights = 27

	int i = 0;
	for (int weightVal : cardWeights) {
		// Compare rnd with weight value
		if (rnd < weightVal)
			return i;

		// Iterate
		rnd -= weightVal;
		i++;
	}
	
	// Should never be reached
	return CARD_DRAW_2;
}

// getRandomColor
// Returns a random color based on the given card number
//
int CardList::getRandomColor(int cardNumber) {
	if (cardNumber == 10 || cardNumber == 11)
		return C_WILD;
	else if (cardNumber == -1)
		return C_UNKNOWN;
	else
		return random(4);
}

// removeAll
// Empties the list of cards
//
void CardList::removeAll() {
    // Delete each card
    for (int index = 0; index < cardCount; index++) {
        cardList[index].setColor(C_UNKNOWN);
        cardList[index].setNumber(CARD_UNKNOWN);
    }
    cardCount = 0;
}

//
// Test functions
//

// dump
// Dumps all card info
//
void CardList::dump() {
	for (int i = 0; i < cardCount; i++) {
		Serial.print("[");

		int number = cardList[i].getNumber();
		int color = cardList[i].getColor();
		
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
		else
			Serial.print(number);

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
	Serial.println();
}
