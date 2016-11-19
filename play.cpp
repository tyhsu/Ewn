#include <iostream>
#include "game.h"
//#include "ewnAi.h"
#include "play.h"
using namespace std;

Movement Play::playerPlay(const int& dice)
{
	int nextMoveCnt = this->game_.availableMove(dice);
	int chs, direct;

	cout << "Next chessman able to move: ";
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
	Movement mvmt(chs, direct);
	//If the movement is illegal, run the loop
	while (this->game_.isLegalMove(mvmt) == false) {
		cout << "Illegal move!!" << endl << endl;
		cout << "Next chessman able to move: ";
		for (int i=0; i<nextMoveCnt; i++)
			cout << i << ")" << this->game_.getMovableChs(i).symbol << " ";
		cout << endl;
		cout << "Choose: ";
		cin >> chs;
		chs = (this->game_.getTurn() == false) ? this->game_.getMovableChs(chs).symbol - '0' : this->game_.getMovableChs(chs).symbol - 'A';
		if (!this->game_.getTurn())
			cout << "Direction: 0)Right 1)Down 2)Right-down" << endl;
		else
			cout << "Direction: 0)Left 1)Up 2)Left-up" << endl;
		cout << "Choose: ";
		cin >> direct;
		mvmt.first = chs;
		mvmt.second = direct;
	}
	return mvmt;
}

void Play::twoPlayers()
{
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
	while (1) {
		//ask for the next chessman to move

		//update, 0(game continues), 1(A wins), 2(B wins)
		int dice = this->game_.rollTheDice();
		int win;
		if (this->game_.getTurn() == true) {
			this->recoverBuffer_ = this->game;
			win = this->game_.update(playerPlay(dice));
			char cmd;
			cout<<"press z to recover, or any other keys to continue : ";
		}
			
		else
			win = this->game_.update(ewnAI.autoPlay(this->game_, dice));
		this->game_.printBoard();
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
	EwnAI ewnAI;
	while (1) {
		//ask for the next chessman to move

		//update, 0(game continues), 1(A wins), 2(B wins)
		int dice = this->game_.rollTheDice();
		int win = this->game_.update(ewnAI.autoPlay(this->game_, dice));
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

void Play::contestAI()
{
	int firstPlay;
	cout << "Whose AI goes first? 0)Ours 1)Opponent's" << endl;
	cin >> firstPlay;

	EwnAI ewnAI;
	if (firstPlay) {	//A: Opponent's, B: Ours
		while (1) {
			int win;
			if (this->game_.getTurn() == true) {	//The opponent's turn
				this->recoverBuffer_ = this->game;

				int chs, direct;
				cout << "Choose the chessman(1~6): ";
				cin >> chs;
				cout << "Direction: 0)Right 1)Down 2)Right-down" << endl;
				cout << "Choose: ";
				cin >> direct;

				win = this->game_.update(make_pair(chs, direct));
				char cmd;
				cout<<"press z to recover, or any other keys to continue : ";
			}
				
			else {	//AI's turn
				int dice = this->game_.rollTheDice();
				win = this->game_.update(ewnAI.autoPlay(this->game_, dice));
			}
			
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
	else {				//A: Ours, B: Opponent's
		while (1) {
			int win;
			if (this->game_.getTurn() == false) {	//The opponent's turn
				this->recoverBuffer_ = this->game;

				char chs;
				int direct;
				cout << "Choose the chessman(A~F): ";
				cin >> chs;
				cout << "Direction: 0)Left 1)Up 2)Left-up" << endl;
				cout << "Choose: ";
				cin >> direct;

				win = this->game_.update(make_pair(chs-'A', direct));
				char cmd;
				cout<<"press z to recover, or any other keys to continue : ";
			}
				
			else {	//AI's turn
				int dice = this->game_.rollTheDice();
				win = this->game_.update(ewnAI.autoPlay(this->game_, dice));
			}
			
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
}

void Play::recover() {
	this->game_ = this->recoverBuffer_;
}
