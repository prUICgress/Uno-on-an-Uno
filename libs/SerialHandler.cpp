#include "SerialHandler.h"




SerialHandler::SerialHandler() {
	
}


bool SerialHandler::sendToPlayer(int player, state& s) {
	char dataBuffer[dataSize];					// Data storage
	memcpy(&dataBuffer, &s, dataSize);		// Copy to data storage

	// Send to specified player
	switch (player) {
	case 1:
		Serial1.write(dataBuffer, dataSize);
		break;
	case 2:
		Serial2.write(dataBuffer, dataSize);
		break;
	case 3:
		Serial3.write(dataBuffer, dataSize);
		break;
	default:
		Serial.println("Error: send() invalid player");
		return false;
	}

	return true;
}


state SerialHandler::receiveFromPlayer(int player) {
	char dataBuffer[dataSize];					// char data storage
	state s;									// data output
	
	// Receive from specified player
	switch (player) {
	case 1:
		while (Serial1.available() < dataSize);	// Wait for serial input
		Serial1.readBytes(dataBuffer, dataSize);
		break;
	case 2:
		while (Serial2.available() < dataSize);	// Wait for serial input
		Serial2.readBytes(dataBuffer, dataSize);
		break;
	case 3:
		while (Serial3.available() < dataSize);	// Wait for serial input
		Serial3.readBytes(dataBuffer, dataSize);
		break;
	default:
		Serial.println("Error: receive() invalid player");
		return s;	// Default state
	}

	memcpy(&s, &dataBuffer, dataSize);		// Copy to data storage
	return s;
}


bool SerialHandler::sendToHost(state& s) {
	char dataBuffer[dataSize];					// Data storage
	memcpy(&dataBuffer, &s, dataSize);		// Copy to data storage

	// Write to host
	Serial.write(dataBuffer, dataSize);

	return true;
}

state SerialHandler::receiveFromHost() {
	char dataBuffer[dataSize];					// char data storage
	state s;									// data output

	while (Serial.available() < dataSize);	// Wait for serial input
	
	// Receive from host
	Serial.readBytes(dataBuffer, dataSize);

	memcpy(&s, &dataBuffer, dataSize);		// Copy to data storage
	return s;
}

