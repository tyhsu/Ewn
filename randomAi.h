#ifndef RANDOMAI_H
#define RANDOMAI_H

#include <iostream>
#include <cstdio>
#include <utility>
#include <map>
#include "game.h"
using namespace std;

// coordinate of the board (x, y)
typedef pair<int, int> Pos;

class RandomAi
{
public:
	bool ai_side;
	char ai_symbol;
	int alg_mode;

	RandomAi();
	~RandomAi() {}
	Movement AI_move(Game& cur_game, int dice);
	int simulation(Game simu_game);
};

#endif