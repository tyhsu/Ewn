#ifndef TREE_NODE_H
#define TREE_NODE_H
#include <vector>
#include "game.h"
#include "tree_node.h"
Tree_node::Tree_node() {
	this->score = 0;
    this->num_visit = 0;
    this->game_status = 0;
    this->game = Game();
    this->parent = NULL;
}
Tree_node::Tree_node (int _game_status, Game _game , Tree_node * _parent) {
    this->score = 0;
    this->num_visit = 0;
    this->game_status = _game_status;
    this->game = _game;
    this->parent = _parent;
    for (int i = 0; i < 18; i++) {
		Movement movement(i % 6, i % 3);
        if (this->game.check_in_board(movement)) {
			is_legal_list[i] = true;
			legal_move_list[i] = movement;
			Game child_game = this->game;
			children_list[i] = new Tree_node(child_game.update_game_status(movement), child_game, this);
		}
	}
            
}
Tree_node::Tree_node(Tree_node* _Tree_node) {
	this->score = _Tree_node->score;
    this->num_visit = _Tree_node->num_visit;
    this->game_status = _Tree_node->game_status;
    this->game = _Tree_node->game;
    this->parent = _Tree_node->parent;
	for (int i = 0; i < 18; i++) {
 		is_legal_list[i] = _Tree_node->is_legal_list[i];  	
		legal_move_list[i] = _Tree_node->legal_move_list[i]; 		
		children_list[i] = _Tree_node->children_list[i];	
	}
   
}
void Tree_node::operator=(Tree_node* _Tree_node) {
	this->score = _Tree_node->score;
    this->num_visit = _Tree_node->num_visit;
    this->game_status = _Tree_node->game_status;
    this->game = _Tree_node->game;
    this->parent = _Tree_node->parent;
    for (int i = 0; i < 18; i++) {
 		is_legal_list[i] = _Tree_node->is_legal_list[i];  	
		legal_move_list[i] = _Tree_node->legal_move_list[i]; 		
		children_list[i] = _Tree_node->children_list[i];	
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
}

Tree_node* Tree_node::get_parent() { 
	return this->parent; 
}
#endif
