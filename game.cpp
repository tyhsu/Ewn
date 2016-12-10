#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
#include "game.h"
using namespace std;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* -------------------- struct Chess -------------------- */
void Chess::assign(const char& s, const bool& e, const int& x, const int& y)
{
	this->symbol = s;
	this->exist = e;
	this->x = x;
	this->y = y;
}

void Chess::move_x_one_unit(int direction)
{
	this->x += direction;
}

void Chess::move_y_one_unit(int direction)
{
	this->y += direction;
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

	this->current_chs_list_ptr = chs_list_A;
	this->current_exist_bitwise_ptr = exist_bitwise_A;
	this->turn = false;

	//Initialize the positions of the chessmen
	//A: 1~6
	int currPos = 0;
	int positionA[6][2] = { {0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {2,0} };
	int six = 0x3f;	//11,1111(2)
	srand( time(NULL) + getpid() );
	//Every position selects a number
	while (currPos<6) {
		int number = rand()%6;	//0~5
		if ((six & (1<<number)) >> number) {	//The number hasn't been used
			int posX = positionA[currPos][0], posY = positionA[currPos][1];
			this->board[posX][posY] = '1' + number;
			this->chs_list_A[number].x = posX;
			this->chs_list_A[number].y = posY;
			currPos++;
			six = six ^ (1<<number);	//The used number is discarded
		}
	}
	//B: A~F
	currPos = 0;
	int positionB[6][2] = { {4,4}, {4,3}, {4,2}, {3,4}, {3,3}, {2,4} };
	six = 0x3f;	//11,1111(2)
	while (currPos<6) {
		int number = rand()%6;	//0~6
		if ((six & (1<<number)) >> number) {	//The number hasn't been used
			int posX = positionB[currPos][0], posY = positionB[currPos][1];
			this->board[posX][posY] = 'A' + number;
			this->chs_list_B[number].x = posX;
			this->chs_list_B[number].y = posY;
			currPos++;
			six = six ^ (1<<number);	//The used number is discarded
		}
	}
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
	this->turn = game.turn;
	this->current_chs_list_ptr = (this->turn == false) ? this->chs_list_A :  this->chs_list_B;
	this->current_exist_bitwise_ptr = (this->turn == false) ? this->exist_bitwise_A :  this->exist_bitwise_B;
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
	this->turn = game.turn;
	this->exist_chs_num_A = game.exist_chs_num_A;
	this->exist_chs_num_B = game.exist_chs_num_B;
}

void Game::set_board(char board[5][5], int turn)
{
	int chsNumA = 0, chsNumB = 0;
	int existA = 0, existB = 0;
	for (int i=0; i<6; i++) {
		this->chs_list_A[i].assign(0, false, 0, 0);
		this->chs_list_B[i].assign(0, false, 4, 4);
	}

	for (int i=0; i<5; i++) {
		for (int j=0; j<5; j++) {
			char c = board[j][i];

			//Set the board
			this->board[i][j] = c;

			//Update other information
			if (c>='1' && c<='6') {
				chsNumA++;
				this->chs_list_A[c-'1'].assign(c, true, i, j);
				existA |= 1 << (c-'1');
			}
			else if (c>='A' && c<='f') {
				chsNumB++;
				this->chs_list_B[c-'A'].assign(c, true, i, j);
				existB |= 1 << (c-'A');
			}
		}
	}
	this->exist_chs_num_A = chsNumA;
	this->exist_chs_num_B = chsNumB;
	this->exist_bitwise_A = existA;
	this->exist_bitwise_B = existB;

	if (turn) {
		this->current_chs_list_ptr = this->chs_list_B;
		this->current_exist_bitwise_ptr = this->exist_bitwise_B;
		this->turn = true;
	}
	else {
		this->current_chs_list_ptr = this->chs_list_A;
		this->current_exist_bitwise_ptr = this->exist_bitwise_A;
		this->turn = false;
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
	//printf("dice: %d \n", dice+1);
	for(int i = 0; i < 2; i++)
		this->movable_chs_list[i].assign(0, false, 0, 0);

	if (this->current_chs_list_ptr[dice].exist == true) {
		// this piece is alive!
		for(int i = 0; i < 2; i++) {
			this->movable_chs_list[i] = this->current_chs_list_ptr[dice];
		}
		return 1;
	}
	else {
		//cout << "Chessman " << this->current_chs_list_ptr[dice].symbol << " doesn't exist" << endl;
		int move = 2;

		// search for maybe other two.
		for (int i = dice-1; i >= 0; i--) {
			if (this->current_chs_list_ptr[i].exist){
				this->movable_chs_list[0] = this->current_chs_list_ptr[i];
				break;
			}
		}
		for (int i = dice+1; i < 6; i++) {
			if (this->current_chs_list_ptr[i].exist){
				this->movable_chs_list[1] = this->current_chs_list_ptr[i];
				break;
			}
		}

		// if one of movable_chs_list doesn't value, let this Chs = the other one (with value).
		for (int i = 0; i < 2; i++) {
			if (this->movable_chs_list[i].symbol == 0) {
				this->movable_chs_list[i] = this->movable_chs_list[1-i];
				move--;
			}
		}
		return move;
	}
}

bool Game::is_in_board(const Movement& mvmt)
{
	Chess chess = this->current_chs_list_ptr[mvmt.first];
	int posneg = (this->turn == false) ? 1 : -1;
	int x = chess.x, y = chess.y;
	x += (mvmt.second == 1) ? 0 : posneg;
	y += (mvmt.second == 0) ? 0 : posneg;
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
	Chess chessToGo = this->current_chs_list_ptr[mvmt.first];
	char replacedChess;
	int direct = mvmt.second;
	int posneg = (this->turn == false)? 1 : -1;

	// Update the chosen movement on the board

	// clear previous location
	Chess previous = chessToGo;
	previous.symbol = 0;
	this->set_chs_on_board(previous);

	int playerIndex = 0;
	if (!this->turn)	//chs_list_A
		playerIndex = chessToGo.symbol - '1';
	else				//chs_list_B
		playerIndex = chessToGo.symbol - 'A';

	if (direct == 0) {
		/* ----------move right---------- */
		this->current_chs_list_ptr[playerIndex].move_x_one_unit(posneg);
	}
	else if (direct == 1) {
		/* ----------move down---------- */
		this->current_chs_list_ptr[playerIndex].move_y_one_unit(posneg);
	}
	else if (direct == 2) {
		/* ----------move right down---------- */
		this->current_chs_list_ptr[playerIndex].move_x_one_unit(posneg);
		this->current_chs_list_ptr[playerIndex].move_y_one_unit(posneg);
	}
	Chess chs = this->current_chs_list_ptr[playerIndex];
	replacedChess = this->get_symbol_on_board(chs.x, chs.y);
	this->set_chs_on_board(this->current_chs_list_ptr[playerIndex]);

	// Update the eaten chessman
	if (replacedChess != 0) {
		// should delete a single piece.
		if (replacedChess < 'A') {
			// this Play is A
			this->chs_list_A[replacedChess-'1'].exist = false;
			this->exist_bitwise_A -= 1<<int(replacedChess-'1');
			this->exist_chs_num_A--;
		}
		else {
			// this Play is B
			this->chs_list_B[replacedChess-'A'].exist = false;
			this->exist_bitwise_B-= 1<<int(replacedChess-'A');
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
	if (!this->turn && this->exist_chs_num_B==0)		//this player is A
		return 1;
	else if (this->turn && this->exist_chs_num_A==0)	//this player is B
		return 2;

	//The game has not come to a close
	return 0;
}

void Game::switch_player()
{
	if (this->turn) {	//now is B
		this->current_chs_list_ptr = this->chs_list_A;
		this->current_exist_bitwise_ptr = this->exist_bitwise_A;
		this->turn = false;
	}
	else {				//now is A
		this->current_chs_list_ptr = this->chs_list_B;
		this->current_exist_bitwise_ptr = this->exist_bitwise_B;
		this->turn = true;
	}
}

void Game::print_status()
{
	cout <<"/*    Player A    */" <<endl;
	for(int i=0; i<6; i++) {
		printf("%c", '1'+i);
		cout << " is at (" << this->chs_list_A[i].x << ", " << this->chs_list_A[i].y << ")   [exist : " << this->chs_list_A[i].exist << "] ;" <<endl;
	}
	cout <<"/*    --------    */" << endl;

	cout <<"/*    Player B    */" <<endl;
	for(int i=0; i<6; i++) {
		printf("%c", 'A'+i);
		cout << " is at (" << this->chs_list_B[i].x << ", " << this->chs_list_B[i].y << ")   [exist : " << this->chs_list_B[i].exist << "] ;" <<endl;
	}
	cout <<"/*    --------    */" << endl;
}

Chess Game::get_movable_chs(const int& k)
{
	return this->movable_chs_list[k];
}

Chess Game::get_current_chs_list(const int& k)
{
	return this->current_chs_list_ptr[k];
}

Chess Game::get_chs_list(const bool turn, const int& k)
{
	if (turn)	//now is playerB
		return this->chs_list_B[k];
	else		//now is playerA
		return this->chs_list_A[k];
}

int Game::get_exist_bitwise() {
	return current_exist_bitwise_ptr;
}

bool Game::get_turn()
{
	return this->turn;
}

Movable_chs_map Game::create_movable_chs_map()
{
	Movable_chs_map _am;
	for(int exist=0; exist<(1<<6); exist++) { // each exist status
		for(int dice=0; dice<6; dice++) {
			int availableFormer = -1, availableLatter = -1;
			int tmp = exist;
			int bin[6];
			int f = exist, l = exist;
			for(int i=dice; i>=0; i--) {
				if((exist%(1<<(i+1)))/(1<<i) == true) {
					availableFormer = i;
					break;
				}
			}
			for(int i=dice; i<6; i++) {
				if((exist>>i)%2 == true) {
					availableLatter = i;
					break;
				}
			}
			availableFormer = (availableFormer == -1) ? availableLatter : availableFormer;
			availableLatter = (availableLatter == -1) ? availableFormer : availableLatter;
			pair<int, int> fir(dice, exist), sec(availableFormer, availableLatter);
			_am.insert(make_pair(fir, sec));
		}
	}
	return _am;
}
