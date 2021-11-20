//
// CardList
//
// Holds a list of cards
//

#pragma once

#include <Arduino.h>

#include "Card.h"




class CardList
{
private:
	Card cardList[100];		// Holds 100 cards
	int cardCount;			// Current card amount in list
	double cardWeights[15] = { 1,2,2,2,2,2,2,2,2,2,2,2,2,1,1 };		// Holds the weight of drawing each card; used by private functions

public:
	CardList();				  // Default constructor
	CardList(int n);		// Parameterized constructor - Initialized with 'n' cards drawn
	Card at(int i);			// Returns card at i
	Card draw();			                                    // Draws a random card, returns the card that was added
	bool playCard(int i);	                                // Plays card at i. Returns true if successful
	int findCardIndex(int cardNumber, int cardColor);			// Returns the index of the card if specified card exists in the cardList
	int getCardCount() const;		                          // Returns the number of cards in cardList
  void removeAll();                                     // Empties the list of cards
  bool addCard(int number, int color);      // Adds specified card to deck

	// Operators
	Card operator[](int i);		// Access [] operator - returns card at i

	// Test functions
	void dump();				      // Dumps all card info

private:
	// Helper functions
	int getRandomCardNumber();				    // Returns a random card number, uses specified card weights
	int getRandomColor(int cardNumber);		// Returns a random color based on the given card number
};
