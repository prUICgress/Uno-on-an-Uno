//
// Player
//
// Generated for each player in the game
//

#include "Player.h"



// Default constructor - initialized for a new player
//
Player::Player() {
	score = 0;
}

// Parameterized constructor - begins with 'cardCount' cards
//
Player::Player(int cardCount) {
	score = 0;
	for (int i = 0; i < cardCount; i++)
		draw();
}

// addScore
// Adds to the score
//
void Player::addScore(int score) {
	this->score += score;
}

// getScore
// Returns the score
//
int Player::getScore() const {
	return score;
}

// draw
// Draws a random card, returns the card that was added
//
Card Player::draw() {
	return cards.draw();
}

// hasCard
// Returns whether or not the card exists in the cardList
//
bool Player::hasCard(int cardNumber, int cardColor) {
	int index = cards.findCardIndex(cardNumber, cardColor);

	if (index == -1)
		return false;
	else
		return true;
}

// findCard
// Returns the index of the given card, returns -1 if it doesn't exist
//
int Player::findCard(int cardNumber, int cardColor) {
	return cards.findCardIndex(cardNumber, cardColor);
}

// playCard
// Plays card at i. Returns true if successful
//
bool Player::playCard(int i) {
	return cards.playCard(i);
}

// getCardCount
// Returns the amount of cards in the player's hand
//
int Player::getCardCount() const {
	return cards.getCardCount();
}

// dump
// Dumps all card information
//
void Player::dump() {
	cards.dump();
}

// at
// Returns the card at index
//
Card Player::cardAt(int index) {
	return cards.at(index);
}

// removeAllCards
// Removes all the player's cards
//
void Player::removeAllCards() {
    cards.removeAll();
}

//
// Operators
//

// operator[]
// Returns the card at cards[index]
//
Card Player::operator[](int index) {
	return cards[index];
}
