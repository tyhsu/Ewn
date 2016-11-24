#include <iostream>
#include "play.h"
using namespace std;

Movement Play::playerPlay(const int& dice)
{
	int nextMoveCnt = this->game_.availableMove(dice);
	int chs, direct;
	Movement mvmt;

	do {
		cout << endl << "Next chessman able to move: ";
		for (int i=0; i<nextMoveCnt; i++)
			cout << i << ")" << this->game_.getMovableChs(i).symbol << " ";
		cout << endl;
		cout << "Choose: ";
		cin >> chs;

		if (!this->game_.getTurn())
			cout << "Direction: 0)Right 1)Down 2)Right-down" << endl;
		else
			cout << "Direction: 0)Left 1)Up 2)Left-up" << endl;
		cout << "Choose: ";
		cin >> direct;

		// get index
		chs = (this->game_.getTurn() == false) ? this->game_.getMovableChs(chs).symbol - '1' : this->game_.getMovableChs(chs).symbol - 'A';
		mvmt.first = chs;
		mvmt.second = direct;
		//If the movement is illegal, run the loop
	} while (!this->game_.isLegalMove(mvmt));

	return mvmt;
}

// void Play::recover() {
// 	this->game_ = this->recoverBuffer_;
// }

void Play::twoPlayers()
{
	game_.printBoard();
	while (1) {
		//update: 0(game continues), 1(A wins), 2(B wins)
		int win = this->game_.update(playerPlay(this->game_.rollTheDice()));
		this->game_.printBoard();
		if (win!=0) {
			cout << "====================================" << endl;
			if (win==1) cout << "A is the winner!!!" << endl;
			else cout << "B is the winner!!!" << endl;
			cout << "====================================" << endl;
			return;
		}
		this->game_.switchPlayer();
	}
}

void Play::playerAI()
{
	EwnAI ewnAI;
	game_.printBoard();
	while (1) {
		int dice = this->game_.rollTheDice();
		int win;
		if (this->game_.getTurn() == true) {
			win = this->game_.update(playerPlay(dice));
		}
		else
			win = this->game_.update(ewnAI.autoPlay(this->game_, dice));

		this->game_.printBoard();
		//update: 0(game continues), 1(A wins), 2(B wins)
		if (win!=0) {
			cout << "====================================" << endl;
			if (win==1) cout << "AI is the winner!!!" << endl;
			else cout << "B is the winner!!!" << endl;
			cout << "====================================" << endl;
			return;
		}
		this->game_.switchPlayer();
	}
}

void Play::twoAIs()
{
	cout << "==========The first AI (A)==========" << endl;
	EwnAI ewnAI1;
	cout << "==========The second AI (B)==========" << endl;
	EwnAI ewnAI2;
	game_.printBoard();
	while (1) {
		int dice = this->game_.rollTheDice();
		int win;
		if (!game_.getTurn())	// the first AI (A)
			win = this->game_.update(ewnAI1.autoPlay(this->game_, dice));
		else					// the second AI (B)
			win = this->game_.update(ewnAI2.autoPlay(this->game_, dice));
		this->game_.printBoard();
		//update: 0(game continues), 1(A wins), 2(B wins)
		if (win!=0) {
			cout << "====================================" << endl;
			if (win==1) cout << "A is the winner!!!" << endl;
			else cout << "B is the winner!!!" << endl;
			cout << "====================================" << endl;
			return;
		}
		this->game_.switchPlayer();
	}
}

void Play::contestAI()
{
	int firstPlay;
	cout << "Whose AI goes first?" << endl;
	cout << "=>Who is A? 0)Ours 1)Opponent's" << endl;
	cout << "Choose: ";
	cin >> firstPlay;

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
		game_.setBoard(board, turn);
	}
	cout << endl;
	game_.checkStatus();
	game_.printBoard();
	
	EwnAI ewnAI;
	if (firstPlay) {	//A: Opponent's, B: Ours
		while (1) {
			int win;
			if (!this->game_.getTurn()) {	//The opponent's turn (A)
				int chs, direct;
				cout << endl << "Choose the chessman(1~6): ";
				cin >> chs;
				if (!(chs>=1 && chs<=6))
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
				
				win = this->game_.update(make_pair(chs-1, direct));
			}

			else {	//AI's turn (B)
				int dice;
				cout << endl << "Set the dice(1~6): ";
				cin >> dice;
				if (!(dice>=1 && dice<=6))
					continue;

				win = this->game_.update(ewnAI.autoPlay(this->game_, dice-1));
			}
			
			this->game_.printBoard();
			//update: 0(game continues), 1(A wins), 2(B wins)
			if (win!=0) {
				this->game_.checkStatus();
				cout << "====================================" << endl;
				if (win==1) cout << "The opponent is the winner!!!" << endl;
				else cout << "Our AI is the winner!!!" << endl;
				cout << "====================================" << endl;
				return;
			}
			this->game_.switchPlayer();
		}
	}
	else {				//A: Ours, B: Opponent's
		while (1) {
			int win;
			if (this->game_.getTurn()) {	//The opponent's turn (B)
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
				
				win = this->game_.update(make_pair(chs-'A', direct));
			}
				
			else {	//AI's turn (A)
				int dice;
				cout << endl << "Set the dice(1~6): ";
				cin >> dice;
				if (!(dice>=1 && dice<=6))
					continue;

				win = this->game_.update(ewnAI.autoPlay(this->game_, dice-1));
			}
			
			this->game_.printBoard();
			//update: 0(game continues), 1(A wins), 2(B wins)
			if (win!=0) {
				this->game_.checkStatus();
				cout << "====================================" << endl;
				if (win==2) cout << "The opponent is the winner!!!" << endl;
				else cout << "Our AI is the winner!!!" << endl;
				cout << "====================================" << endl;
				return;
			}
			this->game_.switchPlayer();
		}

	}
}
