#include <iostream>
#include <cstdio>
#include <algorithm>
#include "game.h"
#include "ewnAi.h"
const int HEIGHT = 4;

EwnAI::EwnAI() {
	// pos: a <int,int> , <x,y> coord;
	// insert to map => hv;
	for (int i = 0; i < 5; i++) {
		for (int j = 0;j < 5; j++) {

			// get the smallest of (x,y)
			int k = (i > j ? j : i);

			k = k * k;
			Pos chess_pos;
			chess_pos.first = i;
			chess_pos.second = j;

			hv_.insert(make_pair(chess_pos, k));
		}
	}
}

int EwnAI::beforeMinimax(Game currentGame, int dice) {
	aiTurn_ = currentGame.getTurn();
	int nextMoveCnt = currentGame.availiableMove(dice);
	for (int i=0; i<nextMoveCnt; i++) {
		for (int direct = 0; direct < 3; direct++) {
			int chs = (currentGame.getTurn() == false) ? currentGame.getMovableChs(chs).symbol - '1' : currentGame.getMovableChs(chs).symbol - 'A';

			Movement mvmt(chs, direct);
			if (currentGame.isLegalMove(mvmt)) {
				minimax(currentGame, HEIGHT);
			}
		}
	}
	choose max(x, y)
}

int EwnAI::minimax(Game& currentGame, int height) {
	// check if end;
	if (height == 0) {
		return evaluate(currentGame);
	}

	int bestValue;
	if (currentGame.getTurn() == aiTurn_) {
		// ai's turn, find the max
		bestValue = -1e9;
		// for #chess and direction
		for (int chs = 0; chs < 6; chs++) {
			if (currentGame.getCurrPlayer(chs).exist) {
				for (int direct = 0; direct < 3; direct++) {
					Movement mvmt(chs, direct);
					if (currentGame.isLegalMove(mvmt)) {
						Game nextStep = currentGame;
						nextStep.update(mvmt);
						nextStep.switchPlayer();

						// return child value.
						int childValue = minimax(nextStep, height-1);
						bestValue = max(bestValue, childValue);
					}
				}
			}
		}
    }
	else {
		// the opponent's turn, find the min
		bestValue = 1e9;
		// for #chess and direction
		for (int chs = 0; chs < 6; chs++) {
			if (currentGame.getCurrPlayer(chs).exist) {
				for (int direct = 0; direct < 3; direct++) {
					Movement mvmt(chs, direct);
					if (currentGame.isLegalMove(mvmt)) {
						Game nextStep = currentGame;
						nextStep.update(mvmt);
						nextStep.switchPlayer();
					
						int childValue = minimax(nextStep, height-1);
						bestValue = min(bestValue, childValue);
					}
				}
			}
		}
	}
	return bestValue;
}

int EwnAI::evaluate(Game& currentGame) {
	int val = 0;

	for(int dice = 0; dice < 6; dice++) {
		if(currentGame.getCurrPlayer(dice).exist) {
			Pos temp;
			temp.first = currentGame.getCurrPlayer(dice).x;
			temp.second = currentGame.getCurrPlayer(dice).y;
			val += hv_[temp];
		}
	}
	return val;
}
