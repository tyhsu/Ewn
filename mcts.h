#include <math.h>
#include "Tree_node.h"
const float eps = 1e-6;
using namespace std;
class MCTS {
public:
    Tree_node* root;
    int iteration;  // nodes
    int max_iterations;
    int simulation_depth;
    float uct_k;    // uct param
    MCTS ();
    Tree_node* get_best_child (Tree_node* node, float uct_k);

    Tree_node* get_most_visited_child (Tree_node* node);

    // mcts main 
    Movement run(const Game& current_state, unsigned int seed = 1, vector<Game>* explored_states = nullptr);
}

// src: https://github.com/memo/ofxMSAmcts/blob/master/src/ofxMSAmcts.h