#ifndef PLAY_H
#define PLAY_H

#include <iostream>
#include "game.h"
#include "ewnAi.h"
using namespace std;

class Play
{
private:
	Game game_;
	Game recoverBuffer_;

public:
	//One player plays the game (the integration)
	Movement playerPlay(const int& dice);
	//To recover one step
	void recover();

	/*-----------the Modes-----------*/
	//Two players fight!
	void twoPlayers();
	//One player fight with AI
	void playerAI();
	//Two AIs fight
	void twoAIs();
	//One player fight with AI in the contest
	void contestAI();
};

#endif
