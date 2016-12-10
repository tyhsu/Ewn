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
		this->ai_ = 0;
}

Movement EwnAI::auto_play(Game currentGame, int dice)
{
	//if (this->ai_ == 0)
		return this->minimax_.auto_play(currentGame, dice);
}
