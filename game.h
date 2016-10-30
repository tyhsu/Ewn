#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
using namespace std;

// < the index of the currentPlayer, direction >
typedef pair<int, int> Movement;

struct Chess
{
	char symbol;
	bool exist;
	int x, y;
	void assign(const char& s, const bool& e, const int& x, const int& y);
	void moveX(int direction);
	void moveY(int direction);
};

class Game
{
private:
	char board_[5][5];
	Chess movableChs_[2];
	//How many chessmen are on the board
	int chsNumA_, chsNumB_;
	Chess playerA_[6], playerB_[6];
	Chess* currentPlayer_;
	//false(0): A, true(1): B
	bool turn_;


public:
	//Including initializing the positions
	Game();
	//Copy constructor
	Game(const Game& game);
	//Copy operator
	void operator=(const Game& game);

	//Print the board
	void printBoard();
	//Roll the dice
	int rollTheDice();
	//Input the result of dice and return how many chessmen the player can move
	int availableMove(const int& dice);
	//Check if the chessman is out of the board
	bool isLegalMove(const Movement& mvmt);
	//Synchronize the chessman and the board
	void setBoard(const Chess& chs);
	//Get the symbol of the chessman originally on the board
	char getChessOnBoard(const Chess& chs);
	//Update the board(have a movement), chsNum, playerA/playerB. Return 0(game continues), 1(A wins), 2(B wins)
	int update(const Movement& mvmt);
	//Change turn and the pointer of the player
	void switchPlayer();
	//Print the position and existence of the both players
	void checkStatus();

	Chess getMovableChs(const int& k);
	Chess getCurrPlayer(const int& k);
	bool getTurn();
};

#endif
