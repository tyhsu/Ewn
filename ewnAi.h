#include <iostream>
#include <cstdio>
#include "game.h"
using namespace std;

class EwnAI
{
public:
	//the result after the simulation
	Chess chosenChs_;
	int chosenDirct_;

	EwnAI();
	~EwnAI();
	int feature(char board[5][5]);
	int simulate(int height);
};