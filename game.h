#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
#include <map>
using namespace std;

// <dice, exist_status> maps to <available_chess, available_chess>
typedef map< pair<int, int>, pair<int, int> > Movable_chs_map;
// < the index of the currentPlayer (chs), direction >
typedef pair<int, int> Movement;

struct Chess
{
	char symbol;
	bool exist;
	int x, y;
	void assign(const char& symbol_, const bool& exist_, const int& x, const int& y);
	void move_x_one_unit(int direct);
	void move_y_one_unit(int direct);
};

class Game
{
private:
	char board[5][5];
	Chess movable_chs_list[2];
	//How many chessmen are on the board
	int exist_chs_num_A, exist_chs_num_B;
	Chess chs_list_A[6], chs_list_B[6];
	Chess* cur_chs_list_ptr;

	// bitwise: store the existance of the chessman the player has
	int exist_bitwise_A, exist_bitwise_B;
	int cur_exist_bitwise_ptr;
	//false(0): A, true(1): B
	bool turn;

public:
	static Movable_chs_map movable_chs_map;

	//Including initializing the positions
	Game();
	//Copy constructor
	Game(const Game& game);
	//Copy operator
	void operator=(const Game& game);

	static Movable_chs_map create_movable_chs_map();

	//Directly set the board
	void set_board(char board_[5][5], int turn_);
	//Print the board
	void print_board();
	//Roll the dice
	int roll_dice();
	//Input the result of dice and return how many chessmen the player can move
	int count_movable_chs(const int& dice);
	//Check if the chessman is out of the board
	bool is_in_board(const Movement& mvmt);
	//Synchronize the chessman and the board
	void set_chs_on_board(const Chess& chs);
	//Get the symbol of the chess originally on the board
	char get_symbol_on_board(const int& x, const int& y);
	//update_game_status the board(have a movement), chsNum, playerA/playerB. Return 0(game continues), 1(A wins), 2(B wins)
	int update_game_status(const Movement& mvmt);
	//Change turn and the pointer of the player
	void switch_player();
	//Print the position and existence of the both players
	void print_status();

	Chess get_movable_chs(const int& k);
	Chess get_cur_chs_list(const int& k);
	Chess get_chs_list(const bool turn_, const int& k);
	int get_exist_bitwise();
	bool get_turn();
};

#endif
