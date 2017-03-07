#include <cstdlib>
#include <cmath>
#include "uct.h"

UCT::UCT()
{
	this->exploration_param = sqrt(2);
	this->t_param = 1;
}

UCT::UCT(float eps_): Selection(eps_)
{
	this->exploration_param = sqrt(2);
	this->t_param = 1;
}

int UCT::select_children_list_index(Tree_node* cur_node)
{
	int child_num_ = cur_node->child_num;
	float child_value_list[18] = {0};
	float best_value = 0.0;
	// calculate every UCT value of the child nodes
	for (int i=0; i<child_num_; i++) {
		if (!cur_node->is_legal_list[i])
			continue;

		Tree_node child_node = cur_node->children_list[i];
		float exploitation = child_node.score / child_node.num_visit;
		float exploration = this->exploration_param * sqrt( log(cur_node->num_visit + this->t_param) / child_node.num_visit );
		child_value_list[i] = exploitation + exploration;
		if (child_value_list[i] > best_value)
			best_value = child_value_list[i];
	}

	// gather the nodes whose UCT values approach best_value
	int best_children_num = 0;
	float child_index_list[18] = {0};
	for (int i=0; i<child_num_; i++) {
		if (best_value - child_value_list[i] < eps)
			child_index_list[ best_children_num++ ] = i;
	}
	// select a child node index randomly
	return child_index_list[ rand()%best_children_num ];
}