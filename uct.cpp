#include <cstdlib>
#include <cmath>
#include "uct.h"

UCT::UCT()
{
	this->exploration_param = sqrt(2);
	this->t_param = 1;
}

UCT::UCT(float eps_)
{
	this->exploration_param = sqrt(2);
	this->t_param = 1;
}

int UCT::select_children_list_index(Tree_node* cur_node)
{
	int child_num_ = 18;
	float child_value_list[18] = {0};
	float best_value = 0.0;
	// calculate every UCT value of the child nodes
	for (int i=0; i<child_num_; i++) {
		if (cur_node->is_legal_list[i] == 0)
			continue;

		Tree_node child_node = cur_node->children_list[i];
		float exploitation = (float)child_node.score / ((float)child_node.num_visit + 0.1);
		float exploration = this->exploration_param * sqrt( log((float)cur_node->num_visit + this->t_param) / ((float)child_node.num_visit + 0.1));
		child_value_list[i] = exploitation + exploration;
		if (child_value_list[i] > best_value)
			best_value = child_value_list[i];
	}
	// gather the nodes whose UCT values approach best_value
	int best_children_num = 0;
	float child_index_list[18];
	for (int i=0; i<child_num_; i++) {
		if (cur_node->is_legal_list[i] == 0)
			continue;
		if ((float)abs(best_value - child_value_list[i]) < eps) {
			child_index_list[ best_children_num++ ] = i;
		}
	}
	// select a child node index randomly
	return child_index_list[ rand()%best_children_num ];
}
