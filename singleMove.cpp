#include <iostream>

bool IsLegalMove(int x, int y) {
  return ( x >= 0 && x < 5 && y >= 0 && y < 5);
}

void SetBoard(chess chs) {
  board[chs.GetX][chs.GetY] = chs;
}

char GetChessOnBoard(chess chs) {
  return board[chessToGo.GetX][chessToGo.GetY];
}

char OneStep(chess chessToGo) {
  char cmd;
  char replacedChess;
  cin >> cmd;
  if( cmd == '0') {
    //move right
    if( IsLegalMove(chessToGo.GetX + 1, chessToGo.GetY) ) {
      chessToGo.moveX();
      replacedChess = GetChessOnBoard(chessToGo);
      SetBoard(chessToGo);
    }
    else {
      cout << "Illegal move !" << endl;
      return '!';
    }
  }
  else if( cmd == '1') {
    //move down
    if( IsLegalMove(chessToGo.GetX, chessToGo.GetY + 1) ) {
      chessToGo.moveY();
      replacedChess = GetChessOnBoard(chessToGo);
      SetBoard(chessToGo);
    }
    else {
      cout << "Illegal move !" << endl;
      return '!';
    }
  }
  else if( cmd == '2') {
    //move right down
    if( IsLegalMove(chessToGo.GetX + 1, chessToGo.GetY + 1) ) {
      chessToGo.moveX();
      chessToGo.moveY();
      replacedChess = GetChessOnBoard(chessToGo);
      SetBoard(chessToGo);
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