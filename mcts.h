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
    //Selection selection(eps);
    Movement AI_move(Game& game, int dice);
    // mcts main
    float run(Game& current_game);
	int simulation(Game simu_game);
};
// src: https://github.com/memo/ofxMSAmcts/blob/master/src/ofxMSAmcts.h

#endif
