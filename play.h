#ifndef PLAY_H
#define PLAY_H

#include <iostream>
#include "game.h"
#include "ewnAi.h"
#include "file.h"
#include <string>
using namespace std;

class Play
{
private:
	Game game;

public:
	Ewnfile ewn_file;
	//One player plays the game (the integration)
	Movement player_move(const int& dice);

	/*-----------the Modes-----------*/
	//Two players fight!
	void two_players_mode();
	//One player fight with AI
	void player_AI_mode();
	//Two AIs fight
	void two_AIs_mode();
	//One player fight with AI in the contest
	void contest_AI_mode();
	//Compare two AIs with several games
	void compare_AI_mode();
};

#endif
