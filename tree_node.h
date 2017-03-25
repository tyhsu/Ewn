#ifndef TREE_NODE_H
#define TREE_NODE_H

#include "game.h"

using namespace std;
class Tree_node {
public:
	int win_count;
	int visit_count;
	
	int game_status;
	Game game;
	Tree_node* parent_ptr;
	// List for legal child nodes
	Tree_node* children_ptr_list[18];

	// Default Constructor
	Tree_node();
	// Constructor for expand
	Tree_node(const Game& game_);
	// Copy Constructor
	Tree_node(Tree_node* node);
	// Copy operator
	void operator=(const Tree_node& node);
	
	// If the node is a end-gamed node
	bool is_terminate();
	// If the node has been visited (it is expanded)
	bool is_visit();

	// New legal child nodes and reset the values of the members(except children_ptr_list)
	void new_child_nodes();
	// Set win_count
	void set_win_count(float win_count_);
	// Update win_count and visit_count
	void update(float win_count_);

	// Get visit_count
	int get_visit_count();
	// Get parent_ptr
	Tree_node* get_parent_ptr();
	// Get child node pointer from children_ptr_list
	Tree_node* get_child_ptr(const int& child_index);

	// Debuging console
	void node_status();
};

#endif
