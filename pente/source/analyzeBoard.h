#pragma once

#include"penteBoard.h"
#include"declarations.h"
#include<vector>
#include<array>
#include <algorithm>
#include <map>

using namespace std;

struct coordinatesAndNum {
	int x;
	int y;
	int num;
	explicit operator coordinates() {
		return { x, y };
	}
};

typedef struct {
	int startx;
	int starty;
	int endx;
	int endy;
	int dirIndex;
	int length;
} chain;

namespace analyzeBoard {
	vector<chain> analyzeForChains(penteBoard *boardToAnalyze, bool whitePlayer, int lineLength);
	vector<coordinates> analyzeForTakings(penteBoard *boardToAnalyze, bool whitePlayer);
	vector<coordinates> analyzeForFourWithGap(penteBoard *boardToAnalyze, bool whitePlayer);
	array<vector<coordinates>, 5> blockingChains(penteBoard *boardToAnalyze, vector<chain> chains);
	array<vector<coordinates>, 8> findFieldsAdjacentToPieces(penteBoard *boardToAnalyze, bool whitePlayer);
	vector<coordinates> analyzeForThreeWithGap(penteBoard *boardToAnalyze, bool whitePlayer);
};
