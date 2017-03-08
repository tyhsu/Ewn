#ifndef TREE_NODE_H
#define TREE_NODE_H
#include <vector>
#include "game.h"

using namespace std;
class Tree_node {
public:
    float score;
    int num_visit;
    int game_status;
	int depth = -1;
	bool is_expanded = false;
    Game game;
    Tree_node* parent;
    int is_legal_list[18] = {0};
    Movement legal_move_list[18];
	Tree_node* children_list[18];
	//Default Constructor
	Tree_node();
	//Constructor
    Tree_node(int _game_status, Game _game  , Tree_node * _parent, int _depth);
	//Copy Constructor
	Tree_node(Tree_node* _Tree_node);
	//Copy operator
	void operator=(Tree_node& _Tree_node);
	//Set Score
    void set_score(float _score);
	//If the node is a end-gamed node
    bool is_terminate();
	//Update score and num_visit
    void update(float _score);
	//Get parent
    Tree_node* get_parent();
};
#endif