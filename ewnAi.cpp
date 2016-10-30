#include <iostream>
#include <cstdio>
#include <algorithm>
#include "game.h"
#include "ewnAi.h"
const int HEIGHT = 4;

EwnAI::EwnAI() {
	// pos: a <int,int> , <x,y> coord;
	// insert to map => hv;
	cout << "create AI." << endl;
	for (int i = 0; i < 5; i++) {
		for (int j = 0;j < 5; j++) {

			// get the smallest of (x,y)
			int k = (i > j ? j : i);

			k = k * k;
			Pos chess_pos;
			chess_pos.first = i;
			chess_pos.second = j;
			printf("%2d", k);
			hv_.insert(make_pair(chess_pos, k));
		}
		cout << endl;
	}
}

Movement EwnAI::autoPlay(Game currentGame, int dice) {
	aiTurn_ = currentGame.getTurn();
	Movement answer;//the best move will return
	int bestValue = -1e9;
	int nextMoveCnt = currentGame.availiableMove(dice);
	for (int i=0; i<nextMoveCnt; i++) {
		for (int direct = 0; direct < 3; direct++) {
			int chs;
			if (currentGame.getTurn() == false)
				chs = currentGame.getMovableChs(i).symbol - '1';
			else
				chs = currentGame.getMovableChs(i).symbol - 'A';
			Movement mvmt(chs, direct);
			if (currentGame.isLegalMove(mvmt)) {
				int tmp = minimax(currentGame, HEIGHT);
				if(bestValue < tmp) { // update
					bestValue = tmp;
					answer = mvmt;
				}
			}
		}
	}
	return answer;
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
