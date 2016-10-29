#include <iostream>
#include <cstdio>
#include <utility>
#include <map>
#include "game.h"
using namespace std;

typedef pair<int, int> Pos;
typedef map<Pos , int> Hmap;

class EwnAI
{
	public:
		//the result after the simulation
		const bool aiTurn_;
		Chess chosenChs_;
		int chosenDirct_;
		Hmap hv_;

		EwnAI();
		~EwnAI();
		int beforeMinimax(Game currentGame, int dice)
		int minimax(Game& originGame, int h);
		int evaluate(Game& currentGame);
		int feature(char board[5][5]);
		int simulate(int height);

		int chooseDirection();
};
