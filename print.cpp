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
	while (currPos<6) {
		int number = rand()%6;	//0~6
		if ((six & (1<<number)) >> number) {	//The number hasn't been used
			this->board[ position[currPos][0] ][ position[currPos][1] ] = '1' + number;
			currPos++;
			six = six ^ (1<<number);	//The used number is discarded
		}
	}
	//B: A~F
	currPos = 0;
	position = { {4,4}, {4,3}, {4,2}, {3,4}, {3,3}, {2,4} };
	six = 0x111111;
	srand( time(NULL) );
	while (currPos<6) {
		int number = rand()%6;	//0~6
		if ((six & (1<<number)) >> number) {	//The number hasn't been used
			this->board[ position[currPos][0] ][ position[currPos][1] ] = 'A' + number;
			currPos++;
			six = six ^ (1<<number);	//The used number is discarded
		}
	}
	printBoard();
}

void Game::printBoard()
{
	cout << "------------------------------------" << endl;
	cout << "The current board:" << endl;
	for (int i=0; i<5; i++) {
		for (int j=0; j<5; j++)
			cout << this->board[i][j];
		cout << endl;
	}
	cout << "====================================" << endl;
}

void Game::play()
{
	while (1) {
		//ask for the next chessman to move
		int nextMoveCnt = nextMove();
		cout << "Next chessman able to move: "
		for (int i=0; i<nextMoveCnt; i++) cout << i << ":" << ewn.movableChs[i].symbol << " ";
		cout << endl;
		cout << "Choose: ";
		int chs, direct;
		cin >> chs;
		if (!turn)
			cout << "Direction: 0.Right 1.Down 2.Right-down" << endl;
		else
			cout << "Direction: 0.Left 1.Up 2.Left-up" << endl;
		cout << "Choose: ";
		cin >> direct;
		char eatenChs = moveChess(ewn.movableChs[chs], direct);
		while (eatenChs == '!') {
			cout << "Illegal move!!" << endl << endl;
			if (!turn)
				cout << "Direction: 0.Right 1.Down 2.Right-down" << endl;
			else
				cout << "Direction: 0.Left 1.Up 2.Left-up" << endl;
			cout << "Choose: ";
			eatenChs = moveChess(ewn.movableChs[chs], direct);
		}
		//update, 0(game continues), 1(A wins), 2(B wins)
		int win = updatePlayer(eatenChs);
		printBoard();
		if (win!=0) return;
	}
}