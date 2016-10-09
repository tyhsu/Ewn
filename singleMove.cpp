#include <iostream>

bool isLegalMove(int y, int x) {
	return ( x >= 0 && x < 5 && y >= 0 && y < 5);
}

void setBoard(Chess chs) {
	board[chs.getY()][chs.getX()] = chs.symbol;
}

char getChessOnBoard(Chess chs) {
	return board[chs.getY()][chs.getX()];
}

char moveChess(Chess chessToGo, int cmd) {
	char replacedChess;
	int direction = (turn == true) 1 : -1;
	if (cmd == 0) {
		//move right
		if( isLegalMove(chessToGo.getY() + direction, chessToGo.getX()) ) {
			chessToGo.moveY();
			replacedChess = getChessOnBoard(chessToGo);
			setBoard(chessToGo);
		}
		else {
			return '!';
		}
	}
	else if (cmd == 1) {
		//move down
		if (isLegalMove(chessToGo.getY(), chessToGo.getX() + direction)) {
			chessToGo.moveX();
			replacedChess = getChessOnBoard(chessToGo);
			setBoard(chessToGo);
		}
		else {
			return '!';
		}
	}
	else if (cmd == 2) {
		//move right down
		if (isLegalMove(chessToGo.getY() + direction, chessToGo.getX() + direction)) {
			chessToGo.moveX();
			chessToGo.moveY();
			replacedChess = getChessOnBoard(chessToGo);
			setBoard(chessToGo);
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
