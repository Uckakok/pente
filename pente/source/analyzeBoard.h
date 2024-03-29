#pragma once

#include"penteBoard.h"
#include"declarations.h"
#include<vector>
#include<array>

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
	vector<chain> analyzeForChains(penteBoard* boardToAnalyze, bool whitePlayer, int lineLength);
	vector<coordinates> analyzeForFourWithGap(penteBoard* boardToAnalyze, bool whitePlayer);
	array<vector<coordinates>, 5> blockingChains(penteBoard* boardToAnalyze, vector<chain> chains);
	array<vector<coordinates>, 8> findFieldsAdjacentToPieces(penteBoard* boardToAnalyze, bool whitePlayer);
	vector<coordinates> analyzeForThreeWithGap(penteBoard* boardToAnalyze, bool whitePlayer);
	vector<coordinates> allAdjacent(penteBoard* boardToAnalyze);
};

namespace evaluateBoard {
	coordinates analyzeForChains(penteBoard* boardToAnalyze, bool whitePlayer, int chainLength);
	int analyzeForTakings(penteBoard* boardToAnalyze, bool whitePlayer);
	coordinates analyzeForGap(penteBoard* boardToAnalyze, bool whitePlayer);
}