#ifndef MCTS_H
#define MCTS_H

#include <math.h>
#include "tree_node.h"
#include "uct.h"
using namespace std;

const float eps = 1e-6;

class MCTS {
public:
    bool ai_side;
    char ai_symbol;
    int max_iterations;
    Selection* selection_ptr;

	MCTS ();
	~MCTS ();
    
    // Selection selection(eps);
    Movement AI_move(Game& cur_game, int dice);
    // Run 4 stages of MCTS for max_iterations times
    int run(const Game& cur_game);
    // Run one time sumulation
	int simulation(const Game& cur_game);
    // Used for destructor
	void recursive_delete_tree_node(Tree_node* node_ptr);
};
// src: https://github.com/memo/ofxMSAmcts/blob/master/src/ofxMSAmcts.h

#endif
