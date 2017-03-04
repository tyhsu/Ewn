#include <math.h>
#include <stdlib.h>
#include "Tree_node.h"
#include "mcts.h"

const float eps = 1e-6;
using namespace std;
UCT::UCT () {
    uct_k = sqrt (2);
}

int UCT::get_best_child_index (Tree_node* node, float uct_k) {
    // end check
    if (node->is_terminate()) return NULL;
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
		
        Tree_node child = node->children_list[i];
        float uct_exploitation = (float)child->score / (child->num_visit);
        float uct_exploration = sqrt(log((float)node->num_visit + 1) / (child->num_visit));
        float uct_score = uct_score_list[i] = uct_exploitation + uct_k* uct_exploration;
		
        if (best_uct_score < uct_score) {
            best_utc_score = uct_score;
            best_node = child;
        }
    }
	for(int i = 0; i < 18; i++) {
		if((float)abs(best_uct_score - uct_score) < eps) { 
			index_list[index_list_size++] = i;
		}
	}
	
    return index_list[random()%index_list_size];
}

Tree_node* UCT::get_most_visited_child (Tree_node* node) {
    // record node
    int most_visited = -1;
    Tree_node* best_node = NULL;

    // iterate all immediate children and find best UCT score
    int index_list[18], index_list_size = 0;
    for(int i = 0; i < node->children_list.size(); i++) {
        Tree_node child = node->children_list[i];
        if(child->num_visit > most_visited) {
            most_visited = child->num_visit;
            best_node = child;
        }
    }
    return best_node;
}
Movement MCTS::AI_move(Game& cur_game, int dice) { 
    this->max_iterations = 100;
    this->simulation_depth = 10;
    this->ai_side = cur_game.get_is_switch();
    this->ai_symbol = this->ai_side? 'A': '1';
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
					//TODO
				else if (game_status != 0)	// lose the game
					continue;
				else {
					child_game.switch_player();
					child_val = run(child_game);
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

    // initialize root TreeNode with current state
    TreeNode* root_node = new TreeNode(0, current_game, null);
    iteration = 0;
    while(true) {
        // 1. select. Start at root, dig down into tree using MCTS on all fully expanded nodes
        TreeNode* best_node = &root_node;
        while(!best_node->is_terminal()) {
			int best_child_index = this->UCT.get_best_child(best_node, uct_k);
            TreeNode* node = best_node->children_list[best_child_index];
			if(node->num_visit == 0) break;
			best_node = node;
		}
        // 2. expand by adding a single child (if not terminal or not fully expanded)
        if(!best_node->is_terminal()) 
			best_node = best_node->children_list[best_child_index];
        Game state(node->update_game_status());
        // 3. simulate
        if(!node->is_terminal()) {
            Movement mvmt;
            for(int t = 0; t < simulation_depth; t++) {
                // check if game state end
                if(state.update_game_status(mvmt) != 0) break;
            }
        }
        // get rewards vector for all node
        const std::vector<float> rewards = state.evaluate();
        // add to history
        if(explored_states) explored_states->push_back(state);
        // 4. back propagation
        while(node) {
            node->update(rewards);
            node = node->get_parent();
        }
        // find most visited child
        best_node = get_most_visited_child(&root_node);
        // exit loop if current iterations exceeds max_iterations
        if(max_iterations > 0 && iterations > max_iterations) break;
        iterations++;
	}
    
}
   
