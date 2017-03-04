#include <math.h>
#include "Tree_node.h"
const float eps = 1e-6;
using namespace std;
class UCT {
public:
    float uct_k;    // uct param
    UCT ();
    int get_best_child_index ( Tree_node * node, float uct_k );
    Tree_node * get_most_visited_child ( Tree_node * node );
}
class MCTS {
public:
    bool ai_side;
    char ai_symbol;
    int iteration;  // nodes
    int max_iterations;
    int simulation_depth;
    UCT uct;
    Movement AI_move(Game& game, int dice);
    // mcts main
    float run(const Game& current_game);

}
// src: https://github.com/memo/ofxMSAmcts/blob/master/src/ofxMSAmcts.h
