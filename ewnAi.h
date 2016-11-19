#ifndef EWNAI_H
#define EWNAI_H
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
// <dice, exist_status> maps to <available_chess, available_chess>
typedef map< pair<int, int>, pair<int, int> > Amap;

class EwnAI
{
	public:
		//the result after the simulation
		bool aiTurn_;
		char aiSymbol_;
		Chess chosenChs_;
		int chosenDirct_;
		Hmap hv_;
		Amap am_;
		
		EwnAI();
		~EwnAI() {}
		Movement autoPlay(Game currentGame, int dice);
		int minimax(Game& originGame, int h);
		int evaluate(Game& currentGame);
		int feature(char board[5][5]);
		int simulate(int height);
		int chooseDirection();
		//making table
		void createHeuristicT();
		void createAvailableT();
};

#endif
