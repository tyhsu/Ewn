#include "game.h"
#include "tree_node.h"

Tree_node::Tree_node() {
	this->win_count = 0;
	this->visit_count = 0;
	this->game_status = 0;
	this->parent_ptr = NULL;
	for (int i = 0; i < 18; i++)
		this->children_ptr_list[i] = NULL;
}

Tree_node::Tree_node(const Game& game_) {
	this->win_count = 0;
	this->visit_count = 0;
	this->game_status = 0;
	this->game = game_;
	this->parent_ptr = NULL;
	new_child_nodes();
}

Tree_node::Tree_node(Tree_node* node) {
	this->win_count = node->win_count;
	this->visit_count = node->visit_count;
	this->game_status = node->game_status;
	this->game = node->game;
	this->parent_ptr = node->parent_ptr;
	for (int i = 0; i < 18; i++) {
		this->children_ptr_list[i] = node->children_ptr_list[i];
	}
}

void Tree_node::operator=(const Tree_node& node) {
	this->win_count = node.win_count;
	this->visit_count = node.visit_count;
	this->game_status = node.game_status;
	this->game = node.game;
	this->parent_ptr = node.parent_ptr;
	for (int i = 0; i < 18; i++) {
		this->children_ptr_list[i] = node.children_ptr_list[i];
	}
}

bool Tree_node::is_terminate() {
	return this->game_status != 0;
}

bool Tree_node::is_visit() {
	return visit_count != 0;
}

void Tree_node::new_child_nodes() {
	int legal_child_num = 0;
	// new the legal nodes for children_ptr
	for (int i = 0; i < 18; i++) {
		// check if the movement is legal
		int chs_index = i / 3, direction = i % 3;
		if (!this->game.get_cur_chs_list(chs_index).exist) continue;
		Movement movement(chs_index, direction);
		if (!this->game.check_in_board(movement)) continue;

		// new legal child nodes
		legal_child_num++;
		this->children_ptr_list[i] = new Tree_node();
		Tree_node* node_ptr = this->children_ptr_list[i];

		// reset the values of the members
		node_ptr->game = this->game;
		node_ptr->game_status = node_ptr->game.update_game_status(movement);
		node_ptr->game.switch_player();
		node_ptr->parent_ptr = this;
	}
	// reset the rest of the children_ptr to NULL
	for (int i = legal_child_num; i < 18; i++)
		this->children_ptr_list[i] = NULL;
}

void Tree_node::set_win_count(float win_count_) {
	this->win_count = win_count_;
}

void Tree_node::update(float win_count_) {
	this->visit_count ++;
	this->win_count += win_count_;
	// cout << " update : " << this->win_count << ", " << this->visit_count << endl;
}

int Tree_node::get_visit_count() {
	return this->visit_count;
}

Tree_node* Tree_node::get_parent_ptr() {
	return this->parent_ptr;
}

Tree_node* Tree_node::get_child_ptr(const int& child_index) {
	return this->children_ptr_list[child_index];
}

void Tree_node::node_status() {
	cout << "vvvvvvvvvvvvvvv NODE_STATUS vvvvvvvvvvvvvvv" << endl;
	cout << "node : " << this << endl;
	this->game.print_board();
	this->game.print_status();
	printf("win_count: %d, visit: %d, status: %d\n", this->win_count, this->visit_count, this->game_status);

	printf("check in board                  :");
	for(int i = 0; i < 18; i ++ )
		printf("%c ", this->game.check_in_board(Movement(i/3, i%3)) ? 'T' : 'F'); printf("\n");

	printf("is exist                        :");
	for(int i = 0; i < 18; i ++ )
		printf("%c ", this->game.get_cur_chs_list(i/3).exist ? 'T' : 'F'); printf("\n");
	cout << "^^^^^^^^^^^^^^^ NODE_STATUS ^^^^^^^^^^^^^^^" << endl;
}
