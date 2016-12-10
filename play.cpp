#include <iostream>
#include "play.h"
using namespace std;

Movement Play::playerPlay(const int& dice)
{
	int nextMoveCnt = this->game_.count_movable_chs(dice);
	int chs, direct;
	Movement mvmt;

	do {
		cout << endl << "Next chessman able to move: ";
		for (int i=0; i<nextMoveCnt; i++)
			cout << i << ")" << this->game_.get_movable_chs(i).symbol << " ";
		cout << endl;
		cout << "Choose: ";
		cin >> chs;

		if (!this->game_.get_turn())
			cout << "Direction: 0)Right 1)Down 2)Right-down" << endl;
		else
			cout << "Direction: 0)Left 1)Up 2)Left-up" << endl;
		cout << "Choose: ";
		cin >> direct;

		// get index
		chs = (this->game_.get_turn() == false) ? this->game_.get_movable_chs(chs).symbol - '1' : this->game_.get_movable_chs(chs).symbol - 'A';
		mvmt.first = chs;
		mvmt.second = direct;
		//If the movement is illegal, run the loop
	} while (!this->game_.is_in_board(mvmt));

	return mvmt;
}

// void Play::recover() {
// 	this->game_ = this->recoverBuffer_;
// }

void Play::twoPlayers()
{
	game_.print_board();
	while (1) {
		//update: 0(game continues), 1(A wins), 2(B wins)
		int dice = this->game_.roll_dice();
		cout << "dice: " << dice+1 << endl;
		int win = this->game_.update_game_status(playerPlay(dice));
		this->game_.print_board();
		if (win!=0) {
			cout << "====================================" << endl;
			if (win==1) cout << "A is the winner!!!" << endl;
			else cout << "B is the winner!!!" << endl;
			cout << "====================================" << endl;
			return;
		}
		this->game_.switch_player();
	}
}

void Play::playerAI()
{
	EwnAI ewnAI;
	game_.print_board();
	while (1) {
		int dice = this->game_.roll_dice();
		cout << "dice: " << dice+1 << endl;
		int win;
		if (this->game_.get_turn() == true) {
			win = this->game_.update_game_status(playerPlay(dice));
		}
		else
			win = this->game_.update_game_status(ewnAI.auto_play(this->game_, dice));

		this->game_.print_board();
		//update: 0(game continues), 1(A wins), 2(B wins)
		if (win!=0) {
			cout << "====================================" << endl;
			if (win==1) cout << "AI is the winner!!!" << endl;
			else cout << "B is the winner!!!" << endl;
			cout << "====================================" << endl;
			return;
		}
		this->game_.switch_player();
	}
}

void Play::twoAIs()
{
	cout << "==========The first AI (A)==========" << endl;
	EwnAI ewnAI1;
	cout << "==========The second AI (B)==========" << endl;
	EwnAI ewnAI2;
	game_.print_board();
	while (1) {
		int dice = this->game_.roll_dice();
		cout << "dice: " << dice+1 << endl;
		int win;
		if (!game_.get_turn())	// the first AI (A)
			win = this->game_.update_game_status(ewnAI1.auto_play(this->game_, dice));
		else					// the second AI (B)
			win = this->game_.update_game_status(ewnAI2.auto_play(this->game_, dice));
		this->game_.print_board();
		//update: 0(game continues), 1(A wins), 2(B wins)
		if (win!=0) {
			cout << "====================================" << endl;
			if (win==1) cout << "A is the winner!!!" << endl;
			else cout << "B is the winner!!!" << endl;
			cout << "====================================" << endl;
			return;
		}
		this->game_.switch_player();
	}
}

void Play::contestAI()
{
	int leftUpper;
	cout << "Whose AI is at the left upper section?" << endl;
	cout << "=>Who is A (1~6)? 0)Ours 1)Opponent's" << endl;
	cout << "Choose: ";
	cin >> leftUpper;

	int initial, turn;
	cout << "Do you need to set the board? 0)No 1)Yes" << endl;
	cout << "Choose: ";
	cin >> initial;
	if (initial == 1) {
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
			cin >> initial;
		} while (initial == 0);

		cout << endl << "Whose turn is it now? 0)A 1)B" << endl;
		cout << "Choose: ";
		cin >> turn;
		game_.set_board(board, turn);
	}
	cout << endl;
	//game_.print_status();
	game_.print_board();

	EwnAI ewnAI;
	if (leftUpper) {	//A: Opponent's, B: Ours
		while (1) {
			int win;
			if (!this->game_.get_turn()) {	//The opponent's turn (A)
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

				int re;
				cout << "Do you want to recover? 0)No 1)Yes" << endl;
				cout << "Choose: ";
				cin >> re;
				if (re != 0)
					continue;

				win = this->game_.update_game_status(make_pair(chs-'1', direct));
			}

			else {	//AI's turn (B)
				int dice;
				cout << endl << "Set the dice(1~6): ";
				cin >> dice;
				if (!(dice>=1 && dice<=6))
					continue;

				win = this->game_.update_game_status(ewnAI.auto_play(this->game_, dice-1));
			}

			this->game_.print_board();
			//update: 0(game continues), 1(A wins), 2(B wins)
			if (win!=0) {
				//this->game_.print_status();
				cout << "====================================" << endl;
				if (win==1) cout << "The opponent is the winner!!!" << endl;
				else cout << "Our AI is the winner!!!" << endl;
				cout << "====================================" << endl;
				return;
			}
			this->game_.switch_player();
		}
	}
	else {				//A: Ours, B: Opponent's
		while (1) {
			int win;
			if (this->game_.get_turn()) {	//The opponent's turn (B)
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

				int re;
				cout << "Do you want to recover? 0)No 1)Yes" << endl;
				cout << "Choose: ";
				cin >> re;
				if (re != 0)
					continue;

				win = this->game_.update_game_status(make_pair(chs-'A', direct));
			}

			else {	//AI's turn (A)
				int dice;
				cout << endl << "Set the dice(1~6): ";
				cin >> dice;
				if (!(dice>=1 && dice<=6))
					continue;

				win = this->game_.update_game_status(ewnAI.auto_play(this->game_, dice-1));
			}

			this->game_.print_board();
			//update: 0(game continues), 1(A wins), 2(B wins)
			if (win!=0) {
				//this->game_.print_status();
				cout << "====================================" << endl;
				if (win==2) cout << "The opponent is the winner!!!" << endl;
				else cout << "Our AI is the winner!!!" << endl;
				cout << "====================================" << endl;
				return;
			}
			this->game_.switch_player();
		}

	}
}
