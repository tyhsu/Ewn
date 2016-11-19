#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
#include "game.h"
using namespace std;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* -------------------- struct Chess -------------------- */
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

/* -------------------- class Game -------------------- */
Game::Game()
{
	memset(this->board_, 0, sizeof(this->board_));
	memset(this->movableChs_, 0, sizeof(this->movableChs_));
	this->chsNumA_ = 6, this->chsNumB_ = 6;
	for (int i=0; i<6; i++)
		this->playerA_[i].assign('1'+i, true, 0, 0);
	for (int i=0; i<6; i++)
		this->playerB_[i].assign('A'+i, true, 0, 0);

	this->existA_ = 0b111111;
	this->existB_ = 0b111111;

	this->currentPlayer_ = playerA_;
	this->currentExist_ = existA_;
	this->turn_ = false;

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
			this->board_[posX][posY] = '1' + number;
			this->playerA_[number].x = posX;
			this->playerA_[number].y = posY;
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
			this->board_[posX][posY] = 'A' + number;
			this->playerB_[number].x = posX;
			this->playerB_[number].y = posY;
			currPos++;
			six = six ^ (1<<number);	//The used number is discarded
		}
	}
}

Game::Game(const Game& game)
{
	for(int i=0; i<5; i++) {
		for(int j=0; j<5; j++) {
			this->board_[i][j] = game.board_[i][j];
		}
	}
	for(int i=0; i<2; i++) {
		this->movableChs_[i] = game.movableChs_[i];
	}
	for(int i=0; i<6; i++) {
		this->playerA_[i] = game.playerA_[i];
		this->playerB_[i] = game.playerB_[i];
	}
	this->existA_ = game.existA_;
	this->existB_ = game.existB_;
	this->turn_ = game.turn_;
	this->currentPlayer_ = (this->turn_ == false) ? this->playerA_ :  this->playerB_;
	this->currentExist_ = (this->turn_ == false) ? this->existA_ :  this->existB_;
	this->chsNumA_ = game.chsNumA_;
	this->chsNumB_ = game.chsNumB_;
}

void Game::operator=(const Game& game)
{
	for(int i=0; i<5; i++) {
		for(int j=0; j<5; j++) {
			this->board_[i][j] = game.board_[i][j];
		}
	}
	for(int i=0; i<2; i++) {
		this->movableChs_[i] = game.movableChs_[i];
	}
	for(int i=0; i<6; i++) {
		this->playerA_[i] = game.playerA_[i];
		this->playerB_[i] = game.playerB_[i];
	}
	this->existA_ = game.existA_;
	this->existB_ = game.existB_;
	this->turn_ = game.turn_;
	this->chsNumA_ = game.chsNumA_;
	this->chsNumB_ = game.chsNumB_;
}

void Game::printBoard()
{
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "The current board:" << endl;
	cout << "     /-----\\" << endl;
	for (int i=0; i<5; i++) {
		cout << "     |";
		for (int j=0; j<5; j++) {
			char c = this->board_[j][i];
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

int Game::rollTheDice()
{
	return rand()%6;
}

int Game::availableMove(const int& dice)
{
	//printf("dice: %d \n", dice+1);
	for(int i = 0; i < 2; i++)
		this->movableChs_[i].assign(0, false, 0, 0);

	if (this->currentPlayer_[dice].exist == true) {
		// this piece is alive!
		for(int i = 0; i < 2; i++) {
			this->movableChs_[i] = this->currentPlayer_[dice];
		}
		return 1;
	}
	else {
		//cout << "Chessman " << this->currentPlayer_[dice].symbol << " doesn't exist" << endl;
		int move = 2;

		// search for maybe other two.
		for (int i = dice-1; i >= 0; i--) {
			if (this->currentPlayer_[i].exist){
				this->movableChs_[0] = this->currentPlayer_[i];
				break;
			}
		}
		for (int i = dice+1; i < 6; i++) {
			if (this->currentPlayer_[i].exist){
				this->movableChs_[1] = this->currentPlayer_[i];
				break;
			}
		}

		// if one of movableChs_ doesn't value, let this Chs = the other one (with value).
		for (int i = 0; i < 2; i++) {
			if (this->movableChs_[i].symbol == 0) {
				this->movableChs_[i] = this->movableChs_[1-i];
				move--;
			}
		}
		return move;
	}
}

bool Game::isLegalMove(const Movement& mvmt)
{
	Chess chess = this->currentPlayer_[mvmt.first];
	int posneg = (this->turn_ == false) ? 1 : -1;
	int x = chess.x, y = chess.y;
	x += (mvmt.second == 1) ? 0 : posneg;
	y += (mvmt.second == 0) ? 0 : posneg;
	return ( x >= 0 && x < 5 && y >= 0 && y < 5);
}

void Game::setBoard(const Chess& chs)
{
	this->board_[chs.x][chs.y] = chs.symbol;
}

char Game::getChessOnBoard(const Chess& chs)
{
	return this->board_[chs.x][chs.y];
}

int Game::update(const Movement& mvmt)
{
	Chess chessToGo = this->currentPlayer_[mvmt.first];
	char replacedChess;
	int direct = mvmt.second;
	int posneg = (this->turn_ == false)? 1 : -1;

	// Update the chosen movement on the board
	if (direct == 0) {
		/* ----------move right---------- */
		// clear previous location
		Chess previous = chessToGo;
		previous.symbol = 0;
		this->setBoard(previous);

		int playerIndex = 0;
		if (!this->turn_)	//playerA_
			playerIndex = chessToGo.symbol - '1';
		else				//playerB_
			playerIndex = chessToGo.symbol - 'A';
		this->currentPlayer_[playerIndex].moveX(posneg);
		replacedChess = this->getChessOnBoard(this->currentPlayer_[playerIndex]);
		this->setBoard(this->currentPlayer_[playerIndex]);
	}
	else if (direct == 1) {
		/* ----------move down---------- */
		// clear previous location
		Chess previous = chessToGo;
		previous.symbol = 0;
		this->setBoard(previous);

		int playerIndex = 0;
		if (!this->turn_)	//playerA_
			playerIndex = chessToGo.symbol - '1';
		else				//playerB_
			playerIndex = chessToGo.symbol - 'A';
		this->currentPlayer_[playerIndex].moveY(posneg);
		replacedChess = this->getChessOnBoard(this->currentPlayer_[playerIndex]);
		this->setBoard(this->currentPlayer_[playerIndex]);

	}
	else if (direct == 2) {
		/* ----------move right down---------- */
		// clear previous location
		Chess previous = chessToGo;
		previous.symbol = 0;
		this->setBoard(previous);

		int playerIndex = 0;
		if (!this->turn_)	//playerA_
			playerIndex = chessToGo.symbol - '1';
		else				//playerB_
			playerIndex = chessToGo.symbol - 'A';
		this->currentPlayer_[playerIndex].moveX(posneg);
		this->currentPlayer_[playerIndex].moveY(posneg);
		replacedChess = this->getChessOnBoard(this->currentPlayer_[playerIndex]);
		this->setBoard(this->currentPlayer_[playerIndex]);
	}

	// Update the eaten chessman
	if (replacedChess != 0) {
		// should delete a single piece.
		if (replacedChess < 'A') {
			// this Play is A
			this->playerA_[replacedChess-'1'].exist = false;
			this->existA_ -= 1<<int(replacedChess-'1');
			this->chsNumA_--;
		}
		else {
			// this Play is B
			this->playerB_[replacedChess-'A'].exist = false;
			this->existB_-= 1<<int(replacedChess-'A');
			this->chsNumB_--;
		}
	}

	// Check if any player arrives the end-game position
	for (int i = 0; i < 6; i++) {
		if(this->playerA_[i].x == 4 && this->playerA_[i].y == 4)
			return 1;
		if(this->playerB_[i].x == 0 && this->playerB_[i].y == 0)
			return 2;
	}

	// Check if the other player is killed the game.
	if (!this->turn_ && this->chsNumB_==0)		//this player is A
		return 1;
	else if (this->turn_ && this->chsNumA_==0)	//this player is B
		return 2;

	//The game has not come to a close
	return 0;
}

void Game::switchPlayer()
{
	if (this->turn_) {	//now is B
		this->currentPlayer_ = this->playerA_;
		this->currentExist_ = this->existA_;
		this->turn_ = false;
	}
	else {				//now is A
		this->currentPlayer_ = this->playerB_;
		this->currentExist_ = this->existB_;
		this->turn_ = true;
	}
}

void Game::checkStatus()
{
	cout <<"/*    Player A    */" <<endl;
	for(int i=0; i<6; i++){
		cout << this->playerA_[i].symbol << " is at (" << this->playerA_[i].x << ", " << this->playerA_[i].y << ")   [exist : " << this->playerA_[i].exist << "] ;" <<endl;
	}
	cout <<"/*    --------    */" << endl;

	cout <<"/*    Player B    */" <<endl;
	for(int i=0; i<6; i++){
		cout << this->playerB_[i].symbol << " is at (" << this->playerB_[i].x << ", " << this->playerB_[i].y << ")   [exist : " << this->playerB_[i].exist << "] ;" <<endl;
	}
	cout <<"/*    --------    */" << endl;
}

Chess Game::getMovableChs(const int& k)
{
	return this->movableChs_[k];
}

Chess Game::getCurrPlayer(const int& k)
{
	return this->currentPlayer_[k];
}

int Game::getCurrentExist() {
	return currentExist_;
}

bool Game::getTurn()
{
	return this->turn_;
}
