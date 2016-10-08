#include <iostream>
using namespace std;

int currentPlayer[6] = {0,0,1,0,1,0};

int nextMove() {

	srand(time(NULL));
	int a[2] = {0,0};
	int result = rand()%6+1;	

	if (currentPlayer[result-1] == 1) {

		// this piece is alive!
		for(int i = 0; i < 2; i++) a[i] = result;
		return 1;
	}
	else {
		int move = 2;
		// search for maybe other two.
		for (int i = 1; i < result; i++) {
			currentPlayer[i-1] == 1? a[0] = i: 0;
		}

		for (int i = result+1; i <= 6 && a[1]==0; i++) {
			currentPlayer[i-1] == 1? a[1] = i : 0;
		}

		for (int i = 0; i < 2; i++) {
			if (a[i] == 0) {
				a[i] = a[1-i];
				move--;
			}
		}
		return move;
	}
	// printf("num: %d, ", result);
	// for (int i = 0; i < 2; i++) {
	// 	printf("%d ", a[i]);
	// }
	// printf("\n");
}

// int updatePlayer(char c){

// 	// to sort out player`s chess.
// 	if (c != '0') {

// 		// should delete a single piece.
// 		if (turn == true) {

// 			// this Play is A
// 			currentPlayer[c-'1'].exist = 0; 

// 		}
// 		else {

// 			// this Play is B
// 			currentPlayer[c-'A'].exist = 0;
// 		}

// 	}
// 	else {

// 		// nothing to deal with.
// 	}

// 	// is this player been killed the game.
// 	for (int i = 0; i < 6; i++) {
// 		if (currentPlayer[i].exist == 1) {
// 			return false;
// 		}
// 	}

// 	return true;
// }
