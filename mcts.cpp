#include <stdlib.h>
#include "mcts.h"
using namespace std;
MCTS::MCTS(){}
Movement MCTS::AI_move(const Game& _cur_game, int dice) {
	Game cur_game = _cur_game;
	this->max_iterations = 1000;
	this->simulation_depth = 1000;
	this->ai_side = cur_game.get_is_switch();
	this->ai_symbol = this->ai_side? 'A': '1';
	int best_val = -1e9, child_val;
	int win = this->ai_side? 2: 1;
	int next_move_cnt = cur_game.count_movable_chs(dice);
	Movement answer;
	for (int i=0; i<next_move_cnt; i++) {
		int chs_index = cur_game.get_movable_chs(i).symbol - this->ai_symbol;
		for (int direct = 0; direct < 3; direct++) {
			Movement mvmt(chs_index, direct);
			if (cur_game.check_in_board(mvmt)) {
				Game child_game = cur_game;
				int game_status = child_game.update_game_status(mvmt);
				child_game.switch_player();

				// check if the game ends
				if (game_status == win) {
					answer = mvmt;
					break;
				}
				else if (game_status != 0)	// lose the game
					continue;
				else {
					child_val = this->run(child_game);
				}

				// update the best value and the best movement
				if (child_val >= best_val) {
					best_val = child_val;
					answer = mvmt;
				}
			}
		}
	}
	return answer;
}

MCTS::~MCTS() {
	//cout << "leave" << endl;
}
// mcts main
float MCTS::run(const Game& current_game) {
	//current_game.print_status();
	int iteration = 0;  // nodes
	// initialize root Tree_node with current state
	Tree_node* root_node = new Tree_node(0, current_game, NULL, 0);
	//root_node->game.print_status();
	while(iteration++ < max_iterations) {
		// 1. select. Start at root, dig down into tree using MCTS on all fully expanded nodes
		Tree_node* best_node = root_node;
		int best_child_index;
		while(!best_node->is_terminate()) {
			best_child_index = this->uct.select_children_list_index(best_node);
			if(best_node->num_visit == 0 || best_node->children_list[best_child_index]->is_expanded == false ) {
				break;
			}
			best_node = best_node->children_list[best_child_index];
		}

		// 2. expand by adding a single child (if not terminal or not fully expanded)
		if(!best_node->is_terminate()) {
			Movement m = best_node->legal_move_list[best_child_index];
			Game child_game = best_node->game;
			int sta = child_game.update_game_status(m);
			child_game.switch_player();
			delete best_node->children_list[best_child_index];
			best_node->children_list[best_child_index] = new Tree_node(sta, child_game, best_node, best_node->depth+1);
			best_node = best_node->children_list[best_child_index] ;
		}
		// 3. simulate
		int reward = best_node->is_terminate() ? best_node->game_status == (this->ai_side? 2 : 1)
											   : this->simulation(best_node->game);

		// 4. back propagation
		while(true) {
			best_node->update(reward);
			if(best_node->parent ==  NULL)  break;
			best_node = best_node->parent;
		}
	}
	float result = root_node->score;
	this->recursive_delete_tree_node(root_node);
	return result;
}

int MCTS::simulation(const Game& _simu_game) {
    Game simu_game = _simu_game;
	char cur_symbol;
	int game_status = 0, ai_win = this->ai_side ? 2 : 1;
	if (simu_game.get_is_switch() == this->ai_side)
		cur_symbol = this->ai_symbol;
	else
		cur_symbol = this->ai_side? '1' : 'A';

	while (game_status == 0) {
		Movement available_mvmt_list[18];
		int available_mvmt_cnt = 0;
		// find all avaible move (6 dice indices, 3 directions)
		for(int chs_index = 0; chs_index < 6; chs_index++) {
			Chess cur_chs = simu_game.get_cur_chs_list(chs_index);
			if (cur_chs.exist) {
				for (int direct = 0; direct < 3; direct++) {
					Movement tmp_mvmt(chs_index, direct);
					if (simu_game.check_in_board(tmp_mvmt)) {
						available_mvmt_list[available_mvmt_cnt] = tmp_mvmt;
						available_mvmt_cnt ++;
					}
				}
			}
		}
		// randomly pick a move.
		Movement next_mvmt = available_mvmt_list[rand() % available_mvmt_cnt];
		game_status = simu_game.update_game_status(next_mvmt);

		// check game status => if the game keep going, switch the player.
		if(game_status == 0) simu_game.switch_player();
		else break;
	}
	return (game_status == ai_win? 1 : 0);
}

void MCTS::recursive_delete_tree_node(Tree_node* _Tree_node) {
	for (int i = 0; i < 18; i++) {
		if (_Tree_node->children_list[i]) {
			recursive_delete_tree_node(_Tree_node->children_list[i]);
		}
	}
	delete _Tree_node;
}