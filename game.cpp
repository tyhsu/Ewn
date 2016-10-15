#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "game.h"
using namespace std;

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
			else cout << c;
		}
		cout << "|" << endl;
	}
	cout << "     \\-----/" << endl;
	cout << "====================================" << endl;
}

void Game::play()
{
	while (1) {
		//ask for the next chessman to move
		int nextMoveCnt = availiableMove(rollTheDice());
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
		//char eatenChs = moveChess(this->movableChs[chs], direct);
		//while (eatenChs == '!') {
		//	cout << "Illegal move!!" << endl << endl;
		//	cout << "Next chessman able to move: ";
		//	for (int i=0; i<nextMoveCnt; i++) cout << i << ")" << this->movableChs[i].symbol << " ";
		//	cout << endl;
		//	cout << "Choose: ";
		//	cin >> chs;
		//	if (!this->turn)
		//		cout << "Direction: 0)Right 1)Down 2)Right-down" << endl;
		//	else
		//		cout << "Direction: 0)Left 1)Up 2)Left-up" << endl;
		//	cout << "Choose: ";
		//	cin >> direct;
		//	eatenChs = moveChess(this->movableChs[chs], direct);
		//}
		////update, 0(game continues), 1(A wins), 2(B wins)
		int win = move(chs, direct);
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

bool Game::isLegalMove(int x, int y) {
	return ( x >= 0 && x < 5 && y >= 0 && y < 5);
}

void Game::setBoard(Chess chs) {
	this->board[chs.x][chs.y] = chs.symbol;
}

char Game::getChessOnBoard(Chess chs) {
	return this->board[chs.x][chs.y];
}

int Game::move(int choice, int cmd) {
	//You'll know availiableMoveCnt in AI
	//int availiableMoveCnt = availiableMove()
	char eatenChs = moveChess(this->movableChs[choice], cmd);
	return updatePlayer(eatenChs);
}

char Game::moveChess(Chess chessToGo, int cmd) {
	char replacedChess;

	int direction = (this->turn == false)? 1 : -1;
	cout <<"currentPlayer :" << (this->turn ? "playerB" : "playerA") << endl;
	if (cmd == 0) {
		//move right
		if( this->isLegalMove(chessToGo.x + direction, chessToGo.y) ) {
			//clear previous location
			Chess previous = chessToGo;
			previous.symbol = 0;
			this->setBoard(previous);

			int playerIndex = 0;
			if (!this->turn)	//playerA
				playerIndex = chessToGo.symbol - '1';
			else				//playerB
				playerIndex = chessToGo.symbol - 'A';
			// for(int i = 0; i < 6; i++){
			// 	if (this->currentPlayer[i].symbol == chessToGo.symbol){
			// 		playerIndex = i;
			// 		break;
			// 	}
			// }
			cout<<"index: " << playerIndex << endl;
			this->currentPlayer[playerIndex].moveX(direction);
			replacedChess = this->getChessOnBoard(this->currentPlayer[playerIndex]);
			this->setBoard(this->currentPlayer[playerIndex]);
		}
		else {
			return '!';
		}
	}
	else if (cmd == 1) {
		//move down
		if (this->isLegalMove(chessToGo.x, chessToGo.y + direction)) {
			Chess previous = chessToGo;
			previous.symbol = 0;
			this->setBoard(previous);//clear previous location

			int playerIndex = 0;
			if (!this->turn)	//playerA
				playerIndex = chessToGo.symbol - '1';
			else				//playerB
				playerIndex = chessToGo.symbol - 'A';
			// for(int i = 0; i < 6; i++){
			// 	if (this->currentPlayer[i].symbol == chessToGo.symbol){
			// 		playerIndex = i;
			// 		break;
			// 	}
			// }
			cout<<"index: " << playerIndex << endl;
			this->currentPlayer[playerIndex].moveY(direction);
			replacedChess = this->getChessOnBoard(this->currentPlayer[playerIndex]);
			this->setBoard(this->currentPlayer[playerIndex]);
		}
		else {
			return '!';
		}
	}
	else if (cmd == 2) {
		//move right down
		if (this->isLegalMove(chessToGo.x + direction, chessToGo.y + direction)) {
			Chess previous = chessToGo;
			previous.symbol = 0;
			this->setBoard(previous);//clear previous location

			int playerIndex = 0;
			if (!this->turn)	//playerA
				playerIndex = chessToGo.symbol - '1';
			else				//playerB
				playerIndex = chessToGo.symbol - 'A';
			// for(int i = 0; i < 6; i++){
			// 	if (this->currentPlayer[i].symbol == chessToGo.symbol){
			// 		playerIndex = i;
			// 		break;
			// 	}
			// }
			cout<<"index: " << playerIndex << endl;
			this->currentPlayer[playerIndex].moveX(direction);
			this->currentPlayer[playerIndex].moveY(direction);
			replacedChess = this->getChessOnBoard(this->currentPlayer[playerIndex]);
			this->setBoard(this->currentPlayer[playerIndex]);
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

int Game::rollTheDice() {
	return rand()%6;
}

int Game::availiableMove(int dice) {

	printf("result: %d \n", dice+1);
	for(int i = 0; i < 2; i++)
		this->movableChs[i].assign(0, false, 0, 0);

	if (this->currentPlayer[dice].exist == true) {
		// this piece is alive!
		for(int i = 0; i < 2; i++) {
			this->movableChs[i] = this->currentPlayer[dice];
			// movableChs[i] = result + whichPlayerPiece;
		}
		return 1;
	}
	else {
		cout << "Chessman " << this->currentPlayer[dice].symbol << " doesn't exist" << endl;
		int move = 2;

		// search for maybe other two.
		for (int i = dice-1; i >= 0; i--) {
			if (this->currentPlayer[i].exist){
				this->movableChs[0] = this->currentPlayer[i];
				break;
				// movableChs[i] = result + whichPlayerPiece;
			}
		}

		for (int i = dice+1; i < 6; i++) {
			if (this->currentPlayer[i].exist){
				this->movableChs[1] = this->currentPlayer[i];
				break;
				// movableChs[1] = i + whichPlayerPiece;
			}
		}

		// if one of movableChs doesn`t value.
		// let this Chs = the other one (with value).
		for (int i = 0; i < 2; i++) {
			if (this->movableChs[i].symbol == 0) {
				this->movableChs[i] = this->movableChs[1-i];
				move--;
			}
		}
		return move;
	}
}

int Game::updatePlayer(char c){

	// to sort out player`s chess.
	if (c != 0) {
		// should delete a single piece.
		if (c < 'A') {
			// this Play is A
			this->playerA[c-'1'].exist = false;
		}
		else {
			// this Play is B
			this->playerB[c-'A'].exist = false;
		}
	}
	else {
		// nothing to deal with.
	}


	// check if player arrive the end-game Pos.
	for (int i = 0; i < 6; i++) {
		if(this->playerA[i].x == 4 && this->playerA[i].y == 4)
			return 1;
		if(this->playerB[i].x == 0 && this->playerB[i].y == 0)
			return 2;
	}

	// is this player been killed the game.

	if (this->currentPlayer == this->playerA) {
		for (int i = 0; i < 6; i++)
			if (this->playerB[i].exist == true)
				return 0;
	}
	else {
		for (int i = 0; i < 6; i++)
			if (this->playerA[i].exist == true)
				return 0;
	}


	// if not return yet => the game end(one of the player have no more pieces)
	// the current player loses, so the winner is the other player
	return (this->turn == true ? 1 : 2);
}

void Game::switchPlayer() {
	if (this->turn) {	//now is B
		this->currentPlayer = this->playerA;
		this->turn = false;
	}
	else {
		this->currentPlayer = this->playerB;
		this->turn = true;
	}
}

void Game::checkStatus()
{
	cout <<"/*    Player A    */" <<endl;
	for(int i=0; i<6; i++){
		cout << this->playerA[i].symbol << " is at (" << this->playerA[i].x << ", " << this->playerA[i].y << ")   [exist : " << this->playerA[i].exist << "] ;" <<endl;
	}
	cout <<"/*    --------    */" << endl;

	cout <<"/*    Player B    */" <<endl;
	for(int i=0; i<6; i++){
		cout << this->playerB[i].symbol << " is at (" << this->playerB[i].x << ", " << this->playerB[i].y << ")   [exist : " << this->playerB[i].exist << "] ;" <<endl;
	}
	cout <<"/*    --------    */" << endl;

}
