#ifndef EWNAI_H
#define EWNAI_H
#include "minimax.h"

class EwnAI
{
private:
	int ai_mode;
	Minimax minimax;	//ai = 0
public:
	EwnAI();
	Movement AI_move(Game cur_game, int dice);
};

#endif
