#include <stdlib.h>
#include "mcts.h"
using namespace std;
UCT::UCT () {
    this->uct_k = sqrt (2);
}

int UCT::get_best_child_index (Tree_node* node, float uct_k) {
    // record node
    int best_child_index = 0;
    float best_uct_score = 0;

    // Store all data for random
    int index_list[18], index_list_size = 0;
	float uct_score_list[18] = {0};
	// iterate all immediate children and find best UCT score
    for(int i = 0; i < 18; i++) {
		if (node->is_legal_list[i] == false) 
			continue;
		
        Tree_node* child = node->children_list[i];
        float uct_exploitation = (float)child->score / (child->num_visit + eps);
        float uct_exploration = sqrt(log((float)node->num_visit + 1) / (child->num_visit + eps));
        float uct_score = uct_score_list[i] = uct_exploitation + uct_k* uct_exploration;
		
        if (best_uct_score < uct_score) {
            best_uct_score = uct_score;
        }
    }
	for(int i = 0; i < 18; i++) {
		if((float)abs(best_uct_score - uct_score_list[i]) < eps) { 
			index_list[index_list_size++] = i;
			//cout << i << " : " <<  uct_score_list[i] << endl;
		}
	}
	//cout << "UCT done " << endl;
    return index_list[random()%index_list_size];
}
MCTS::MCTS(){}
Movement MCTS::AI_move(Game& cur_game, int dice) { 
    this->max_iterations = 100;
    this->simulation_depth = 10;
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
				// check if the game ends
				if (game_status == win) {
					answer = mvmt;
					break;
				}
				else if (game_status != 0)	// lose the game
					continue;
				else {
					child_game.switch_player();
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
float MCTS::run(const Game& current_game) {
	
	int iteration = 0;  // nodes
    // initialize root Tree_node with current state
    Tree_node* root_node = new Tree_node(0, current_game, NULL, 0);
	
    while(iteration++ < max_iterations) {
		cout << "stage 0 "<< endl;
        // 1. select. Start at root, dig down into tree using MCTS on all fully expanded nodes
        Tree_node* best_node = root_node;
		int best_child_index;
		best_child_index = this->uct.get_best_child_index(best_node, this->uct.uct_k);
		
        while(!best_node->is_terminate()) {
			cout << "dea" << endl;
			if (best_node->is_expanded) cout <<"EXPANDED";
			else cout << "NOT EXPANDED";
			cout<< " DEPTH :" << best_node->depth << endl;
            cout << " Best Child Index:" << best_child_index << endl;
			if(best_node->num_visit == 0) {
				break;
			}
			cout << "num_visit :" << best_node->num_visit << endl;
			best_node = best_node->children_list[best_child_index];
			/*
			*	HERE IS THE FUCKING ERROR
			*/
			// you may trace the tree node constructor looking for detail
			if(best_node == NULL) cout <<"LERRRRR"<<endl;
			cout << "dead" << endl;
		}
		cout << "stage 1 "<< endl;
        // 2. expand by adding a single child (if not terminal or not fully expanded)
        if(!best_node->is_terminate()) {
				best_node->children_list[best_child_index] = 
				new Tree_node(best_node->game.update_game_status(best_node->legal_move_list[best_child_index]), 
				best_node->game, 
				best_node, best_node->depth);
				best_node = best_node->children_list[best_child_index] ;
					cout << "EXPAND " << best_child_index << endl;
		}
		cout << "stage 2 "<< endl;
        // 3. simulate
		int reward = best_node->is_terminate() ? best_node->game_status == (this->ai_side? 2 : 1)
											   : this->simulation(best_node->game);
		cout << "stage 3 "<< endl;									   
        // 4. back propagation
        while(true) {
            best_node->update(reward);
			if(best_node->parent ==  NULL) break;
            best_node = best_node->get_parent();
        }
		cout << "stage 4 "<< endl;
	}
	return root_node->score;
}

int MCTS::simulation(Game simu_game) {
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
	// return the result of game.
	return (game_status == ai_win? 1 : 0);
}
   
