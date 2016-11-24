#ifndef EWNAI_H
#define EWNAI_H
#include "minimax.h"

class EwnAI
{
private:
	int ai_;
	Minimax minimax_;	//ai = 0
public:
	EwnAI();
	Movement autoPlay(Game currentGame, int dice);
};

#endif
