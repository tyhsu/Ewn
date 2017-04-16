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
protected:
	int HEIGHT;
	int SIMU_TIMES;

public:
	bool ai_side;
	char ai_symbol;
	Heuristic_map feature_map_A;
	Heuristic_map feature_map_B;
	int alg_mode;

	Minimax();
	~Minimax() {}
	void set_height(const int& height_);
	void set_simu_times(const int& simu_times_);
	Movement AI_move(Game& cur_game, int dice);
	virtual int minimax(Game& cur_game, int height);
	int evaluate_feature(Game& cur_game);
	virtual int evaluate_simulation(const Game& cur_game);
	int simulation(Game simu_game);
};

#endif