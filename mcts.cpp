#include <math.h>
#include "Tree_node.h"
#include "mcts.h"

const float eps = 1e-6;
using namespace std;
MCTS::MCTS () {
    max_iterations = 100;
    simulation_depth = 10;
    uct_k = sqrt (2);
}

Tree_node* MCTS::get_best_child (Tree_node* node, float uct_k) {
    // end check
    if (node->is_terminate()) return NULL;
    MCTS();
    Tree_node* get_best_child (Tree_node* node, float uct_k);

    // record node
    Tree_node* best_node = NULL;
    float best_uct_score = 0;

    // iterate all immediate children and find best UTC score
    int index_list[18], index_list_size = 0;
    for(int i = 0; i < node->children_list.size(); i++) {
        Tree_node child = node->children_list[i];
        float uct_exploitation = (float)child->score / (child->num_visit);
        float uct_exploration = sqrt(log((float)node->num_visit + 1) / (child->num_visit));
        float uct_score = uct_exploitation + uct_k* uct_exploration;

        if (best_uct_score < uct_score) {
            best_utc_score = uct_score;
            best_node = child;
        }
    }
    return best_node;
}

Tree_node* MCTS::get_most_visited_child (Tree_node* node) {
    // record node
    int most_visited = -1;
    Tree_node* best_node = NULL;

    // iterate all immediate children and find best UTC score
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

// mcts main 
Movement MCTS::run(const Game& current_state, unsigned int seed = 1, vector<Game>* explored_states = nullptr) {

    // initialize root TreeNode with current state
    TreeNode root_node(0, current_state, null);
    TreeNode* best_node = NULL;
    iteration = 0;
    while(true) {
        // 1. select. Start at root, dig down into tree using MCTS on all fully expanded nodes
        TreeNode* node = &root_node;
        while(!node->is_terminal()) {
            node = get_best_child(node, uct_k);
        }
        // 2. expand by adding a single child (if not terminal or not fully expanded)
        if(!node->is_terminal()) node = node->add_child_with_action();
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
    // return best node's action
    if(best_node) return best_node->get_action();
}
   
