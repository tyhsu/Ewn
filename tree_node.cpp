#include <vector>
#include <game.h>
#include "tree_node.h"

Tree_node::Tree_node (int _game_status, Game _game , Tree_node * _parent) {
    this->score = 0;
    this->num_visit = 0;
    this->game_status = _game_status;
    this->game = _game;
    this->parent = _parent;
    for (int i = 0; i < 18; i++)
        if (this->game.check_in_board(Movement movement(i % 6, i % 3)))
            this->legal_move_list.push_back(movement);
}
Tree_node::Tree_node(Tree_node* _Tree_node) {
	this->score = _Tree_node->score;
    this->num_visit = _Tree_node->num_visit;
    this->game_status = _Tree_node->game_status;
    this->game = _Tree_node->game;
    this->parent = _Tree_node->parent;
    this->legal_move_list = _Tree_node->legal_move_list;
}
void Tree_node::operator=(Tree_node* _Tree_node) {
	this->score = _Tree_node->score;
    this->num_visit = _Tree_node->num_visit;
    this->game_status = _Tree_node->game_status;
    this->game = _Tree_node->game;
    this->parent = _Tree_node->parent;
    this->legal_move_list = _Tree_node->legal_move_list;
}
void Tree_node::set_score(float _score) {
	this->score = _score;
}

bool Tree_node::is_terminate() {
	return this->game != 0;
}

void Tree_node::update(float _score) {
    this->num_visit ++;
    this->score += _score;
}

Tree_node* Tree_node::get_parent() { 
	return this->parent; 
}

Tree_node* Tree_node::add_child_with_action(int move_index) {
    Game child_game = this->game;
    Tree_node* child = new Tree_node(child_game.update_game_status(this->legal_move_list[move_index]), child_game, this);
    this->children_list.push_back(child);
    return child;
}
