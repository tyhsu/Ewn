#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#include "game.h"
using namespace std;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void Chess::assign(const char& s, const bool& e, const int& x, const int& y)
{
	this->symbol = s;
	this->exist = e;
	this->x = x;
	this->y = y;
}

void Chess::moveX(int direction)
{
	this->x += direction;
}

void Chess::moveY(int direction)
{
	this->y += direction;
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
	srand( time(NULL) + getpid() );
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
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "The current board:" << endl;
	cout << "     /-----\\" << endl;
	for (int i=0; i<5; i++) {
		cout << "     |";
		for (int j=0; j<5; j++) {
			char c = this->board[j][i];
			if (c==0) cout << " ";
			else if (c >= 'A' && c <= 'F')
				cout << ANSI_COLOR_RED << c << ANSI_COLOR_RESET;
			else
				cout << ANSI_COLOR_YELLOW << c << ANSI_COLOR_RESET;
			//else cout << c;
		}
		cout << "|" << endl;
	}
	cout << "     \\-----/" << endl;
	cout << "====================================" << endl;
}

int Game::playerPlay(int dice) {
	int nextMoveCnt = availiableMove(dice);
	cout << "Next chessman able to move: ";
	for (int i=0; i<nextMoveCnt; i++) cout << i << ")" << this->movableChs[i].symbol << " ";
	cout << endl;
	cout << "Choose: ";
	int chs, direct;
	cin >> chs;
	if (!this->turn)
		cout << "Direction: 0)Right 1)Down 2)Right-down" << endl;
	else
		cout << "Direction: 0)Left 1)Up 2)Left-up" << endl;
	cout << "Choose: ";
	cin >> direct;
	char eatenChs = moveChess(this->movableChs[chs], direct);
	while (eatenChs == '!') {
		cout << "Illegal move!!" << endl << endl;
		cout << "Next chessman able to move: ";
		for (int i=0; i<nextMoveCnt; i++) cout << i << ")" << this->movableChs[i].symbol << " ";
		cout << endl;
		cout << "Choose: ";
		cin >> chs;
		if (!this->turn)
			cout << "Direction: 0)Right 1)Down 2)Right-down" << endl;
		else
			cout << "Direction: 0)Left 1)Up 2)Left-up" << endl;
		cout << "Choose: ";
		cin >> direct;
		eatenChs = moveChess(this->movableChs[chs], direct);
	}
	return updatePlayer(eatenChs) ;
}

void Game::autoPlay(int dice) {

	// Run AI and get chosen.
	return move(chosenChessman, chosenDirection);
}

void Game::twoPlayers() {
	while (1) {
		//ask for the next chessman to move

		//update, 0(game continues), 1(A wins), 2(B wins)
		//int win = move(chs, direct);
		int dice = rollTheDice();
		int win = playerPlay(dice);
		cout << "turn, win :" << turn << " " << win << endl;
		checkStatus();
		printBoard();
		if (win!=0) {
			cout << "====================================" << endl;
			if (win==1) cout << "A is the winner!!!" << endl;
			else cout << "B is the winner!!!" << endl;
			cout << "====================================" << endl;
			return;
		}
		switchPlayer();
	}
}

void Game::playerAI() {
	while (1) {
		//ask for the next chessman to move

		//update, 0(game continues), 1(A wins), 2(B wins)
		//int win = move(chs, direct);
		int dice = rollTheDice();
		int win =(this->turn == true )? autoPlay(dice) : playerPlay(dice);
		cout << "turn, win :" << turn << " " << win << endl;
		checkStatus();
		printBoard();
		if (win!=0) {
			cout << "====================================" << endl;
			if (win==1) cout << "A is the winner!!!" << endl;
			else cout << "B is the winner!!!" << endl;
			cout << "====================================" << endl;
			return;
		}
		switchPlayer();
	}
}

void Game::twoAIs() {
	while (1) {
		//ask for the next chessman to move

		//update, 0(game continues), 1(A wins), 2(B wins)
		//int win = move(chs, direct);
		int dice = rollTheDice();
		int win = autoPlay(dice);
		printBoard();
		if (win!=0) {
			cout << "====================================" << endl;
			if (win==1) cout << "A is the winner!!!" << endl;
			else cout << "B is the winner!!!" << endl;
			cout << "====================================" << endl;
			return;
		}
		switchPlayer();
	}
}
