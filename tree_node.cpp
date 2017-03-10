
#include <vector>
#include "game.h"
#include "tree_node.h"
 Tree_node::Tree_node() {
	this->score = 0;
    this->num_visit = 0;
    this->game_status = 0;
	this->is_expanded = false;
    this->parent = NULL;
    for (int i = 0; i < 18; i++) {
        this->is_legal_list[i] = false;
		this->children_list[i] = NULL;
	}
}
Tree_node::Tree_node (int _game_status, const Game& _game , Tree_node * _parent, int _depth = 0) {
    this->score = 0;
    this->num_visit = 0;
    this->game_status = _game_status;
    this->game = _game;
    this->parent = _parent;
	this->depth = _depth;
	this->is_expanded = true;
    for (int i = 0; i < 18; i++) {
        Movement movement(i / 3, i % 3);
        this->is_legal_list[i] = ((this->game.check_in_board(movement) == true) && (this->game.get_cur_chs_list(i/3).exist == true));
		this->legal_move_list[i] = movement;
		this->children_list[i] = new Tree_node();
		this->children_list[i]->depth = _depth+1;
		this->children_list[i]->parent = this;
	}
}
Tree_node::Tree_node(Tree_node* _tree_node) {
	this->score = _tree_node->score;
    this->num_visit = _tree_node->num_visit;
    this->game_status = _tree_node->game_status;
    this->game = _tree_node->game;
    this->parent = _tree_node->parent;
	for (int i = 0; i < 18; i++) {
        this->is_legal_list[i] = _tree_node->is_legal_list[i];
		this->legal_move_list[i].first = _tree_node->legal_move_list[i].first;
    	this->legal_move_list[i].second = _tree_node->legal_move_list[i].second;
		this->children_list[i] = _tree_node->children_list[i];
	}

}
void Tree_node::operator=(const Tree_node& _tree_node) {

	this->score = _tree_node.score;
    this->num_visit = _tree_node.num_visit;
    this->game_status = _tree_node.game_status;
    this->game = _tree_node.game;
    this->parent = _tree_node.parent;
	this->depth = _tree_node.depth;
	this->is_expanded = _tree_node.is_expanded;
    for (int i = 0; i < 18; i++) {
 		this->is_legal_list[i] = _tree_node.is_legal_list[i];
		this->legal_move_list[i].first = _tree_node.legal_move_list[i].first;
    	this->legal_move_list[i].second = _tree_node.legal_move_list[i].second;
		this->children_list[i] = _tree_node.children_list[i];
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
	// cout << " update : " << this->score << ", " << this->num_visit << endl;
}

Tree_node* Tree_node::get_parent() {
	return this->parent;
}

void Tree_node::node_status() {
    cout << "vvvvvvvvvvvvvvv NODE_STATUS vvvvvvvvvvvvvvv" << endl;
    cout << "node : " << this << endl;
    this->game.print_board();
    this->game.print_status();
    printf("score : %f, visit : %d, status : %d, depth : %d, is_expanded : %s\n", score, num_visit, game_status, depth, is_expanded ? " true " : "false");
    printf("is_legal_list                   :");
    for(int i = 0; i < 18; i ++ ) printf("%c ", is_legal_list[i] ? 'T' : 'F'); printf("\n");
    printf("chess                           :");
    for(int i = 0; i < 18; i ++ ) printf("%d ", legal_move_list[i].first ); printf("\n");
    printf("direction                       :");
    for(int i = 0; i < 18; i ++ ) printf("%d ", legal_move_list[i].second); printf("\n");
    printf("check in board                  :");
    for(int i = 0; i < 18; i ++ ) printf("%c ", game.check_in_board(Movement(i/3, i%3)) ? 'T' : 'F'); printf("\n");
    printf("is exist                        :");
    for(int i = 0; i < 18; i ++ ) printf("%c ", game.get_cur_chs_list(i/3).exist ? 'T' : 'F'); printf("\n");
    cout << "^^^^^^^^^^^^^^^ NODE_STATUS ^^^^^^^^^^^^^^^" << endl;

}
