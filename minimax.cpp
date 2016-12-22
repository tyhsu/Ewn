#include <iostream>
#include <cstdio>
#include <algorithm>
#include <time.h>
#include <string>
#include "game.h"
#include "minimax.h"

const int HEIGHT = 4;

Minimax::Minimax() {
	// pos: a <int,int> , <x,y> coord;
	for (int i = 0; i < 5; i++) {
		for (int j = 0;j < 5; j++) {
			// insert to map => feature_map_A;
			// get the smaller of (x,y)
			int k = (i > j ? j : i);
			k = (k+1) * (k+1) * 3;
			Pos chs_pos;
			chs_pos.first = i;
			chs_pos.second = j;
			this->feature_map_A.insert(make_pair(chs_pos, k));

			// insert to map => feature_map_B;
			// get the bigger of (x,y)
			k = (i > j ? i : j);
			k = (5-k) * (5-k) * 3;
			chs_pos.first = i;
			chs_pos.second = j;
			this->feature_map_B.insert(make_pair(chs_pos, k));
		}
	}
}

Movement Minimax::AI_move(Game cur_game, int dice)
{
    cerr << "calculating..." ;
    cerr.flush();
	clock_t init_time = clock();
	this->ai_side = cur_game.get_is_switch();
	this->ai_symbol = this->ai_side? 'A': '1';

	Movement answer;	//the best move will return
	int best_val = -1e9, child_val;
	int win = this->ai_side? 2: 1;
	int next_move_cnt = cur_game.count_movable_chs(dice);

	for (int i=0; i<next_move_cnt; i++) {
		int chs_index = cur_game.get_movable_chs(i).symbol - this->ai_symbol;
		for (int direct = 0; direct < 3; direct++) {
			Movement mvmt(chs_index, direct);
			if (cur_game.check_in_board(mvmt)) {
				Game child_game = cur_game;
				int game_status = child_game.update_game_status(mvmt);
				// check if the game ends
				if (game_status == win)
					child_val = feature(child_game);
				else if (game_status != 0)	// lose the game
					continue;
				else {
					child_game.switch_player();
					child_val = minimax(child_game, HEIGHT);
				}
				// update the best value and the best movement
				if (child_val >= best_val) {
					best_val = child_val;
					answer = mvmt;
				}
			}
		}
	}
	double timeCost = (double)(clock() - init_time) / CLOCKS_PER_SEC;
	printf("\ntime cost: %.2fs\n", timeCost);
	return answer;
}

int Minimax::minimax(Game& cur_game, int height)
{
	// check if end;
	if (height == 0) {
		cur_game.switch_player();
		return feature(cur_game);
	}
	int best_val;
	int chs_val_list[6];
	int lookahead_cnt = 0;

	bool is_ai_side;
	int cur_win;
	char cur_symbol;
	if (cur_game.get_is_switch() == this->ai_side) {
		// ai's cur_side		=> the largest, the better
		is_ai_side = true;
		best_val = -1e9;
		cur_win = this->ai_side ? 2 : 1;
		cur_symbol = this->ai_symbol;
		for (int i=0; i<6; i++)
			chs_val_list[i] = -1e9;
	}
	else {
		// opponent's cur_side	=> the smallest, the better
		is_ai_side = false;
		best_val = 1e9;
		cur_win = this->ai_side ? 1 : 2;
		cur_symbol = this->ai_side? '1' : 'A';
		for (int i=0; i<6; i++)
			chs_val_list[i] = 1e9;
	}

	// ai's cur_side, find the max
	// opponent's cur_side, find the min
	for (int dice = 0; dice < 6; dice++) {
		Chess cur_chs = cur_game.get_cur_chs_list(dice);
		if (cur_chs.exist) {
			int chs_index = cur_chs.symbol - cur_symbol;
			for (int direct = 0; direct < 3; direct++) {
				Movement mvmt(chs_index, direct);
				if (cur_game.check_in_board(mvmt)) {
					Game child_game = cur_game;
					int game_status = child_game.update_game_status(mvmt);
					// check if the game ends
					if (game_status == cur_win) return feature(child_game);
					else if (game_status != 0) continue;
					child_game.switch_player();

					// return child value.
					int child_val = minimax(child_game, height-1);
					if (is_ai_side) {
						best_val = max(best_val, child_val);
					}
					else {
						best_val = min(best_val, child_val);
					}
				}
				else {
				}
			}
			chs_val_list[dice] = best_val;

			// if there is lookahead_cnt, compare those values of the non-existing with the existing
			// check if the current value is bigger than the previous one.
			if (lookahead_cnt) {
				int offset;
				if ((is_ai_side && best_val>chs_val_list[dice-1]) || (!is_ai_side && best_val<chs_val_list[dice-1])) {
					for (int j = 1; j <= lookahead_cnt; j++) {
						offset = dice - j;
						chs_val_list[offset] = best_val;
					}
				}
				lookahead_cnt = 0;
			}
		}
		else {
			// this chess can't move, use other's value
			lookahead_cnt++;
			if (dice - lookahead_cnt < 0) {
				// there's no chess exists before the selected one
				continue;
			}
			else {
				chs_val_list[dice] = chs_val_list[dice - lookahead_cnt];
			}
		}
	}

	// return the sum of the best values in the chs_val_list (chance node)
	best_val = 0;
	for(int i = 0; i < 6; i++)
		best_val += chs_val_list[i];

	return best_val;
}

int Minimax::feature(Game& cur_game)
{
	int ai_val = 0, opp_val = 0;
	bool cur_side = this->ai_side;
	Heuristic_map *feature_map = this->ai_side? &this->feature_map_B: &this->feature_map_A;
	int *val_ptr = &ai_val;

	for (int side=0; side<2; side++) {
		for (int chs_index = 0; chs_index < 6; chs_index++) {
			// calculate the score of the side
			Chess chs = cur_game.get_chs_list(cur_side, chs_index);
			if (chs.exist) {
				// [exist] chess calculating function.
				Pos temp;
				temp.first = chs.x;
				temp.second = chs.y;
				*val_ptr += (*feature_map)[temp];
			}
			else {
				// [non-exist] chess calculating function.
				// prev_chs_val: find closest, previous chess num's value.
				// next_chs_val: find closest, following chess num's value.
				int prev_chs_val = 0, next_chs_val = 0;
				// get small part of chess list.
				for (int prev_chs_index = chs_index; prev_chs_index > 0; prev_chs_index--) {
					Chess ctmp = cur_game.get_chs_list(cur_side, prev_chs_index);
					if (ctmp.exist) {
						Pos temp;
						temp.first = ctmp.x;
						temp.second = ctmp.y;
						prev_chs_val = (*feature_map)[temp];
						break;
					}
				}

				// get big part of chess list.
				for (int next_chs_index = chs_index; next_chs_index < 6; next_chs_index++) {
					Chess ctmp = cur_game.get_chs_list(cur_side, next_chs_index);
					if (ctmp.exist) {
						Pos temp;
						temp.first = ctmp.x;
						temp.second = ctmp.y;
						next_chs_val = (*feature_map)[temp];
						break;
					}
				}

				// select one.
				prev_chs_val = (prev_chs_val > next_chs_val ? prev_chs_val : next_chs_val);
				*val_ptr += prev_chs_val;
			}
		}

		// calculate the score of opponent's side
		cur_side = !this->ai_side;
		feature_map = this->ai_side? &this->feature_map_A: &this->feature_map_B;
		val_ptr = &opp_val;
	}

	return ai_val - (int)(opp_val*1.2);
}
/**
 * try simulation
 */
int Minimax::prepare_simulate(Game& cur_game) {
	int ratio = 0;
	Game simu_game = cur_game;
	for(int game_cnt = 0; game_cnt < 100; game_cnt++) {
		ratio += simulation(simu_game);
	}
	return ratio;
}

int Minimax::simulation(Game& simu_game) {
	char cur_symbol;
	int game_status = 0, cur_win;
	if (simu_game.get_is_switch() == this->ai_side) {
		// ai's cur_side		=> the largest, the better
		cur_win = this->ai_side ? 2 : 1;
		cur_symbol = this->ai_symbol;
	}
	else {
		// opponent's cur_side	=> the smallest, the better
		cur_win = this->ai_side ? 1 : 2;
		cur_symbol = this->ai_side? '1' : 'A';
	}
	while(game_status == 0) {

		Movement mvmt[18];
		int mvmt_cnt = 0;
		for(int chs_index = 0; chs_index < 6; chs_index++) {
			// find all avaible move (6 dice index, 3 direction)
			Chess cur_chs = simu_game.get_cur_chs_list(chs_index);
			if (cur_chs.exist) {
				int chs_index = cur_chs.symbol - cur_symbol;
				for (int direct = 0; direct < 3; direct++) {
					Movement tmp_mvmt(chs_index, direct);
					if (simu_game.check_in_board(tmp_mvmt)) {
						mvmt[mvmt_cnt] = tmp_mvmt;
						mvmt_cnt ++;
					}
				}
			}
		}
		// randomly pick a move.
		Movement next_mvmt = mvmt[rand() % mvmt_cnt];
		game_status = simu_game.update_game_status(next_mvmt);

		// check game status => if the game keep going, switch the player.
		if(game_status == 0) simu_game.switch_player();

	}
	// return the result of game.
	return (game_status == 1? 1 : 0);
}
