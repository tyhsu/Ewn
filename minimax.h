#ifndef MINIMAX_H
#define MINIMAX_H
#include <iostream>
#include <cstdio>
#include <utility>
#include <map>
#include "game.h"
using namespace std;

// coordinate of the board (x, y)
typedef pair<int, int> Pos;
// Heuristic map: Pos -> heuristic value
typedef map<Pos, int> Heuristic_map;

class Minimax
{
	public:
		bool ai_side;
		char ai_symbol;
		Heuristic_map feature_map_A;
		Heuristic_map feature_map_B;

		Minimax();
		~Minimax() {}
		Movement AI_move(Game cur_game, int dice);
		int minimax(Game& cur_game, int height);
		int feature(Game& cur_game);
		int simulate(int height);
};

#endif
