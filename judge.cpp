#include <iostream>

void updatePlayer(char c){

	// to sort out player`s chess.
	if (c != '0') {

		// should delete a single piece.
		if (turn == true) {

			// this Play is A
			currentPlayer[c-'1'].exist = 0; 

		} else {

			// this Play is B
			currentPlayer[c-'A'].exist = 0;
		}

	} else {

		// nothing to deal with.
	}
}

bool isGameEnd(){

	// is this player been killed the game.
	for (int i = 0; i < 6; i++) {
		if (currentPlayer[i].exist == 1) {
			return false;
		}
	}

	return true;
}