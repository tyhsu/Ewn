
#include <vector>
#include "game.h"
#include "tree_node.h"
 Tree_node::Tree_node() {
	this->score = 0;
    this->num_visit = 0;
    this->game_status = 0;
    this->game = Game();
	this->is_expanded = false;
    this->parent = NULL;
}
Tree_node::Tree_node (int _game_status, Game _game , Tree_node * _parent, int _depth = 0) {
    this->score = 0;
    this->num_visit = 0;
    this->game_status = _game_status;
    this->game = _game;
    this->parent = _parent;
	this->depth = _depth;
	this->is_expanded = true;
    for (int i = 0; i < 18; i++) {
		Movement movement(i / 3, i % 3);
        if (this->game.check_in_board(movement)) {
			is_legal_list[i] = true;
			legal_move_list[i] = movement;
			Game child_game = this->game;
			children_list[i] = new Tree_node();
			children_list[i]->depth = _depth+1;
			children_list[i]->parent = this;
		}
		else is_legal_list[i] = false;
	}
	
            
}
Tree_node::Tree_node(Tree_node* _tree_node) {
	this->score = _tree_node->score;
    this->num_visit = _tree_node->num_visit;
    this->game_status = _tree_node->game_status;
    this->game = _tree_node->game;
    this->parent = _tree_node->parent;
	for (int i = 0; i < 18; i++) {
 		is_legal_list[i] = _tree_node->is_legal_list[i];  	
		legal_move_list[i] = _tree_node->legal_move_list[i]; 		
		children_list[i] = _tree_node->children_list[i];	
	}
   
}
void Tree_node::operator=(Tree_node& _tree_node) {
	
	this->score = _tree_node.score;
    this->num_visit = _tree_node.num_visit;
    this->game_status = _tree_node.game_status;
    this->game = _tree_node.game;
    this->parent = _tree_node.parent;
	this->depth = _tree_node.depth;
	this->is_expanded = _tree_node.is_expanded;
    for (int i = 0; i < 18; i++) {
 		is_legal_list[i] = _tree_node.is_legal_list[i];  	
		legal_move_list[i] = _tree_node.legal_move_list[i]; 		
		children_list[i] = _tree_node.children_list[i];	
	}
}
void Tree_node::set_score(float _score) {
	this->score = _score;
}

bool Tree_node::is_terminate() {
	return this->game_status != 0;
}

void Tree_node::update(float _score) {
    this->num_visit ++;
	
    this->score += _score;
	cout << " update : " << this->score << ", " << this->num_visit << endl;
}

Tree_node* Tree_node::get_parent() { 
	return this->parent; 
}

