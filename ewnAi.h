#ifndef EWNAI_H
#define EWNAI_H
#include "minimax.h"
#include "minimax1.h"
#include "minimax2.h"
#include "minimax3.h"
#include "minimax4.h"
#include "mcts.h"
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
public:
	EwnAI();
	EwnAI(int mode);
	Movement AI_move(Game cur_game, int dice);
};

#endif
