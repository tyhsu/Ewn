#include <iostream>
#include "play.h"
using namespace std;

Movement Play::player_move(const int& dice)
{
	int movable_chs_cnt = this->game.count_movable_chs(dice);
	int chs, direct;
	Movement mvmt;

	do {
		cout << endl << "Next chessman able to move: ";
		for (int i=0; i<movable_chs_cnt; i++)
			cout << i << ")" << this->game.get_movable_chs(i).symbol << " ";
		cout << endl;
		cout << "Choose: ";
		cin >> chs;

		if (!this->game.get_is_switch())
			cout << "Direction: 0)Right 1)Down 2)Right-down" << endl;
		else
			cout << "Direction: 0)Left 1)Up 2)Left-up" << endl;
		cout << "Choose: ";
		cin >> direct;

		// get index
		chs = (this->game.get_is_switch() == false) ? this->game.get_movable_chs(chs).symbol - '1' : this->game.get_movable_chs(chs).symbol - 'A';
		mvmt.first = chs;
		mvmt.second = direct;
		//If the movement is illegal, run the loop
	} while (!this->game.check_in_board(mvmt));

	return mvmt;
}

void Play::two_players_mode()
{
	this->game.print_board();
	while (1) {
		//update: 0(game continues), 1(A wins), 2(B wins)
		int dice = this->game.roll_dice();
		cout << "dice: " << dice+1 << endl;
		int game_status = this->game.update_game_status(player_move(dice));
		this->game.print_board();
		if (game_status != 0) {
			cout << "====================================" << endl;
			if (game_status == 1) cout << "A is the winner!!!" << endl;
			else cout << "B is the winner!!!" << endl;
			cout << "====================================" << endl;
			return;
		}
		this->game.switch_player();
	}
}

void Play::player_AI_mode()
{
	EwnAI ewnAI;
	this->game.print_board();
	while (1) {
		int dice = this->game.roll_dice();
		cout << "dice: " << dice+1 << endl;
		int game_status;
		if (this->game.get_is_switch() == true) {
			game_status = this->game.update_game_status(player_move(dice));
		}
		else
			game_status = this->game.update_game_status(ewnAI.AI_move(this->game, dice));

		this->game.print_board();
		//update: 0(game continues), 1(A wins), 2(B wins)
		if (game_status != 0) {
			cout << "====================================" << endl;
			if (game_status == 1) cout << "AI is the winner!!!" << endl;
			else cout << "B is the winner!!!" << endl;
			cout << "====================================" << endl;
			return;
		}
		this->game.switch_player();
	}
}

void Play::two_AIs_mode()
{
	cout << "==========The first AI (A)==========" << endl;
	EwnAI ewnAI1;
	cout << "==========The second AI (B)==========" << endl;
	EwnAI ewnAI2;
	this->game.print_board();
	while (1) {
		int dice = this->game.roll_dice();
		cout << "dice: " << dice+1 << endl;
		
		int game_status;
		if (!this->game.get_is_switch())	// the first AI (A)
			game_status = this->game.update_game_status(ewnAI1.AI_move(this->game, dice));
		else					// the second AI (B)
			game_status = this->game.update_game_status(ewnAI2.AI_move(this->game, dice));
		this->game.print_board();
		
		//update: 0(game continues), 1(A wins), 2(B wins)
		if (game_status!=0) {
			cout << "====================================" << endl;
			if (game_status==1) cout << "A is the winner!!!" << endl;
			else cout << "B is the winner!!!" << endl;
			cout << "====================================" << endl;
			return;
		}
		this->game.switch_player();
	}
}

void Play::contest_AI_mode()
{
	int is_left_upper_side;
	cout << "Whose AI is at the left upper section?" << endl;
	cout << "=>Who is A (1~6)? 0)Ours 1)Opponent's" << endl;
	cout << "Choose: ";
	cin >> is_left_upper_side;

	int is_init_board, is_switch;
	cout << "Do you need to set the board? 0)No 1)Yes" << endl;
	cout << "Choose: ";
	cin >> is_init_board;
	if (is_init_board == 1) {
		char board[5][5];
		do {
			cout << endl << "Please give values for the 5x5 array" << endl;
			char c;
			for (int i=0; i<5; i++) {
				for (int j=0; j<5; j++) {
					cin >> c;
					if (c=='0')
						board[i][j] = 0;
					else
						board[i][j] = c;
				}
			}
			cout << "Are you ready to set the board? 0)No 1)Yes" << endl;
			cout << "Choose: ";
			cin >> is_init_board;
		} while (!is_init_board);

		cout << endl << "Whose turn is it now? 0)A 1)B" << endl;
		cout << "Choose: ";
		cin >> is_switch;
		this->game.set_board(board, is_switch);
	}
	cout << endl;
	this->game.print_board();

	EwnAI ewnAI;
	if (is_left_upper_side) {	//A: Opponent's, B: Ours
		while (1) {
			int game_status;
			if (!this->game.get_is_switch()) {	//The opponent's turn (A)
				char chs;
				int direct;
				cout << endl << "Choose the chessman(1~6): ";
				cin >> chs;
				if (!(chs>='1' && chs<='6'))
					continue;

				cout << "Direction: 0)Right 1)Down 2)Right-down" << endl;
				cout << "Choose: ";
				cin >> direct;
				if (!(direct>=0 && direct<=2))
					continue;

				bool is_recover;
				cout << "Do you want to recover? 0)No 1)Yes" << endl;
				cout << "Choose: ";
				cin >> is_recover;
				if (!is_recover)
					continue;

				game_status = this->game.update_game_status(make_pair(chs-'1', direct));
			}

			else {	//AI's turn (B)
				int dice;
				cout << endl << "Set the dice(1~6): ";
				cin >> dice;
				if (!(dice>=1 && dice<=6))
					continue;

				game_status = this->game.update_game_status(ewnAI.AI_move(this->game, dice-1));
			}

			this->game.print_board();
			//update: 0(game continues), 1(A wins), 2(B wins)
			if (game_status!=0) {
				cout << "====================================" << endl;
				if (game_status==1) cout << "The opponent is the winner!!!" << endl;
				else cout << "Our AI is the winner!!!" << endl;
				cout << "====================================" << endl;
				return;
			}
			this->game.switch_player();
		}
	}
	else {				//A: Ours, B: Opponent's
		while (1) {
			int game_status;
			if (this->game.get_is_switch()) {	//The opponent's turn (B)
				char chs;
				int direct;
				cout << endl << "Choose the chessman(A~F): ";
				cin >> chs;
				if (!(chs>='A' && chs<='F'))
					continue;

				cout << "Direction: 0)Left 1)Up 2)Left-up" << endl;
				cout << "Choose: ";
				cin >> direct;
				if (!(direct>=0 && direct<=2))
					continue;

				int is_recover;
				cout << "Do you want to recover? 0)No 1)Yes" << endl;
				cout << "Choose: ";
				cin >> is_recover;
				if (is_recover != 0)
					continue;

				game_status = this->game.update_game_status(make_pair(chs-'A', direct));
			}

			else {	//AI's turn (A)
				int dice;
				cout << endl << "Set the dice(1~6): ";
				cin >> dice;
				if (!(dice>=1 && dice<=6))
					continue;

				game_status = this->game.update_game_status(ewnAI.AI_move(this->game, dice-1));
			}

			this->game.print_board();
			//update: 0(game continues), 1(A wins), 2(B wins)
			if (game_status!=0) {
				//this->game.print_status();
				cout << "====================================" << endl;
				if (game_status==2) cout << "The opponent is the winner!!!" << endl;
				else cout << "Our AI is the winner!!!" << endl;
				cout << "====================================" << endl;
				return;
			}
			this->game.switch_player();
		}

	}
}
