#include <iostream>
using namespace std;

// int currentPlayer[6] = {0,0,1,0,1,0};

int Game::nextMove() {

	srand(time(NULL));
	int result = rand()%6;

	char whichPlayerPiece = (this->turn == false? 'A' : '1');
	// movableChs = {'0','0'};

	for(int i = 0; i < 2; i++)
		movableChs[i].assign('0', false, 0, 0);

	if (currentPlayer[result].exist == 1) {
		// this piece is alive!
		for(int i = 0; i < 2; i++) {
			movableChs[i] = currentPlayer[result];
			// movableChs[i] = result + whichPlayerPiece;
		}
		return 1;
	}
	else {
		int move = 2;

		// search for maybe other two.
		for (int i = 0; i < result; i++) {
			if (currentPlayer[i].exist == 1){
				movableChs[0] = currentPlayer[result];
				// movableChs[i] = result + whichPlayerPiece;
			} 
		}

		for (int i = result; i < 6 && movableChs[1].symbol!='0'; i++) {
			if (currentPlayer[i].exist == 1){
				movableChs[1] = currentPlayer[result];
				// movableChs[1] = i + whichPlayerPiece;
			} 
		}

		for (int i = 0; i < 2; i++) {
			if (movableChs[i].symbol == '0') {
				movableChs[i] = movableChs[1-i];
				move--;
			}
		}
		return move;
	}
	// printf("num[0-5]: %d, ", result);
	// for (int i = 0; i < 2; i++) {
	// 	printf("%c ", movableChs[i]);
	// }
	// printf("\n");
}

int Game::updatePlayer(char c){

	// to sort out player`s chess.
	if (c != '0') {

		// should delete a single piece.
		if (this->turn == false) {

			// this Play is A
			currentPlayer[c-'1'].exist = 0; 

		}
		else {

			// this Play is B
			currentPlayer[c-'A'].exist = 0;
		}

	}
	else {

		// nothing to deal with.
	}

	// is this player been killed the game.
	for (int i = 0; i < 6; i++) {
		if (currentPlayer[i].exist == 1) {
			return 0;
		}
	}

	// if not return yet => the game end(one of the player have no more pieces)
	return (this->turn == false ? 2 : 1);
}

void Game::switchPlayer() {

	// rotate the board.
	// Game *rotateGame = new Game;

	// for (int i = 0; i < 5; i++)
	// 	for (int j = 0; j < 5; j++)
	// 		rotateGame->board[4-i][4-j] = Game.board[i][j];

	// // and coor of this player`s piece.
	// if (turn == 2) {
	// 	for () {

	// 	}
	// 	int rotateX = 4 - currentPlayer[/* a piece */].getX();
	// 	int rotateY	= 4 - currentPlayer[/* a piece */].getY();	
	// }
	
	currentPlayer = (this->turn == false? playerA : playerB);
}
