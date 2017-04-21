#include <iostream>
#include "ewnAi.h"
using namespace std;

EwnAI::EwnAI()
{
	int cmd;
	cout << "Please choose an AI:" << endl;
	cout << "0)Minimax evaluating with feature" << endl;
	cout << "1)Minimax evaluating with simulation 1" << endl;
	cout << "2)Minimax evaluating with simulation 2" << endl;
	cout << "3)Minimax evaluating with simulation 3" << endl;
	cout << "4)Minimax evaluating with simulation 4" << endl;
	cout << "5)MCTS" << endl;
	cout << "6)Random" << endl;
	cout << "Choose: ";
	cin >> cmd;
	if (cmd >= 0 && cmd <=6) this->ai_mode = cmd;
	else this->ai_mode = 0;
}

EwnAI::EwnAI(int mode)
{
	this->ai_mode = mode;
}

Movement EwnAI::AI_move(Game cur_game, int dice)
{
	switch (this->ai_mode) {
		case 0:
			return this->minimax.AI_move(cur_game, dice);
		case 1:
			return this->minimax1.AI_move(cur_game, dice);
		case 2:
			return this->minimax2.AI_move(cur_game, dice);
		case 3:
			return this->minimax3.AI_move(cur_game, dice);
		case 4:
			return this->minimax4.AI_move(cur_game, dice);
		case 5:
			return this->mcts.AI_move(cur_game, dice);
		case 6:
			return this->randomAi.AI_move(cur_game, dice);
		default:
			return this->minimax.AI_move(cur_game, dice);
	}
}

void EwnAI::set_height(const int& height_)
{
	// only minimax can call this function (mode = 0 ~ 4)
	switch (this->ai_mode) {
		case 0:
			return this->minimax.set_height(height_);
		case 1:
			return this->minimax1.set_height(height_);
		case 2:
			return this->minimax2.set_height(height_);
		case 3:
			return this->minimax3.set_height(height_);
		case 4:
			return this->minimax4.set_height(height_);
		default:
			cout << "Error: Only minimax can set height currently." << endl;
			return;
	}
}

void EwnAI::set_simu_times(const int& simu_times_)
{
	// only minimax can call this function (mode = 0 ~ 4)
	switch (this->ai_mode) {
		case 0:
			return this->minimax.set_simu_times(simu_times_);
		case 1:
			return this->minimax1.set_simu_times(simu_times_);
		case 2:
			return this->minimax2.set_simu_times(simu_times_);
		case 3:
			return this->minimax3.set_simu_times(simu_times_);
		case 4:
			return this->minimax4.set_simu_times(simu_times_);
		default:
			cout << "Error: Only minimax can set simu_times currently." << endl;
			return;
	}
}

