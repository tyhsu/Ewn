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
    int simulation_depth;
    UCT uct;
	MCTS ();
	~MCTS ();
    //Selection selection(eps);
    Movement AI_move(const Game&  game, int dice);
    //Run 4 stages of MCTS for max_iterations times
    float run(const Game& current_game);
    //Run one time sumulation
	int simulation(const Game& simu_game);

	void recursive_delete_tree_node(Tree_node* _Tree_node);
};
// src: https://github.com/memo/ofxMSAmcts/blob/master/src/ofxMSAmcts.h

#endif
