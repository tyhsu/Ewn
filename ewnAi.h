#include <iostream>
#include <cstdio>
using namespace std;

class EwnAI
{
public:
	EwnAI();
	~EwnAI();
	int feature(char board[5][5]);
	int simulate(int height);
};