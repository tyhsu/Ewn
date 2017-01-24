#ifndef MINIMAX_H
#define MINIMAX_H
#include <iostream>
#include <cstdio>
#include <utility>
#include <map>
#include <sys/time.h>
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
	int ai_mode;
    char ai_symbol;
    Heuristic_map feature_map_A;
    Heuristic_map feature_map_B;
    Minimax();
    ~Minimax() {}
    Movement AI_move(Game cur_game, int dice, int _mode = 0);
    int minimax(Game cur_game, int height);
    int evaluate_feature(Game& cur_game);
    int evaluate_simulation(const Game& cur_game);
    int evaluate_simulation_second_type_rand(const Game& cur_game);
    int evaluate_simulation_third_type_rand(const Game& cur_game);
    int simulation(Game simu_game);
    int simulation_second_type(Game simu_game);
    int simulation_third_type(Game simu_game);
};

#endif
