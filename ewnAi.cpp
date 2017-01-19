#include <iostream>
#include "ewnAi.h"
using namespace std;

EwnAI::EwnAI()
{
	int cmd;
	cout << "Please choose an AI:" << endl;
	cout << "0)Minimax" << endl;
	cout << "Choose: ";
	cin >> cmd;
	if (cmd == 0)
		this->ai_mode = 0;
}
EwnAI::EwnAI(int _mode)
{
	this->ai_mode = _mode;
}
Movement EwnAI::AI_move(Game cur_game, int dice)
{
	//if (this->ai_mode == 0)
		return this->minimax.AI_move(cur_game, dice, this->ai_mode);
}
