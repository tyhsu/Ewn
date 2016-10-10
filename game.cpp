#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "game.h"
using namespace std;

void Chess::assign(const char& s, const bool& e, const int& x, const int& y)
{
	this->symbol = s;
	this->exist = e;
	this->x = x;
	this-> y = y;
}

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
		cout << "Next chessman able to move= ";
		for (int i=0; i<nextMoveCnt; i++) cout << i << ":" << this->movableChs[i].symbol << " ";
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
		char eatenChs = moveChess(this->movableChs[chs], direct);
		while (eatenChs == '!') {
			cout << "Illegal move!!" << endl << endl;
			if (!turn)
				cout << "Direction= 0:Right 1:Down 2:Right-down" << endl;
			else
				cout << "Direction= 0:Left 1:Up 2:Left-up" << endl;
			cout << "Choose next chessman: ";
			cin >> chs;
			cout << "Choose the direction: ";
			cin >> direct;
			eatenChs = moveChess(this->movableChs[chs], direct);
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

bool Game::isLegalMove(int y, int x) {
	return ( x >= 0 && x < 5 && y >= 0 && y < 5);
}

void Game::setBoard(Chess chs) {
	this->board[chs.y][chs.x] = chs.symbol;
}

char Game::getChessOnBoard(Chess chs) {
	return this->board[chs.y][chs.x];
}
char Game::moveChess(Chess chessToGo, int cmd) {
	char replacedChess;
	int direction = (this->turn == false)? 1 : -1;
	if (cmd == 0) {
		//move right
		if( this->isLegalMove(chessToGo.y + direction, chessToGo.x) ) {
			chessToGo.moveY();
			replacedChess = this->getChessOnBoard(chessToGo);
			this->setBoard(chessToGo);
		}
		else {
			return '!';
		}
	}
	else if (cmd == 1) {
		//move down
		if (this->isLegalMove(chessToGo.y, chessToGo.x + direction)) {
			chessToGo.moveX();
			replacedChess = this->getChessOnBoard(chessToGo);
			this->setBoard(chessToGo);
		}
		else {
			return '!';
		}
	}
	else if (cmd == 2) {
		//move right down
		if (this->isLegalMove(chessToGo.y + direction, chessToGo.x + direction)) {
			chessToGo.moveX();
			chessToGo.moveY();
			replacedChess = this->getChessOnBoard(chessToGo);
			this->setBoard(chessToGo);
		}
		else {
			return '!';
		}
	}
	else {
		return '!';
	}

	return replacedChess;
}

int Game::nextMove() {

	srand(time(NULL));
	int result = rand()%6;

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
	currentPlayer = (this->turn == false? playerA : playerB);
}
