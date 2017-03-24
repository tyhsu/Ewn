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

Tree_node::Tree_node(const Game& game_, Tree_node* parent_ptr_) {
	this->win_count = 0;
	this->visit_count = 0;
	this->game_status = 0;
	this->game = game_;
	this->parent_ptr = parent_ptr_;	// strange?
	for (int i = 0; i < 18; i++) {
		Movement movement(i / 3, i % 3);
		this->children_ptr_list[i] = new Tree_node();
		this->children_ptr_list[i]->parent_ptr = this;
	}
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

void Tree_node::set_child_nodes(const Game& game_, Tree_node* parent_) {

}

void Tree_node::set_win_count(float win_count_) {
	this->win_count = win_count_;
}

void Tree_node::update(float win_count_) {
	this->visit_count ++;
	this->win_count += win_count_;
	// cout << " update : " << this->win_count << ", " << this->visit_count << endl;
}

Tree_node* Tree_node::get_parent() {
	return this->parent_ptr;
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
