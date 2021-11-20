//
// UnoGame
//
// Handles the UNO game state and players
//


#include "UnoGame.h"


// Constructor - Initializes game with 'playerCount' players each with 'cardCount' cards
//
UnoGame::UnoGame(int playerCount, int cardCount) {
	calledUno = false;
	isReversed = false;
  this->cardCount = cardCount;
	
	// Max 4 players
	this->playerCount = playerCount;
	if (this->playerCount > 4)
		this->playerCount = 4;
	else if (this->playerCount < 2)
		this->playerCount = 2;
	
	// Initialize each player
	for (int i = 0; i < playerCount; i++)
		players[i] = Player(cardCount);

	// Determine first player (random)
	currentPlayer = random(playerCount);

	// Determine initial top card
	setTop(random(10), random(4));
}

// newRound
// Begins a new round after a game is complete
//
void UnoGame::newRound() {
    calledUno = false;
    isReversed = false;
  
  // Initialize each player
  for (int i = 0; i < playerCount; i++) {
    players[i].removeAllCards();
    for (int cards=0; cards<cardCount; cards++)
      players[i].draw();
  }

  // Determine first player (random)
  currentPlayer = random(playerCount);

  // Determine initial top card
  setTop(random(10), random(4));
}

// getPlayerCount
// Returns the number of players
//
int UnoGame::getPlayerCount() const {
	return playerCount;
}

// playerAt
// Returns the player at index
//
Player& UnoGame::playerAt(int index) {
	if (index < 0 || index >= playerCount)
		return players[0];
	else
		return players[index];
}

// getCurrentPlayer
// Returns the current player
//
int UnoGame::getCurrentPlayer() const {
	return currentPlayer;
}

// nextPlayer
// Sets the currentPlayer to the next player
//
void UnoGame::nextPlayer() {
	if (!isReversed) {
		currentPlayer++;
		if (currentPlayer == playerCount)
			currentPlayer = 0;
	} else {				// Player order is reversed
		currentPlayer--;
		if (currentPlayer < 0)
			currentPlayer = playerCount - 1;
	}
}

// setTop
// Sets the last played card
//
void UnoGame::setTop(int cardNumber, int cardColor) {
	top = Card(cardNumber, cardColor);
}

// getTop
// Returns the last played card
//
Card UnoGame::getTop() {
	return top;
}

// canPlayCard
// Returns true if the given card can be played on top
//
bool UnoGame::canPlayCard(int cardNumber, int cardColor) {
	// Check if wild
	if (cardColor == C_WILD)
		return true;

	// Check if valid color
	if (cardColor == getTop().getColor())
		return true;

	// Check if matching number
	if (cardNumber == getTop().getNumber())
		return true;

	// Cannot play
	return false;
}

// playCard
// Plays the given card. Returns true if the card is played
//
bool UnoGame::playCard(int playerIndex, int cardIndex) {
	// Check if valid player
	if (currentPlayer != playerIndex)
		return false;
	
	// Obtain card
	Card c = players[playerIndex].cardAt(cardIndex);
	
	// Check if card can be played
	if (!canPlayCard(c.getNumber(), c.getColor()))
		return false;
	
	// Play card
	return players[playerIndex].playCard(cardIndex);
}

// addScore
// Adds to the player's score
//
void UnoGame::addScore(int playerIndex, int score) {
	if (playerIndex < 0 || playerIndex >= playerCount)
		return;

	players[playerIndex].addScore(score);
}

// getScore
// Returns the player's score
//
int UnoGame::getScore(int playerIndex) const {
	if (playerIndex < 0 || playerIndex >= playerCount)
		return -1;
	
	return players[playerIndex].getScore();
}

// hasUNO
// Returns true if given player has UNO
//
bool UnoGame::hasUNO(int playerIndex) const {
	if (playerIndex < 0 || playerIndex >= playerCount)
		return false;

	return (players[playerIndex].getCardCount() == 1);
}

// canHaveUno
// Returns true if given player has the ability to call UNO
//
bool UnoGame::canHaveUno(int playerIndex) {
	if (playerIndex < 0 || playerIndex >= playerCount)
		return false;

	// Can have uno with two cards
	Card c1 = players[playerIndex].cardAt(0);
	Card c2 = players[playerIndex].cardAt(1);
	
	if (players[playerIndex].getCardCount() == 2) {
		// Is either card playable
		if (canPlayCard(c1.getNumber(), c1.getColor()))
			return true;
		if (canPlayCard(c2.getNumber(), c2.getColor()))
			return true;
	}

	return false;
}

// isGameComplete
// Returns true if given player has 0 cards
//
bool UnoGame::isGameComplete() const {
	for (int i = 0; i < playerCount; i++)
		if (players[i].getCardCount() == 0)
			return true;

	// No zeroes found
	return false;
}

// getWinner
// Returns the index of the winner who has 0 cards
//
int UnoGame::getWinner() const {
	for (int i = 0; i < playerCount; i++)
		if (players[i].getCardCount() == 0)
			return i;

	// No winners found
	return -1;
}

// dumpGameState
// Displays the game state of all players' cards
//
void UnoGame::dumpGameState() {
	for (int i=0; i<playerCount; i++) {
		Serial.print("Player ");
		Serial.print(i + 1);
		Serial.print(":   ");
		players[i].dump();
	}
}

// getCalledUno
// Returns true if UNO was called for the player
//
bool UnoGame::getCalledUno() const {
	return calledUno;
}

// setCalledUno
// Sets the calledUno variable
//
void UnoGame::setCalledUno(bool value) {
	calledUno = value;
}

// getIsReversed
// Returns true if the game is reversed order
//
bool UnoGame::getIsReversed() const {
	return isReversed;
}

// toggleReverse
// Toggles the reverse behavior
//
void UnoGame::toggleReverse() {
	isReversed = !isReversed;
}

// getScoreEarned
// Returns the score based on the currently held cards
//
int UnoGame::getScoreEarned() {
    dumpGameState();
    int total = 0;
    
    for (int i=0; i<playerCount; i++) {
        for (int currCard=0; currCard<players[i].getCardCount(); currCard++) {
            int num = players[i].cardAt(currCard).getNumber();

            // Get score from number
            if (num >= 0 && num <= 9)
              total += num;
            else if (num == CARD_DRAW_2 || num == CARD_REVERSE || num == CARD_SKIP)
              total += 20;
            else if (num == CARD_WILD || num == CARD_WILD_4)
              total += 50;
        }
    }

    return total;
}

//
// Operators
//

// operator[]
// Returns the player at index
//
Player& UnoGame::operator[](int index) {
	return playerAt(index);
}
