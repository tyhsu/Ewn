#include <iostream>
#include <cstdio>
#include "game.h"
#include "ewnAi.h"
#define H = 4

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

			hv.insert(make_pair(chess_pos, k));
		}
	}
}

int EwnAI::beforeMinimax(Game currentGame, int dice) {
	int nextMoveCnt = currentGame.availiableMove(dice);
	for (int i=0; i<nextMoveCnt; i++) {
		for (int direct = 0; direct < 3; direct++) {
			int chs = (currentGame.getTurn() == false) ? currentGame.getMovableChs(chs).symbol - '1' : currentGame.getMovableChs(chs).symbol - 'A';

			Movement mvmt(chs, direct);
			if (currentGame.isLegalMove(mvmt)) {
				minimax(currentGame, H);
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

    int bestValue = 0;
    // if false(0): A`s turn, find max, else find min.
    if (currentGame.getTurn() == false) {
        // nextCnt
        bestValue = -1e9;
        // for #chess and direction
        for (int chs = 0; chs < 6; chs++) {
        	if(currentGame.currentPlayer_[chs].exist) {
	            for (int direct = 0; direct < 3; direct++) {

	                Movement mvmt(chs, direct);
	                if(currentGame.isLegalMove(mvmt)) {
	                    Game next_step = currentGame;
	                    next_step.update(mvmt);
	                    next_step.switchPlayer();

		                // return child value.
		                int child_value = minimax(next_step, height-1);
		                bestValue = min(bestValue, child_value);
	                }
	            }
        	}
        }
        return bestValue;
    }
    else {

        bestValue = 1e9;
        // for #chess and direction
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 3; j++) {

                Movement mvmt(chs, direct);
                if(isLegalMove(mvmt)) {
                    Game next_step = currentGame;
                    next_step.update(mvmt);
                    next_step.switchPlayer();

	                int child_value = minimax(next_step, height-1);
	                bestValue = max(bestValue, child_value);
                }
            }
        }
        return bestValue;
    }
}

int EwnAI::evaluate(Game& currentGame) {
    int val = 0;

    for(int dice = 0; dice < currentGame.currentPlayer_; dice++) {
        if(currentGame.currentPlayer_[dice].exist) {
            Pos temp;
            temp.first = currentGame.currentPlayer_[dice].x;
            temp.second = currentGame.currentPlayer_[dice].y;
            val += hv[temp];
        }
    }
    return val;
}
