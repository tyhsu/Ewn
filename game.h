#include <iostream>
#include <map>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
using namespace std;

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
public:
	char board_[5][5];
	Chess movableChs_[2];
	//How many chessmen are on the board
	int chsNumA_, chsNumB_;
	Chess playerA_[6], playerB_[6];
	Chess* currentPlayer_;
	//false(0): A, true(1): B
	bool turn_;

	//Including initializing the positions
	Game();
	//Print the board
	void printBoard();
	//One player plays the game (the integration)
	Movement playerPlay(int dice);
	//One AI plays the game (without std input)
	//int autoPlay(int dice);
	//Two players fight!
	void twoPlayers();
	//One player fight with AI
	//void playerAI();
	//Two AIs fight
	//void twoAIs();

	//Check if the chessman is out of the board
	bool isLegalMove(Movement mvmt);
	//Synchronize the chessman and the board
	void setBoard(Chess chs);
	//Get the symbol of the chessman originally on the board
	char getChessOnBoard(Chess chs);
	//Player has a move according to the assigned chessman and direction
	//char moveChess(Chess chessToGo, int cmd);
	//Make a move includes choosing a chessman and move the chessman
	//You should know whether the cmd is legal (for AI)
	//int move(int choice, int cmd);

	//Roll the dice
	int rollTheDice();
	//Input the result of dice and return how many chessmen the player can move
	int availiableMove(int dice);
	//Update the board(have a movement), chsNum, playerA/playerB. Return 0(game continues), 1(A wins), 2(B wins)
	int update(Movement mvmt);
	//Change turn and the pointer of the player
	void switchPlayer();
	//Print the position and existence of the both players
	void checkStatus();
};
