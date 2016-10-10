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
	this->chsNumA = 0, this->chsNumB = 0;
	for (int i=0; i<6; i++)
		this->playerA[i].assign('1'+i, true, 0, 0);
	for (int i=0; i<6; i++)
		this->playerB[i].assign('A'+i, true, 0, 0);
	this->currentPlayer = playerA;
	this->turn = false;

	//Initialize the positions of the chessmen
	//A: 1~6
	int currPos = 0;
	int positionA[6][2] = { {0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {2,0} };
	int six = 0x3f;	//11,1111(2)
	srand( time(NULL) );
	//Every position selects a number
	while (currPos<6) {
		int number = rand()%6;	//0~5
		if ((six & (1<<number)) >> number) {	//The number hasn't been used
			int posX = positionA[currPos][0], posY = positionA[currPos][1];
			this->board[posX][posY] = '1' + number;
			this->playerA[number].x = posX;
			this->playerA[number].y = posY;
			currPos++;
			six = six ^ (1<<number);	//The used number is discarded
		}
	}
	//B: A~F
	currPos = 0;
	int positionB[6][2] = { {4,4}, {4,3}, {4,2}, {3,4}, {3,3}, {2,4} };
	six = 0x3f;	//11,1111(2)
	srand( time(NULL) );
	while (currPos<6) {
		int number = rand()%6;	//0~6
		if ((six & (1<<number)) >> number) {	//The number hasn't been used
			int posX = positionB[currPos][0], posY = positionB[currPos][1];
			this->board[posX][posY] = 'A' + number;
			this->playerB[number].x = posX;
			this->playerB[number].y = posY;
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
		for (int j=0; j<5; j++) {
			char c = this->board[i][j];
			if (c==0) cout << " ";
			else cout << this->board[i][j];
		}
		cout << endl;
	}
	cout << "====================================" << endl;
}

void Game::play()
{
	while (1) {
		//ask for the next chessman to move
		int nextMoveCnt = nextMove();
		cout << "Next chessman able to move= "
		for (int i=0; i<nextMoveCnt; i++) cout << i << ":" << ewn.movableChs[i].symbol << " ";
		cout << endl;
		cout << "Choose: ";
		int chs, direct;
		cin >> chs;
		if (!turn)
			cout << "Direction= 0:Right 1:Down 2:Right-down" << endl;
		else
			cout << "Direction= 0:Left 1:Up 2:Left-up" << endl;
		cout << "Choose: ";
		cin >> direct;
		char eatenChs = moveChess(ewn.movableChs[chs], direct);
		while (eatenChs == '!') {
			cout << "Illegal move!!" << endl << endl;
			if (!turn)
				cout << "Direction= 0:Right 1:Down 2:Right-down" << endl;
			else
				cout << "Direction= 0:Left 1:Up 2:Left-up" << endl;
			cout << "Choose: ";
			eatenChs = moveChess(ewn.movableChs[chs], direct);
		}
		//update, 0(game continues), 1(A wins), 2(B wins)
		int win = updatePlayer(eatenChs);
		printBoard();
		if (win!=0) {
			cout << "====================================" << endl;
			if (win==1) cout << "A is the winner!!!" << endl;
			else cout << "B is the winner!!!" << endl;
			cout << "====================================" << endl;
			return;
		}
	}
}