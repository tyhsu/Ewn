#include <iostream>

bool isLegalMove(int x, int y) {
	return ( x >= 0 && x < 5 && y >= 0 && y < 5);
}

void setBoard(Chess chs) {
	board[chs.x][chs.y] = chs.symbol;
}

char getChessOnBoard(Chess chs) {
	return board[chs.x][chs.y];
}

char moveChess(Chess chessToGo) {
	char cmd;
	char replacedChess;
	cin >> cmd;
	if(cmd == '0') {
		//move right
		if( isLegalMove(chessToGo.x + 1, chessToGo.y) ) {
			chessToGo.moveX();
			replacedChess = getChessOnBoard(chessToGo);
			setBoard(chessToGo);
		}
		else {
			cout << "Illegal move !" << endl;
			return '!';
		}
	}
	else if( cmd == '1') {
		//move down
		if( isLegalMove(chessToGo.x, chessToGo.y + 1) ) {
			chessToGo.moveY();
			replacedChess = getChessOnBoard(chessToGo);
			setBoard(chessToGo);
		}
		else {
			cout << "Illegal move !" << endl;
			return '!';
		}
	}
	else if( cmd == '2') {
		//move right down
		if( isLegalMove(chessToGo.x + 1, chessToGo.y + 1) ) {
			chessToGo.moveX();
			chessToGo.moveY();
			replacedChess = getChessOnBoard(chessToGo);
			setBoard(chessToGo);
		}
		else {
			cout << "Illegal move !" << endl;
			return '!';
		}
	}
	else {
		cout << "Illigal command !" << endl;
		return '!';
	}

	return replacedChess;
}