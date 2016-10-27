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
  Chess chosenChs_;
  int chosenDirct_;
  Hmap hv;

  EwnAI();
  ~EwnAI();
  int feature(char board[5][5]);
  int simulate(int height);

  int minimax(Game& origin_game, int h);
  int evaluate(Game& current_game);
};
