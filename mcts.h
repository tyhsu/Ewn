#include <math.h>
#include "tree_node.h"
const float eps = 1e-6;
using namespace std;
class UCT {
public:
    tree_node * root;
    int iteration;  // nodes
    float uct_k;    // uct param
    UCT () {
        iteration = 100;
        uct_k = sqrt ( 2 );
    }
    tree_node * get_best_child ( tree_node * node ) {
    	// end check
        if ( node->is_terminate() ) return NULL;

        // record node
        tree_node* best_node = NULL;
        float best_uct_score = 0;

        // iterate all immediate children and find best UTC score
        int index_list[18], index_list_size = 0;
        for( int i = 0; i < node->children_list.size(); i++ ) {
            tree_node child = node->children_list[i];
            float uct_exploitation = (float)child->score / (child->num_visit);
            float uct_exploration = sqrt( log((float)node->num_visit + 1) / (child->num_visit) );
            float uct_score = uct_exploitation + uct_k * uct_exploration;

            if (best_uct_score < uct_score) {
                best_utc_score = uct_score;
                best_node = child;
            }
        }
        return best_node;
    }

    tree_node * get_most_visited_child ( tree_node * node ) {
        // record node
        int most_visited = -1;
        tree_node* best_node = NULL;

        // iterate all immediate children and find best UTC score
        int index_list[18], index_list_size = 0;
        for( int i = 0; i < node->children_list.size(); i++ ) {
            tree_node child = node->children_list[i];
            if(child->num_visit > most_visited) {
            	most_visited = child->num_visit;
            	best_node = child;
            }
        }
        return best_node;
    }
}
