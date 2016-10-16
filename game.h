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
	//How many chessmen are on the board
	int chsNumA, chsNumB;
	Chess playerA[6], playerB[6];
	Chess* currentPlayer;
	//false(0): A, true(1): B
	bool turn;

	//Including initializing the positions
	Game();
	//Print the board
	void printBoard();
	//One player plays the game (the integration)
	void playerPlay();
	//One AI plays the game (without std input)
	void autoPlay();
	//Two players fight!
	void twoPlayers();
	//One player fight with AI
	void playerAI();
	//Two AIs fight
	void twoAIs();

	//Check if the chessman is out of the board
	bool isLegalMove(int x, int y);
	//Synchronize the chessman and the board
	void setBoard(Chess chs);
	//Get the symbol of the chessman originally on the board
	char getChessOnBoard(Chess chs);
	//Player has a move according to the assigned chessman and direction
	char moveChess(Chess chessToGo, int cmd);
	//Make a move includes choosing a chessman and move the chessman
	//You should know whether the cmd is legal (for AI)
	int move(int choice, int cmd);
	
	//Roll the dice
	int rollTheDice();
	//Input the result of dice and return how many chessmen the player can move
	int availiableMove(int dice);
	//Recieve the eaten chessman and update the existence, and return 0(game continues), 1(A wins), 2(B wins)
	int updatePlayer(char c);
	//Rotate the board and change the pointer of the player
	void switchPlayer();
	//Check chesses
	void checkStatus();
};
