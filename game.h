#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
using namespace std;

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
	char board[5][5];
	Chess movableChs[2];
	int chsNumA, chsNumB;
	Chess playerA[6], playerB[6];
	Chess* currentPlayer;
	//false(0): A, true(1): B
	bool turn;

	//Including initializing the positions
	Game();
	//Print the board
	void printBoard();
	//Play the game (the integration)
	void play();

	//Check if the chessman is out of the board
	bool isLegalMove(int x, int y);
	//Synchronize the chessman and the board
	void setBoard(Chess chs);
	//Get the symbol of the chessman originally on the board
	char getChessOnBoard(Chess chs);

	//Make a move includes choosing a chessman and move the chessman      You should know whether the cmd is legal
	int move(int choice, int cmd);
	//Player move a chessman
	char moveChess(Chess chessToGo, int cmd);
	//Roll the dice
	int rollTheDice();
	//Return how many chessmen the player can move
	int availiableMove(int dice);
	//Recieve the eaten chessman and update the existence, and return 0(game continues), 1(A wins), 2(B wins)
	int updatePlayer(char c);
	//Rotate the board and change the pointer of the player
	void switchPlayer();
	//Check chesses
	void checkStatus();
	//R
};
