#include <iostream>
#include <cstdio>
#include <algorithm>
#include "game.h"
#include "ewnAi.h"
const int HEIGHT = 2;
const float chance_weight = 1/6;

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
	aiSymbol_ = aiTurn_? 'A': '1';

	Movement answer;//the best move will return
	int bestValue = -1e9;
	int nextMoveCnt = currentGame.availableMove(dice);
	for (int i=0; i<nextMoveCnt; i++) {
		for (int direct = 0; direct < 3; direct++) {
			int chs = currentGame.getMovableChs(i).symbol - aiSymbol_;
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
	cout << "========== a minimax ===========" << endl;
	int bestValue;
	int diceArray[6] = {0};
	int lookahead = 0;

	if (currentGame.getTurn() == aiTurn_) {
		// ai's turn, find the max
		bestValue = -1e9;
		int win = aiTurn_? 2: 1;
		// for #chess and direction
		for (int dice = 0; dice < 6; dice++) {
			bool isThisChsMovable = currentGame.getCurrPlayer(dice).exist;

			if(isThisChsMovable) {
				char thisSymbol = currentGame.getCurrPlayer(dice).symbol;
				int chs = thisSymbol - aiSymbol_;
				cout << "[char value] " << chs << endl;
				for (int direct = 0; direct < 3; direct++) {
					Movement mvmt(chs, direct);
					if (currentGame.isLegalMove(mvmt)) {
						Game nextStep = currentGame;
						int end = nextStep.update(mvmt);
						// check if the game ends
						if (end == win) return 1000;
						else if (end != 0) continue;
						nextStep.switchPlayer();

						// return child value.
						int childValue = minimax(nextStep, height-1);
						bestValue = max(bestValue, childValue);
						cout << "[max]in 3 direction, return bestvalue. " << bestValue << endl;
					} else {
						cout << "nope, the chs can`t be move." << endl;
					}
				}
				diceArray[dice] = bestValue;
			}
			else {
				// this chs can`t move, use other`s value
				if(dice - lookahead - 1 < 0) {
					diceArray[dice] = 0;
				}
				else if(){
					diceArray[dice] = diceArray[dice - diceArray];
				}
			}
		}

    }
	else {
		// the opponent's turn, find the min
		bestValue = 1e9;
		int lose = aiTurn_? 1: 2;
		// for #chess and direction
		for (int dice = 0; dice < 6; dice++) {

			int availableNum = currentGame.availableMove(dice);
			for (int i=0; i<availableNum; i++) {
				int oppntSymbol = aiTurn_? '1': 'A';
				int chs = currentGame.getMovableChs(i).symbol - oppntSymbol;
				for (int direct = 0; direct < 3; direct++) {
					Movement mvmt(chs, direct);
					if (currentGame.isLegalMove(mvmt)) {
						Game nextStep = currentGame;
						int end = nextStep.update(mvmt);
						// check if the game ends
						if (end == lose) return -1000;
						else if (end != 0) continue;
						nextStep.switchPlayer();

						// return child value.
						int childValue = minimax(nextStep, height-1);
						bestValue = min(bestValue, childValue);
						cout << "[min]in 3 direction, return bestvalue. " << bestValue << endl;
					}
				}
			}
		}
	}
	cout << "feature value:[ ";
	for(int i = 0; i < 6; i++){
		cout << dice_array[i] << " ";
	}
	cout << "]" << endl;
	cout << "========== a minimax ===========" << endl;
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
