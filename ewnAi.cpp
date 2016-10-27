#include <iostream>
#include <cstdio>
#include "game.h"
#include "ewnAi.h"

// Minimax
// function minimax(board, height, maximizingPlayer)
// 	if height==0	//node is a terminal node
// 	{
// 		loop though(player)
// 		return the feature value of node	//heuristic value
// 	}

// 	curBoard := board	//copy
// 	if maximizingPlayer
// 		bestValue := -∞
// 		// according to maximizingPlayer, know which player to play
// 		for each possible move chs, dirct	//each child of node
// 			nxtBoard := curBoard	//copy
// 			nxtBoard.move(chs, dirct)
// 			val := minimax(nxtBoard, height-1, FALSE)
// 			bestValue := max(bestValue, val);
// 		return bestValue
// 	else
// 		bestValue := +∞
// 		for each possible move chs, dirct
// 			nxtBoard := curBoard	//copy
// 			nxtBoard.move(chs, dirct)
// 			val := minimax(nxtBoard, height-1, TRUE)
// 			bestValue := min(bestValue, val);
// 		return bestValue

// (* Initial call for maximizing player *)
// minimax(origin, height, TRUE)

int EwnAI::minimax(Game& currentGame, int height) {

  // check if end;
  if (height == 0) {
    return evaluate(currentGame);
  }
  else {
    height--;
  }

  int bestValue = 0;
  // if false(0): A`s turn, find max, else find min.
  if (current_game.getTurn() == false) {
    // nextCnt
    bestValue = -1e9;
    // for #chess and direction
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 3; j++) {
        Game next_step = current_game;

        next_step.update(i,j);

        // switch player;
        next_step.switchPlayer();

        // return child value.
        int child_value = minimax(next_step, height-1);
        bestValue = min(bestValue, child_value);
      }
    }
    return bestValue;
  }
  else {

    bestValue = 1e9;
    // for #chess and direction
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 3; j++) {
        Game next_step = current_game;
        next_step.move(i,j);

        // switch player;
        next_step.switchPlayer();

        // return child value.
        int child_value = minimax(next_step, height-1);
        bestValue = min(bestValue, child_value);
      }
    }
    return bestValue;
  }
}

int EwnAI::evaluate(Game& current_game) {
  int val = 0;

  for(int dice = 0; dice < current_game.currentPlayer_; dice++) {
    if(current_game.currentPlayer_[dice].exist) {
      Pos temp;
      temp.first = current_game.currentPlayer_[dice].x;
      temp.second = current_game.currentPlayer_[dice].y;
      val += hv[temp];
    }
  }
  return val;
}

EwnAI::EwnAI() {
  // pos: a <int,int> , <x,y> coord;
  // insert to map => hv;
  for (int i = 0; i < 5; i++) {
    for (int j = 0;j < 5; j++) {

      // get the smallest of (x,y)
      int k = (i > j ? j : i);

      k = k * k;
      Pos chess_value;
      chess_value.first = i;
      chess_value.second = j;

      hv.insert(make_pair(chess_value, k));
    }
  }
}
