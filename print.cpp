#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
using namespace std;

void Chess::moveX()
{
	x++;
}

void Chess::moveY()
{
	y++;
}

Game::Game()
{
	memset(this->board, 0, sizeof(this->board));
	memset(this->movableChs, 0, sizeof(this->movableChs));
	chsNumA = 0, chsNumB = 0;
	memset(this->playerA, 0, sizeof(this->playerA));
	memset(this->playerB, 0, sizeof(this->playerB));
	currentPlayer = playerA;
	turn = false;

	//Initialize the positions of the chessmen
	//A: 1~6
	int currPos = 0;
	int position[6][2] = { {0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {2,0} };
	int six = 0x111111;
	srand( time(NULL) );
	//Every position selects a number
	while (six!=0) {
		int number = rand()%6;	//0~6
		if ((six & (1<<number)) >> number) {	//The number hasn't been used
			board[ position[currPos][0] ][ position[currPos][1] ] = '1' + number;
			currPos++;
			six = six ^ (1<<number);	//The used number is discarded
		}
	}
	//B: A~F
	currPos = 0;
	position = { {4,4}, {4,3}, {4,2}, {3,4}, {3,3}, {2,4} };
	six = 0x111111;
	srand( time(NULL) );
	while (six!=0) {
		int number = rand()%6;	//0~6
		if ((six & (1<<number)) >> number) {	//The number hasn't been used
			board[ position[currPos][0] ][ position[currPos][1] ] = 'A' + number;
			currPos++;
			six = six ^ (1<<number);	//The used number is discarded
		}
	}
}

void Game::printBoard()
{
	cout << "------------------------------------" << endl;
	cout << "The current board:" << endl;
	if (!this->turn) {
		for (int i=0; i<5; i++) {
			cout << this->board[i][0];
			for (int j=0; j<5; j++) {
				cout << " " << this->board[i][j];
			}
			cout << endl;
		}
	}
	else {
		for (int i=4; i>=0; i--) {
			cout << this->board[i][4];
			for (int j=3; j>=0; j--) {
				cout << " " << this->board[i][j];
			}
			cout << endl;
		}
	}
	cout << "====================================" << endl;
}