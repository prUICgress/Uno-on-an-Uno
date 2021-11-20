//
// Player
//
// Generated for each player in the game
//

#pragma once

#include <Arduino.h>

#include "CardList.h"



class Player
{
private:
	CardList cards;			// Currently held cards
	int score;				  // Score

public:
	Player();										        // Default constructor - initialized for a new player
	Player(int cardCount);							// Parameterized constructor - begins with 'cardCount' cards
	void addScore(int score);						// Adds to the score
	int getScore() const;							  // Returns the score
	Card draw();									      // Draws a random card, returns the card that was added
	bool hasCard(int cardNumber, int cardColor);	// Returns whether or not the card exists in the cardList
	int findCard(int cardNumber, int cardColor);	// Returns the index of the given card, returns -1 if it doesn't exist
	bool playCard(int i);							  // Plays card at i. Returns true if successful
	int getCardCount() const;						// Returns the amount of cards in the player's hand
	void dump();									      // Dumps all card information
	Card cardAt(int index);							// Returns the card at index
  void removeAllCards();              // Removes all the player's cards

	// Operators
	Card operator[](int index);						// Returns the card at cards[index]
};
