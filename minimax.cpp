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
	// insert to map => hvA;
	//cout << "create AI." << endl;
	for (int i = 0; i < 5; i++) {
		for (int j = 0;j < 5; j++) {

			// get the smaller of (x,y)
			int k = (i > j ? j : i);
			k = k * k;
			Pos chess_pos;
			chess_pos.first = i;
			chess_pos.second = j;
			//printf("%2d", k);
			hvA_.insert(make_pair(chess_pos, k));
		}
		//cout << endl;
	}
	//cout << endl;
	// insert to map => hvB;
	for (int i = 0; i < 5; i++) {
		for (int j = 0;j < 5; j++) {

			// get the bigger of (x,y)
			int k = (i > j ? i : j);
			k = (4-k) * (4-k);
			Pos chess_pos;
			chess_pos.first = i;
			chess_pos.second = j;
			//printf("%2d", k);
			hvB_.insert(make_pair(chess_pos, k));
		}
		//cout << endl;
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
	int bestValue = -1e9;
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
				if (end == win) return mvmt;
				else if (end != 0) continue;	// lose the game
				nextStep.switchPlayer();
				
				int childValue = minimax(nextStep, HEIGHT);
				if(childValue > bestValue) {	// update
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
	// cout << "========== a minimax ===========" << endl;
	int bestValue;
	int diceArray[6] = {0};
	int lookahead = 0;
	string space = "";
	for (int i=HEIGHT; i>height; i--) space += " ";


	if (currentGame.getTurn() == aiTurn_) {
		// ai's turn, find the max
		bestValue = -1e9;
		int win = aiTurn_? 2: 1;
		// for #chess and direction
		for (int dice = 0; dice < 6; dice++) {
			Chess currChess = currentGame.getCurrPlayer(dice);
			if (currChess.exist) {
				int chs = currChess.symbol - aiSymbol_;
				for (int direct = 0; direct < 3; direct++) {
					Movement mvmt(chs, direct);
					if (currentGame.isLegalMove(mvmt)) {
						Game nextStep = currentGame;
						int end = nextStep.update(mvmt);
						// check if the game ends
						if (end == win) return 1000;
						else if (end != 0) continue;
						nextStep.switchPlayer();

						// return child value.
						int childValue = minimax(nextStep, height-1);
						bestValue = max(bestValue, childValue);
					} else {
						// cout << space << "nope, the chs can`t be move." << endl;
					}
				}
				diceArray[dice] = bestValue;

				// if there is lookahead, compare those value of the non-existing
				// if current is bigger than the front value.
				if (lookahead) {
					int offset;
					if (bestValue > diceArray[dice - 1]) {
						for (int j = 1; j <= lookahead; j++){
							offset = dice - j;
							diceArray[offset] = bestValue;
						}
					}
					lookahead = 0;
				}
			}
			else {
				// cout << space << "[**Mine** not exist] " << dice << endl;
				// this chs can`t move, use other`s value
				lookahead++;
				if (dice - lookahead < 0) {
					continue;
				}
				else {
					diceArray[dice] = diceArray[dice - lookahead];
				}
			}
		}

    }
	else {
		// the opponent's turn, find the min
		bestValue = 1e9;

		int lose = aiTurn_? 1: 2;
		// for #chess and direction
		for (int dice = 0; dice < 6; dice++) {
			Chess currChess = currentGame.getCurrPlayer(dice);
			if (currChess.exist) {
				int oppntSymbol = aiTurn_? '1': 'A';
				int chs = currChess.symbol - oppntSymbol;

				for (int direct = 0; direct < 3; direct++) {
					Movement mvmt(chs, direct);
					if (currentGame.isLegalMove(mvmt)) {
						Game nextStep = currentGame;
						int end = nextStep.update(mvmt);
						// check if the game ends
						if (end == lose) return -1000;
						else if (end != 0) continue;
						nextStep.switchPlayer();

						// return child value.
						int childValue = minimax(nextStep, height-1);
						bestValue = min(bestValue, childValue);
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
					if (bestValue < diceArray[dice - 1]) {
						for (int j = 1; j <= lookahead; j++){
							offset = dice - j;
							diceArray[offset] = bestValue;
						}
					}
					lookahead = 0;
				}
			}
			else {
				// this chs can`t move, use other`s value
				lookahead++;
				if (dice - lookahead < 0) {
					continue;
				}
				else {
					diceArray[dice] = diceArray[dice - lookahead];
				}
			}
		}
	}

	bestValue = 0;
	for(int i = 0; i < 6; i++)
		bestValue += diceArray[i];
	
	return bestValue;
}

int Minimax::feature(Game& currentGame)
{
	int valMy = 0, valOpp = 0;
	int dice_count = 0;
	//cout << "hv0--------------------------------" << endl;
	Hmap hvMy, hvOpp;
	if (!currentGame.getTurn()) {
		hvMy = hvA_;
		hvOpp = hvB_;
	}
	else {
		hvMy = hvB_;
		hvOpp = hvA_;
	}

	for (int dice = 0; dice < 6; dice++) {
		// calculate the score of our side
		Chess chs = currentGame.getCurrPlayer(dice);
		if (chs.exist) {
			// [exist] chess calc function.
			Pos temp;
			temp.first = chs.x;
			temp.second = chs.y;
			//cout << "dice#" << dice+1 << " : (" << temp.first << ", " << temp.second << ") score:" << hvMy[temp] << endl;
			valMy += hvMy[temp];
			dice_count++;
		} 
		else {	
			// [non-exist] chess calc function.
			// spVal: find closest, small chess num`s value.
			// bpVal: find closest, big chess num`s value.
			int spVal = 0, bpVal = 0;
			// get small part of chess list.
			for (int sp = dice; sp > 0; sp--) {
				Chess ctmp = currentGame.getCurrPlayer(sp);
				if (ctmp.exist) {
					Pos temp;
					temp.first = ctmp.x;
					temp.second = ctmp.y;
					spVal = hvMy[temp];
					break;
				}
			}

			// get big part of chess list.
			for (int bp = dice; bp < 6; bp++) {
				Chess ctmp = currentGame.getCurrPlayer(bp);
				if (ctmp.exist) {
					Pos temp;
					temp.first = ctmp.x;
					temp.second = ctmp.y;
					bpVal = hvMy[temp];
					break;	
				}
			}

			// select one.
			spVal = (spVal > bpVal ? spVal : bpVal);
			//cout << "dice#" << dice+1 << ", score:" << spVal << endl;
			valMy += spVal;
		}

		// calculate the score of opponent's side
		chs = currentGame.getOppPlayer(dice);
		if (chs.exist) {
			// [exist] chess calc function.
			Pos temp;
			temp.first = chs.x;
			temp.second = chs.y;
			//cout << "Opp dice#" << dice+1 << " : (" << temp.first << ", " << temp.second << ") score:" << hvOpp[temp] << endl;
			valOpp += hvOpp[temp];
			dice_count++;
		} 
		else {	
			// [non-exist] chess calc function.
			// spVal: find closest, small chess num`s value.
			// bpVal: find closest, big chess num`s value.
			int spVal = 0, bpVal = 0;
			// get small part of chess list.
			for (int sp = dice; sp > 0; sp--) {
				Chess ctmp = currentGame.getOppPlayer(sp);
				if (ctmp.exist) {
					Pos temp;
					temp.first = ctmp.x;
					temp.second = ctmp.y;
					spVal = hvOpp[temp];
					break;
				}
			}

			// get big part of chess list.
			for (int bp = dice; bp < 6; bp++) {
				Chess ctmp = currentGame.getOppPlayer(bp);
				if (ctmp.exist) {
					Pos temp;
					temp.first = ctmp.x;
					temp.second = ctmp.y;
					bpVal = hvOpp[temp];
					break;	
				}
			}

			// select one.
			spVal = (spVal > bpVal ? spVal : bpVal);
			//cout << "Opp dice#" << dice+1 << ", score:" << spVal << endl;
			valOpp += spVal;
		}
	}
	//cout << "hv " << valMy << " - " << valOpp << "---------------------------" << endl << endl;
	return valMy - valOpp;
}

void Minimax::createHeuristicT()
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0;j < 5; j++) {

			// get the smaller of (x,y)
			int k = (i > j ? j : i);
			k = k * k;
			Pos chess_pos;
			chess_pos.first = i;
			chess_pos.second = j;
			//printf("%2d", k);
			hvA_.insert(make_pair(chess_pos, k));
		}
		//cout << endl;
	}
	// insert to map => hv;
	for (int i = 0; i < 5; i++) {
		for (int j = 0;j < 5; j++) {

			// get the bigger of (x,y)
			int k = (i > j ? i : j);
			k = (4-k) * (4-k);
			Pos chess_pos;
			chess_pos.first = i;
			chess_pos.second = j;
			//printf("%2d", k);
			hvB_.insert(make_pair(chess_pos, k));
		}
		//cout << endl;
	}	
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
