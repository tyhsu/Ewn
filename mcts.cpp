#include <stdlib.h>
#include "mcts.h"
using namespace std;

MCTS::MCTS() {
	this->selection_ptr = new UCT(eps);
}

MCTS::~MCTS() {
	delete this->selection_ptr;
}

Movement MCTS::AI_move(Game& cur_game, int dice) {
	this->max_iterations = 1000;
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

// mcts main
float MCTS::run(const Game& cur_game) {
	// initialize root Tree_node with current state
	Tree_node* root_node_ptr = new Tree_node(cur_game);

	int iteration = 0;  // nodes
	while(iteration++ < max_iterations) {
		// 1. select
		// Start from the root, digging down until finding an unvistied node
		Tree_node* node_ptr = root_node_ptr;
		int best_child_index;
		while(!node_ptr->is_terminate()) {
			best_child_index = this->selection_ptr->select_children_list_index(node_ptr);
			Tree_node* next_child_ptr = node_ptr->get_child_ptr(best_child_index);
			if(!node_ptr->is_visit() || !next_child_ptr->is_visit()) {
				// !next_child_ptr->is_visit() => why???
				break;
			}
			node_ptr = next_child_ptr;
		}

		// 2. expand by adding a single child (if not terminal or not fully expanded)
		if(!node_ptr->is_terminate()) {
			node_ptr->new_child_nodes();
			// Movement m = node_ptr->legal_move_list[best_child_index];
			// Game child_game = node_ptr->game;
			// int sta = child_game.update_game_status(m);
			// child_game.switch_player();
			// delete node_ptr->children_ptr_list[best_child_index];
			// node_ptr->children_ptr_list[best_child_index] = new Tree_node(sta, child_game, node_ptr, node_ptr->depth+1);
			node_ptr = node_ptr->get_child_ptr(best_child_index);	// why???
		}
		// 3. simulate
		int win_reward;
		if (node_ptr->is_terminate())
			win_reward = node_ptr->get_game_status() == (this->ai_side? 2: 1);
		else
			win_reward = this->simulation(node_ptr->game);

		// 4. back propagation
		while(true) {
			node_ptr->update(win_reward);
			if(node_ptr->get_parent_ptr() == NULL) break;
			node_ptr = node_ptr->get_parent_ptr();
		}
	}
	float result = (float)root_node_ptr->get_win_count();
	this->recursive_delete_tree_node(root_node_ptr);
	return result;
}

int MCTS::simulation(const Game& cur_game) {
    Game simu_game = cur_game;
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
	return (game_status == ai_win? 1: 0);
}

void MCTS::recursive_delete_tree_node(Tree_node* node_ptr) {
	for (int i = 0; i < 18; i++) {
		if (node_ptr->get_child_ptr(i)) {
			recursive_delete_tree_node(node_ptr->get_child_ptr(i));
		}
	}
	delete node_ptr;
}