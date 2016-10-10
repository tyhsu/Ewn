#include <iostream>

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
	int direction = (turn == true)? 1 : -1;
	if (cmd == 0) {
		//move right
		if( this->isLegalMove(chessToGo.y + direction, chessToGo.x) ) {
			Chess previous = chessToGo;
			previous.symbol = 0;
			this->setBoard(previous);//clear previous location

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
