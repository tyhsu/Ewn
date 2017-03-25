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
	this->eps = eps_;
}

int UCT::select_children_list_index(Tree_node* cur_node)
{
	float child_value_list[18] = {0};
	float best_value = 0.0;

	// calculate every UCT value of the child nodes to find the max
	for (int i=0; i<18; i++) {
		if (cur_node->get_child_ptr(i) == NULL) break;

		Tree_node* node_ptr = cur_node->get_child_ptr(i);
		float exploitation = (float)node_ptr->win_count / ((float)node_ptr->visit_count + this->eps);
		float exploration = this->exploration_param * sqrt( log((float)cur_node->visit_count + this->t_param) / ((float)node_ptr->visit_count + eps));
		child_value_list[i] = exploitation + exploration;
		if (child_value_list[i] > best_value)
			best_value = child_value_list[i];
	}

	// gather the nodes whose UCT value approaches best_value
	int best_children_num = 0;
	float child_index_list[18];
	for (int i=0; i<18; i++) {
		if (cur_node->get_child_ptr(i) == NULL) break;
		if (best_value - child_value_list[i] < eps)
			child_index_list[ best_children_num++ ] = i;
	}
	
	// select one of best child node randomly and return its index
	return child_index_list[ rand()%best_children_num ];
}
