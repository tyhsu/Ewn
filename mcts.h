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
        if ( node->is_terminate() ) return NULL;
        float best_uct_score = 0;
        int index_list[18], index_list_size = 0;
        for( int i = 0; i < node->children_list.size(); i++ ) {
            tree_node child = node->children_list[i];
            float uct_exploitation = (float)child->score / (child->num_visit);
            float uct_exploration = sqrt( log((float)node->num_visit + 1) / (child->num_visit) );
            float uct_score = uct_exploitation + uct_k * uct_exploration;
            if (best_uct_score < uct_score)
        }
    }
}
