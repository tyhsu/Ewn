#ifndef MINIMAX_H
#define MINIMAX_H
#include <iostream>
#include <cstdio>
#include <utility>
#include <map>
#include "game.h"
using namespace std;

typedef pair<int, int> Pos;
// Heuristic map
typedef map<Pos, int> Hmap;
// Available move map


class Minimax
{
	public:
		//the result after the simulation
		bool aiTurn_;
		char aiSymbol_;
		Chess chosenChs_;
		int chosenDirct_;
		Hmap hvA_;
		Hmap hvB_;
		Amap am_;
		// OurOppScoreMap sm_;
		Minimax();
		~Minimax() {}
		Movement autoPlay(Game currentGame, int dice);
		int minimax(Game& originGame, int h);
		int feature(Game& currentGame);
		// int feature(char board[5][5]);
		int simulate(int height);
		int chooseDirection();
};

#endif
