#include <iostream>
#include "game.h"
//#include "ewnAi.h"
#include "play.h"
using namespace std;

Movement Play::playerPlay(const int& dice)
{
	int nextMoveCnt = this->game_.availiableMove(dice);
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
	return mvmt ;
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
/*
void Play::playerAI()
{
	EwnAI ewnAI;
	while (1) {
		//ask for the next chessman to move

		//update, 0(game continues), 1(A wins), 2(B wins)
		int dice = this->game_.rollTheDice();
		int win =(this->game_.getTurn() == true )? this->game_.update(ewnAI.autoPlay(this->game_, dice)) :this->game_.update(playerPlay(this->game_.rollTheDice()));
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
*/
