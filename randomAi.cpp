#include <iostream>
#include <cstdio>
#include <algorithm>
#include <time.h>
#include <string>
#include "game.h"
#include "randomAi.h"

RandomAi::RandomAi() {
	// cout a signal
	cout << "RandomAi start" << endl;
}

Movement RandomAi::AI_move(Game& cur_game, int dice)
{
    //cerr << "calculating..." ;
    //cerr.flush();
    // cout << "read to random" << endl;
	clock_t init_time = clock();
	this->ai_side = cur_game.get_is_switch();
	this->ai_symbol = this->ai_side? 'A': '1';

	Movement available_mvmt_list[6]; // random
	int available_mvmt_cnt = 0;

	int best_val = -1e9, child_val;
	int win = this->ai_side? 2: 1;
	int next_move_cnt = cur_game.count_movable_chs(dice);
	for (int i=0; i<next_move_cnt; i++) {
		int chs_index = cur_game.get_movable_chs(i).symbol - this->ai_symbol;
		for (int direct = 0; direct < 3; direct++) {
			Movement mvmt(chs_index, direct);
			if (cur_game.check_in_board(mvmt)) {
				available_mvmt_list[available_mvmt_cnt] = mvmt;
				available_mvmt_cnt ++;
			}
		}
	}

	// randomly pick a move.
	Movement next_mvmt = available_mvmt_list[rand() % available_mvmt_cnt];

	double timeCost = (double)(clock() - init_time) / CLOCKS_PER_SEC;

	return next_mvmt;
}