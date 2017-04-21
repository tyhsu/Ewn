#ifndef EWNAI_H
#define EWNAI_H
#include "minimax.h"
#include "minimax1.h"
#include "minimax2.h"
#include "minimax3.h"
#include "minimax4.h"
#include "mcts.h"
#include "randomAi.h"

class EwnAI
{
private:
	int ai_mode;
	Minimax minimax;	//ai = 0
	Minimax1 minimax1;	//ai = 1
	Minimax2 minimax2;	//ai = 2
	Minimax3 minimax3;	//ai = 3
	Minimax4 minimax4;	//ai = 4
	MCTS mcts;
	RandomAi randomAi;
public:
	EwnAI();
	EwnAI(int mode);
	Movement AI_move(Game cur_game, int dice);

	void set_height(const int& height_);
	void set_simu_times(const int& simu_times_);
	void set_max_iterations(const int& max_iterations_);
};

#endif
