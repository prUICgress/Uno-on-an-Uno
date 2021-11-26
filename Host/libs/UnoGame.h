//
// UnoGame
//
// Handles the UNO game state and players
//

#pragma once

#include <Arduino.h>

#include "Player.h"
#include "Buzzer.h"
#include "LCD.h"
#include "LEDMatrix.h"
#include "SevenSeg2Bit.h"


class UnoGame
{
private:
	Player players[4];			  // The list of all players in the game
	int playerCount;			  // The number of players
	int currentPlayer;			  // The current player's turn
	Card top;				  // The last played card
	bool calledUno = false;			  // Has the current player called uno
	bool isReversed = false;		  // Game order is reversed
  	int cardCount;           		  // Initial card count

public:
	UnoGame(int playerCount, int cardCount);	        // Constructor - Initializes game with 'playerCount' players each with 'cardCount' cards
	int getPlayerCount() const;				// Returns the number of players
	Player& playerAt(int index);				// Returns the player at index					
	int getCurrentPlayer() const;				// Returns the current player
	void nextPlayer();					// Sets the currentPlayer to the next player
	void setTop(int cardNumber, int cardColor);		// Sets the last played card
	Card getTop();						// Returns the last played card
	bool canPlayCard(int cardNumber, int cardColor); 	// Returns true if the given card can be played on top
	bool playCard(int playerIndex, int cardIndex);		// Plays the given card. Returns true if the card is played
	void addScore(int playerIndex, int score);		// Adds to the player's score
	int getScore(int playerIndex) const;		// Returns the player's score
	bool hasUNO(int playerIndex) const;		// Returns true if given player has UNO
	bool canHaveUno(int playerIndex);		// Returns true if given player has the ability to call UNO
	bool isGameComplete() const;			// Returns true if given player has 0 cards
	int getWinner() const;				// Returns the index of the winner who has 0 cards
	void dumpGameState();				// Displays the game state of all players' cards
	bool getCalledUno() const;			// Returns true if UNO was called for the player
	void setCalledUno(bool value);		  // Sets the calledUno variable
	bool getIsReversed() const;		  // Returns true if the game is reversed order
	void toggleReverse();			  // Toggles the reverse behavior
  	void newRound();                    	  // Begins a new round
  	int getScoreEarned();              	  // Returns the score based on the currently held cards
	

	// Operators
	Player& operator[](int index);		  // Returns the player at index
};
