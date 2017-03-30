#include <iostream>
#include <cstdio>
#include <cmath>
#include "game.h"
using namespace std;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* -------------------- struct Player -------------------- */

void Player::assign(const int& chs_index, const int& x, const int& y)
{
	unsigned int pos_mask = 1;
	unsigned int pos_origin, pos_assign;

	// use a loop instead of pow() because it is integer
	for (int i=0; i<chs_index; i++)
		pos_mask *= 6*6;	// (x, y)
	
	pos_origin = this->chs_pos % (pos_mask * 6*6) / pos_mask;
	pos_assign = x * 6 + y;
	this->chs_pos += (pos_assign - pos_origin) * pos_mask;
}

void Player::move_x_one_unit(const int& chs_index, const int& direct)
{
	unsigned int pos_mask = 1;
	// use a loop instead of pow() because it is integer
	for (int i=0; i<chs_index; i++)
		pos_mask *= 6*6;	// (x, y)
	
	this->chs_pos += (direct * 6) * pos_mask;
}

void Player::move_y_one_unit(const int& chs_index, const int& direct)
{
	unsigned int pos_mask = 1;
	// use a loop instead of pow() because it is integer
	for (int i=0; i<chs_index; i++)
		pos_mask *= 6*6;	// (x, y)
	
	this->chs_pos += direct * pos_mask;
}

/* -------------------- class Game -------------------- */
Game::Game()
{
	memset(this->board, 0, sizeof(this->board));
	memset(this->movable_chs_list, 0, sizeof(this->movable_chs_list));
	this->exist_chs_num_A = 6, this->exist_chs_num_B = 6;
	for (int i=0; i<6; i++)
		this->chs_list_A[i].assign('1'+i, true, 0, 0);
	for (int i=0; i<6; i++)
		this->chs_list_B[i].assign('A'+i, true, 0, 0);

	this->exist_bitwise_A = 0b111111;
	this->exist_bitwise_B = 0b111111;

	this->cur_chs_list_ptr = this->chs_list_A;
	this->cur_exist_bitwise_ptr = exist_bitwise_A;
	this->is_switch = false;

	//Initialize the positions of the chessmen
	//A: 1~6	B: A~F
	int chs_pos_index = 0;
	int chs_pos_A[6][2] = { {0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {2,0} };
	int chs_pos_B[6][2] = { {4,4}, {4,3}, {4,2}, {3,4}, {3,3}, {2,4} };
	int (*current_chs_pos_ptr)[2] = &(chs_pos_A[0]);
	int chs_unuse_bitwise = 0b111111;
	char symbol_ = '1';
	srand( time(NULL) + getpid() );
	for (int i=0; i<2; i++) {
		//Every position selects a number
		while (chs_pos_index<6) {
			int chs_index = rand()%6;	//0~5
			if ((chs_unuse_bitwise & (1<<chs_index)) >> chs_index) {	//The number hasn't been used
				int x_ = current_chs_pos_ptr[chs_pos_index][0];
				int y_ = current_chs_pos_ptr[chs_pos_index][1];
				this->board[x_][y_] = symbol_ + chs_index;
				this->cur_chs_list_ptr[chs_index].x = x_;
				this->cur_chs_list_ptr[chs_index].y = y_;
				chs_pos_index++;
				chs_unuse_bitwise = chs_unuse_bitwise ^ (1<<chs_index);	//The used number is discarded
			}
		}
		chs_pos_index = 0;
		chs_unuse_bitwise = 0b111111;
		this->cur_chs_list_ptr = this->chs_list_B;
		current_chs_pos_ptr = chs_pos_B;
		symbol_ = 'A';
	}
	this->cur_chs_list_ptr = this->chs_list_A;	//recover the initialization
}

Game::Game(const Game& game)
{
	for(int i=0; i<5; i++) {
		for(int j=0; j<5; j++) {
			this->board[i][j] = game.board[i][j];
		}
	}
	for(int i=0; i<2; i++) {
		this->movable_chs_list[i] = game.movable_chs_list[i];
	}
	for(int i=0; i<6; i++) {
		this->chs_list_A[i] = game.chs_list_A[i];
		this->chs_list_B[i] = game.chs_list_B[i];
	}
	this->exist_bitwise_A = game.exist_bitwise_A;
	this->exist_bitwise_B = game.exist_bitwise_B;
	this->is_switch = game.is_switch;
	this->cur_chs_list_ptr = (this->is_switch == false) ? this->chs_list_A :  this->chs_list_B;
	this->cur_exist_bitwise_ptr = (this->is_switch == false) ? this->exist_bitwise_A :  this->exist_bitwise_B;
	this->exist_chs_num_A = game.exist_chs_num_A;
	this->exist_chs_num_B = game.exist_chs_num_B;
}

void Game::operator=(const Game& game)
{
	for(int i=0; i<5; i++) {
		for(int j=0; j<5; j++) {
			this->board[i][j] = game.board[i][j];
		}
	}
	for(int i=0; i<2; i++) {
		this->movable_chs_list[i] = game.movable_chs_list[i];
	}
	for(int i=0; i<6; i++) {
		this->chs_list_A[i] = game.chs_list_A[i];
		this->chs_list_B[i] = game.chs_list_B[i];
	}
	this->exist_bitwise_A = game.exist_bitwise_A;
	this->exist_bitwise_B = game.exist_bitwise_B;
	this->is_switch = game.is_switch;
	this->cur_chs_list_ptr = (game.is_switch) ? this->chs_list_B : this->chs_list_A;
	this->exist_chs_num_A = game.exist_chs_num_A;
	this->exist_chs_num_B = game.exist_chs_num_B;
//	cout << "calling copy : "  << this->is_switch << " " << this->cur_chs_list_ptr << "  " << this->chs_list_A << "  " << this->chs_list_B << "  " << endl;
}

void Game::set_board(char board_[5][5], int is_switch_)
{
	int chs_num_A_ = 0, chs_num_B_ = 0;
	int exist_bitwise_A_ = 0, exist_bitwise_B_ = 0;
	for (int i=0; i<6; i++) {
		this->chs_list_A[i].assign(0, false, 0, 0);
		this->chs_list_B[i].assign(0, false, 4, 4);
	}

	for (int i=0; i<5; i++) {
		for (int j=0; j<5; j++) {
			char c = board_[j][i];

			//Set the board
			this->board[i][j] = c;

			//Update other information
			if (c>='1' && c<='6') {
				chs_num_A_++;
				this->chs_list_A[c-'1'].assign(c, true, i, j);
				exist_bitwise_A_ |= 1 << (c-'1');
			}
			else if (c>='A' && c<='f') {
				chs_num_B_++;
				this->chs_list_B[c-'A'].assign(c, true, i, j);
				exist_bitwise_B_ |= 1 << (c-'A');
			}
		}
	}
	this->exist_chs_num_A = chs_num_A_;
	this->exist_chs_num_B = chs_num_B_;
	this->exist_bitwise_A = exist_bitwise_A_;
	this->exist_bitwise_B = exist_bitwise_B_;

	if (is_switch_) {
		this->cur_chs_list_ptr = this->chs_list_B;
		this->cur_exist_bitwise_ptr = this->exist_bitwise_B;
		this->is_switch = true;
	}
	else {
		this->cur_chs_list_ptr = this->chs_list_A;
		this->cur_exist_bitwise_ptr = this->exist_bitwise_A;
		this->is_switch = false;
	}
}

void Game::print_board()
{
	cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "The current board:" << endl;
	cout << "     /-----\\" << endl;
	for (int i=0; i<5; i++) {
		cout << "     |";
		for (int j=0; j<5; j++) {
			char c = this->board[j][i];
			if (c==0) cout << " ";
			else if (c >= 'A' && c <= 'F')
				cout << ANSI_COLOR_RED << c << ANSI_COLOR_RESET;
			else
				cout << ANSI_COLOR_YELLOW << c << ANSI_COLOR_RESET;
			//else cout << c;
		}
		cout << "|" << endl;
	}
	cout << "     \\-----/" << endl;
	cout << "====================================" << endl;
}

int Game::roll_dice()
{
	return rand()%6;
}

int Game::count_movable_chs(const int& dice)
{
	for(int i = 0; i < 2; i++)
		this->movable_chs_list[i].assign(0, false, 0, 0);

	if (this->cur_chs_list_ptr[dice].exist == true) {
		// this piece is alive!
		for(int i = 0; i < 2; i++)
			this->movable_chs_list[i] = this->cur_chs_list_ptr[dice];
		return 1;
	}
	else {
		int movable_chs_cnt = 2;

		// search for maybe other two.
		for (int i = dice-1; i >= 0; i--) {
			if (this->cur_chs_list_ptr[i].exist){
				this->movable_chs_list[0] = this->cur_chs_list_ptr[i];
				break;
			}
		}
		for (int i = dice+1; i < 6; i++) {
			if (this->cur_chs_list_ptr[i].exist){
				this->movable_chs_list[1] = this->cur_chs_list_ptr[i];
				break;
			}
		}

		// if one of movable_chs_list doesn't value, let this Chs = the other one (with value).
		for (int i = 0; i < 2; i++) {
			if (this->movable_chs_list[i].symbol == 0) {
				this->movable_chs_list[i] = this->movable_chs_list[1-i];
				movable_chs_cnt--;
			}
		}
		return movable_chs_cnt;
	}
}

bool Game::check_in_board(const Movement& mvmt)
{
	Chess chs = this->cur_chs_list_ptr[mvmt.first];
	int side = (this->is_switch == false) ? 1 : -1;
	int x = chs.x, y = chs.y;
	x += (mvmt.second == 1) ? 0 : side;
	y += (mvmt.second == 0) ? 0 : side;
	return ( x >= 0 && x < 5 && y >= 0 && y < 5);
}

void Game::set_chs_on_board(const Chess& chs)
{
	this->board[chs.x][chs.y] = chs.symbol;
}

char Game::get_symbol_on_board(const int& x, const int& y)
{
	return this->board[x][y];
}

int Game::update_game_status(const Movement& mvmt)
{
	Chess next_chs = this->cur_chs_list_ptr[mvmt.first];
	int direct = mvmt.second;
	int side = (this->is_switch == false)? 1 : -1;
	char eaten_chs_symbol;

	// Clear previous location
	Chess blank_chs = next_chs;
	blank_chs.symbol = 0;
	this->set_chs_on_board(blank_chs);
	// Update the chosen movement on the board
	int chs_list_index = 0;
	if (!this->is_switch)	//chs_list_A
		chs_list_index = next_chs.symbol - '1';
	else				//chs_list_B
		chs_list_index = next_chs.symbol - 'A';

	if (direct == 0)		// move right
		this->cur_chs_list_ptr[chs_list_index].move_x_one_unit(side);
	else if (direct == 1)	// move down
		this->cur_chs_list_ptr[chs_list_index].move_y_one_unit(side);
	else if (direct == 2)	// move right down
		this->cur_chs_list_ptr[chs_list_index].move_x_one_unit(side),
		this->cur_chs_list_ptr[chs_list_index].move_y_one_unit(side);

	Chess chs = this->cur_chs_list_ptr[chs_list_index];
	eaten_chs_symbol = this->get_symbol_on_board(chs.x, chs.y);
	this->set_chs_on_board(chs);

	// Update the eaten chessman
	if (eaten_chs_symbol != 0) {
		// should delete a single piece.
		if (eaten_chs_symbol < 'A') {
			// this Play is A
			this->chs_list_A[eaten_chs_symbol-'1'].exist = false;
			this->exist_bitwise_A -= 1 << int(eaten_chs_symbol-'1');
			this->exist_chs_num_A--;
		}
		else {
			// this Play is B
			this->chs_list_B[eaten_chs_symbol-'A'].exist = false;
			this->exist_bitwise_B -= 1 << int(eaten_chs_symbol-'A');
			this->exist_chs_num_B--;
		}
	}

	// Check if any player arrives the end-game position
	for (int i = 0; i < 6; i++) {
		if(this->chs_list_A[i].x == 4 && this->chs_list_A[i].y == 4)
			return 1;
		if(this->chs_list_B[i].x == 0 && this->chs_list_B[i].y == 0)
			return 2;
	}

	// Check if the other player is killed the game.
	if (!this->is_switch && this->exist_chs_num_B==0)		//this player is A
		return 1;
	else if (this->is_switch && this->exist_chs_num_A==0)	//this player is B
		return 2;

	//The game has not come to a close
	return 0;
}

void Game::switch_player()
{
	if (this->is_switch) {	//now is B
		this->cur_chs_list_ptr = this->chs_list_A;
		this->cur_exist_bitwise_ptr = this->exist_bitwise_A;
		this->is_switch = false;
	}
	else {				//now is A
		this->cur_chs_list_ptr = this->chs_list_B;
		this->cur_exist_bitwise_ptr = this->exist_bitwise_B;
		this->is_switch = true;
	}
}

void Game::print_status()
{
	cout << this->is_switch << " " << this->cur_chs_list_ptr << "  " << this->chs_list_A << "  " << this->chs_list_B << "  " << endl;
	cout << "/*    Player A    */" << endl;
	for(int i=0; i<6; i++) {
		printf("%c", '1'+i);
		cout << " is at (" << this->chs_list_A[i].x << ", " << this->chs_list_A[i].y << ")   [exist : " << this->chs_list_A[i].exist << "] ;" << endl;
	}
	cout << "/*    --------    */" << endl;

	cout << "/*    Player B    */" << endl;
	for(int i=0; i<6; i++) {
		printf("%c", 'A'+i);
		cout << " is at (" << this->chs_list_B[i].x << ", " << this->chs_list_B[i].y << ")   [exist : " << this->chs_list_B[i].exist << "] ;" << endl;
	}
	cout << "/*    --------    */" << endl;
}

Chess Game::get_movable_chs(const int& k)
{
	return this->movable_chs_list[k];
}

Chess Game::get_cur_chs_list(const int& k)
{
	return this->cur_chs_list_ptr[k];
}

Chess Game::get_chs_list(const bool is_switch_, const int& k)
{
	if (is_switch_)	//now is playerB
		return this->chs_list_B[k];
	else		//now is playerA
		return this->chs_list_A[k];
}

int Game::get_exist_bitwise()
{
	return this->cur_exist_bitwise_ptr;
}

bool Game::get_is_switch()
{
	return this->is_switch;
}

Movable_chs_map Game::create_movable_chs_map()
{
	Movable_chs_map movable_chs_map_;
	for(int exist_=0; exist_<(1<<6); exist_++) { // each exist status
		for(int dice=0; dice<6; dice++) {
			int former_chs_index = -1, latter_chs_index = -1;
			for(int i=dice-1; i>=0; i--) {
				if((exist_%(1<<(i+1)))/(1<<i) == true) {
					former_chs_index = i;
					break;
				}
			}
			for(int i=dice+1; i<6; i++) {
				if((exist_>>i)%2 == true) {
					latter_chs_index = i;
					break;
				}
			}
			former_chs_index = (former_chs_index == -1) ? latter_chs_index : former_chs_index;
			latter_chs_index = (latter_chs_index == -1) ? former_chs_index : latter_chs_index;
			pair<int, int> fir(dice, exist_), sec(former_chs_index, latter_chs_index);
			movable_chs_map_.insert(make_pair(fir, sec));
		}
	}
	return movable_chs_map_;
}
