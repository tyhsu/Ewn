#include <iostream>
#include <cstdio>
#include <algorithm>
#include "game.h"
#include "minimax.h"
#include <time.h>
#include <string>
const int HEIGHT = 4;
const float chance_weight = 1/6;

Minimax::Minimax() {
	// pos: a <int,int> , <x,y> coord;
	for (int i = 0; i < 5; i++) {
		for (int j = 0;j < 5; j++) {
			// insert to map => hvA;
			// get the smaller of (x,y)
			int k = (i > j ? j : i);
			// calculate the feature value
			k = (k+1) * (k+1) * 3;
			Pos chess_pos;
			chess_pos.first = i;
			chess_pos.second = j;
			hvA_.insert(make_pair(chess_pos, k));

			// insert to map => hvB;
			// get the bigger of (x,y)
			k = (i > j ? i : j);
			// calculate the feature value
			k = (5-k) * (5-k) * 3;
			chess_pos.first = i;
			chess_pos.second = j;
			hvB_.insert(make_pair(chess_pos, k));
		}
	}
}

Movement Minimax::autoPlay(Game currentGame, int dice)
{
    cerr << "calculating..." ;
    cerr.flush();
	clock_t timeInit = clock();
	aiTurn_ = currentGame.getTurn();
	aiSymbol_ = aiTurn_? 'A': '1';

	Movement answer;	//the best move will return
	int bestValue = -1e9, childValue;
	int win = aiTurn_? 2: 1;
	int nextMoveCnt = currentGame.availableMove(dice);

	for (int i=0; i<nextMoveCnt; i++) {
		int chs = currentGame.getMovableChs(i).symbol - aiSymbol_;
		//cout << "select: " << chs+1 << endl;
		for (int direct = 0; direct < 3; direct++) {
			Movement mvmt(chs, direct);
			if (currentGame.isLegalMove(mvmt)) {
				Game nextStep = currentGame;
				int end = nextStep.update(mvmt);
				// check if the game ends
				if (end == win)
					childValue = feature(nextStep);
				else if (end != 0)	// lose the game
					continue;
				else {
					nextStep.switchPlayer();
					childValue = minimax(nextStep, HEIGHT);
				}
				//cout << endl << "direction " << direct << ": " << childValue << endl;
				// update the best value and the best movement
				if (childValue >= bestValue) {
					bestValue = childValue;
					answer = mvmt;
				}
			}
		}
	}
	double timeCost = (double)(clock() - timeInit) / CLOCKS_PER_SEC;
	printf("\ntime cost: %.2fs\n", timeCost);
	return answer;
}

int Minimax::minimax(Game& currentGame, int height)
{
	// check if end;
	if (height == 0) {
		// Game nextStep = currentGame;
		currentGame.switchPlayer();
		return feature(currentGame);
	}
	// cout << "========== a minimax =========== height:" << height << endl;
	int bestValue;
	int diceArray[6];
	int lookahead = 0;
	string space = "";
	for (int i=HEIGHT; i>height; i--) space += " ";

	int myTurn, myWin;
	char mySymbol;
	if (currentGame.getTurn() == aiTurn_) {
		// my turn			=> the largest, the better
		myTurn = 1;
		bestValue = -1e9;
		myWin = aiTurn_ ? 2 : 1;
		mySymbol = aiSymbol_;
		for (int i=0; i<6; i++)
			diceArray[i] = -1e9;
	}
	else {
		// opponent's turn	=> the smallest, the better
		myTurn = 0;
		bestValue = 1e9;
		myWin = aiTurn_ ? 1 : 2;
		mySymbol = aiTurn_? '1' : 'A';
		for (int i=0; i<6; i++)
			diceArray[i] = 1e9;
	}

	// ai's turn, find the max
	// for #chess and direction
	for (int dice = 0; dice < 6; dice++) {
		Chess currChess = currentGame.getCurrPlayer(dice);
		if (currChess.exist) {
			int chs = currChess.symbol - mySymbol;
			for (int direct = 0; direct < 3; direct++) {
				Movement mvmt(chs, direct);
				if (currentGame.isLegalMove(mvmt)) {
					Game nextStep = currentGame;
					int end = nextStep.update(mvmt);
					// check if the game ends
					if (end == myWin) return feature(nextStep);
					else if (end != 0) continue;
					nextStep.switchPlayer();

					// return child value.
					int childValue = minimax(nextStep, height-1);
					if (myTurn) {
						bestValue = max(bestValue, childValue);
					}
					else {
						bestValue = min(bestValue, childValue);
					}
				}
				else {
					// cout << space << "nope, the chs can`t be move." << endl;
				}
			}
			diceArray[dice] = bestValue;

			// if there is lookahead, compare those value of the non-existing
			// if current is bigger than the front value.
			if (lookahead) {
				int offset;
				if ((myTurn && bestValue>diceArray[dice-1]) || (!myTurn && bestValue<diceArray[dice-1])) {
					for (int j = 1; j <= lookahead; j++) {
						offset = dice - j;
						diceArray[offset] = bestValue;
					}
				}
				lookahead = 0;
			}
		}
		else {
			//cout << space << "[**Mine** not exist] " << dice << endl;
			// this chs can`t move, use other`s value
			lookahead++;
			if (dice - lookahead < 0) {
				// there's no chess exists before the selected one
				continue;
			}
			else {
				diceArray[dice] = diceArray[dice - lookahead];
			}
		}
	}

	// return the sum of the best values in the diceArray (chance node)
	bestValue = 0;
	for(int i = 0; i < 6; i++) {
		// if (height == HEIGHT) cout << i+1 << ":" << diceArray[i] << endl;
		bestValue += diceArray[i];
	}

	return bestValue;
}

int Minimax::feature(Game& currentGame)
{
	int valMy = 0, valOpp = 0;
	int dice_count = 0;
	//cout << "hv0--------------------------------" << endl;
	bool turn = aiTurn_;
	Hmap *hv = aiTurn_? &hvB_: &hvA_;
	int *val = &valMy;

	for (int side=0; side<2; side++) {
		for (int dice = 0; dice < 6; dice++) {
			// calculate the score of our side
			Chess chs = currentGame.getPlayer(turn, dice);
			if (chs.exist) {
				// [exist] chess calc function.
				Pos temp;
				temp.first = chs.x;
				temp.second = chs.y;
				//cout << "dice#" << dice+1 << " : (" << temp.first << ", " << temp.second << ") score:" << (*hv)[temp] << endl;
				*val += (*hv)[temp];
				dice_count++;
			}
			else {
				// [non-exist] chess calc function.
				// spVal: find closest, small chess num`s value.
				// bpVal: find closest, big chess num`s value.
				int spVal = 0, bpVal = 0;
				// get small part of chess list.
				for (int sp = dice; sp > 0; sp--) {
					Chess ctmp = currentGame.getPlayer(turn, sp);
					if (ctmp.exist) {
						Pos temp;
						temp.first = ctmp.x;
						temp.second = ctmp.y;
						spVal = (*hv)[temp];
						break;
					}
				}

				// get big part of chess list.
				for (int bp = dice; bp < 6; bp++) {
					Chess ctmp = currentGame.getPlayer(turn, bp);
					if (ctmp.exist) {
						Pos temp;
						temp.first = ctmp.x;
						temp.second = ctmp.y;
						bpVal = (*hv)[temp];
						break;
					}
				}

				// select one.
				spVal = (spVal > bpVal ? spVal : bpVal);
				//cout << "dice#" << dice+1 << ", score:" << spVal << endl;
				*val += spVal;
			}
		}

		// calculate the score of opponent's side
		turn = !aiTurn_;
		hv = aiTurn_? &hvA_: &hvB_;
		val = &valOpp;
	}

	//cout << "hv " << valMy << " - " << valOpp << "---------------------------" << endl << endl;
	return valMy - (int)(valOpp*1.2);
}

void Minimax::createAvailableT()
{
	for(int exist=0; exist<(1<<6); exist++) { // each exist status
		for(int dice=0; dice<6; dice++) {
			int availableFormer = -1, availableLatter = -1;
			int tmp = exist;
			int bin[6];
			int f = exist, l = exist;
			for(int i=dice; i>=0; i--) {
				if((exist%(1<<(i+1)))/(1<<i) == true) {
					availableFormer = i;
					break;
				}
			}
			for(int i=dice; i<6; i++) {
				if((exist>>i)%2 == true) {
					availableLatter = i;
					break;
				}
			}
			availableFormer = (availableFormer == -1) ? availableLatter : availableFormer;
			availableLatter = (availableLatter == -1) ? availableFormer : availableLatter;
			pair<int, int> fir(dice, exist), sec(availableFormer, availableLatter);
			am_.insert(make_pair(fir, sec));
		}
	}
}
