#pragma once

#include <stdio.h>
//#include <cstring>
#include "Card.h"



static struct state {		// Receiving played card, or sending n cards (up to 7)
	int cardCount = 0;
	Card cards[7];
	bool calledUno = false;
} STATE;

#define	dataSize sizeof(state)





class SerialHandler
{
private:

public:
	SerialHandler();									// Initialize player serials, also sends initial cards to each player
	bool sendToPlayer(int player, state& s);			// Sends card data to the player
	state receiveFromPlayer(int player);				// Waits for player to send data
	bool sendToHost(state& s);				// Send data to host
	state receiveFromHost();				// Receive data from host
	
	
};
