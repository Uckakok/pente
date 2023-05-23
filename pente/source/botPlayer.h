#pragma once

#include"penteBoard.h"
#include"declarations.h"
#include"vector"
#include"array"

using namespace std;

typedef struct {
	int x;
	int y;
	int num;
}coordinatesAndNum;

typedef struct {
	int startx;
	int starty;
	int endx;
	int endy;
	int dirIndex;
	int length;
} chain;

namespace botPlayer {
	vector<chain> analyzeForChains(penteBoard *boardToAnalyze, bool whitePlayer, int mineLineLength);
	vector<coordinates> analyzeForTakings(penteBoard *boardToAnalyze);
	void evaluateBoard(penteBoard *boardToAnalyze);
	vector<coordinates> analyzeForFourWithGap(penteBoard *boardToAnalyze, bool whitePlayer);
	array<vector<coordinates>, 4> blockingChains(penteBoard *boardToAnalyze, vector<chain> chains);
};
