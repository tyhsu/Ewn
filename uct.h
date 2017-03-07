#ifndef UCT_H
#define UCT_H
#include "tree_node.h"

class Selection
{
public:
	float eps;
	Selection() {}
	Selection(float eps_) { this->eps = eps_; }
	~Selection() {}
	virtual int select_children_list_index(Tree_node* cur_node);
};

class UCT: public Selection {
public:
	float exploration_param;
	float t_param;
	UCT();
	UCT(float eps_);
	int select_children_list_index(Tree_node* cur_node);
	//Tree_node * get_most_visited_child ( Tree_node * node );
};

#endif
