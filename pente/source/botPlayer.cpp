#include "botPlayer.h"


vector<coordinates> botPlayer::analyzeForTakings(penteBoard * boardToAnalyze)
{
	vector<coordinates> possibleTakings;
	int dir[][2] = { { 0, 1 }, {0, -1}, {1, 0}, {-1, 0}, {-1, -1}, {1, 1 }, {-1, 1}, {1, -1} };
	for (int x = 0; x < BOARDSIZE; ++x) {
		for (int y = 0; y < BOARDSIZE; ++y) {
			if (boardToAnalyze->board[y][x] != (boardToAnalyze->isWhiteTurn ? WHITE : BLACK)) {
				continue;
			}
			bool canContinue[8] = { true, true, true, true, true, true, true, true };
			for (int i = 0; i < 8; ++i) {
				coordinates temp;
				int tempEnemies = 0;
				for (int j = 1; j < 4; ++j) {
					if (x + j * dir[i][1] < 0 || x + j * dir[i][1] >= BOARDSIZE || y + j * dir[i][0] < 0 || y + j * dir[i][0] >= BOARDSIZE) {
						break;
					}
					if (boardToAnalyze->board[y + j * dir[i][0]][x + j * dir[i][1]] == (boardToAnalyze->isWhiteTurn ? BLACK : WHITE) && canContinue[i]) {
						tempEnemies++;
					}
					else if (boardToAnalyze->board[y + j * dir[i][0]][x + j * dir[i][1]] == (boardToAnalyze->isWhiteTurn ? WHITE : BLACK) && canContinue[i]) {
						canContinue[i] = false;
					}
					else if (boardToAnalyze->board[y + j * dir[i][0]][x + j * dir[i][1]] == EMPTY) {
						canContinue[i] = false;
						temp.x = x + j * dir[i][1];
						temp.y = y + j * dir[i][0];
					}
					
				}
				if ((tempEnemies == 2) || (boardToAnalyze->penteVariant == KERYOPENTE && tempEnemies == 3)) {
					possibleTakings.push_back(temp);
				}
			}
		}
	}
	return possibleTakings;
}

vector<chain> botPlayer::analyzeForChains(penteBoard * boardToAnalyze, bool whitePlayer, int minLineLength)
{
	vector<chain> allChains;
	coordinates blockingPos[2];
	coordinates noBlocking = { -1, -1 };
	int dir[][2] = { {1, 0}, {-1, 0}, { 0, 1 }, {0, -1}, {-1, -1}, {1, 1 }, {-1, 1}, {1, -1} };
	for (int x = 0; x < BOARDSIZE; ++x) {
		if (x % minLineLength != 0) {
			continue;
		}
		for (int y = 0; y < BOARDSIZE; ++y) {
			if (boardToAnalyze->board[y][x] != (whitePlayer ? WHITE : BLACK)) {
				continue;
			}
			for (int i = 2; i < 8; i += 2) {
				chain line;
				int inLine = 1;
				line.startx = x;
				line.starty = y;
				line.endx = x;
				line.endy = y;
				for (int j = 1; j < 4; ++j) {
					if (x + j * dir[i][1] < 0 || x + j * dir[i][1] >= BOARDSIZE || y + j * dir[i][0] < 0 || y + j * dir[i][0] >= BOARDSIZE) {
						break;
					}
					if (boardToAnalyze->board[y + j * dir[i][0]][x + j * dir[i][1]] == (whitePlayer ? WHITE : BLACK)) {
						line.endx = x + j * dir[i][1];
						line.endy = y + j * dir[i][0];
						inLine++;
					}
					else if (boardToAnalyze->board[y + j * dir[i][0]][x + j * dir[i][1]] == (whitePlayer ? BLACK : WHITE)) {
						blockingPos[0] = noBlocking;
						break;
					}
					else if (boardToAnalyze->board[y + j * dir[i][0]][x + j * dir[i][1]] == EMPTY) {
						coordinates temp;
						temp.x = x + j * dir[i][1];
						temp.y = y + j * dir[i][0];
						blockingPos[0] = temp;
						break;
					}
				}
				//test w przeciwnym kierunku
				for (int j = 1; j < 4; ++j) {
					if (x + j * dir[i + 1][1] < 0 || x + j * dir[i + 1][1] >= BOARDSIZE || y + j * dir[i + 1][0] < 0 || y + j * dir[i + 1][0] >= BOARDSIZE) {
						break;
					}
					if (boardToAnalyze->board[y + j * dir[i + 1][0]][x + j * dir[i + 1][1]] == (whitePlayer ? WHITE : BLACK)) {
						line.startx = x + j * dir[i + 1][1];
						line.starty = y + j * dir[i + 1][0];
						inLine++;
					}
					else if (boardToAnalyze->board[y + j * dir[i + 1][0]][x + j * dir[i + 1][1]] == (whitePlayer ? BLACK : WHITE)) {
						blockingPos[1] = noBlocking;
						break;
					}
					else if (boardToAnalyze->board[y + j * dir[i + 1][0]][x + j * dir[i + 1][1]] == EMPTY) {
						coordinates temp;
						temp.x = x + j * dir[i + 1][1];
						temp.y = y + j * dir[i + 1][0];
						blockingPos[1] = temp;
						break;
					}
				}
				if (inLine >= minLineLength) {
					line.dirIndex = i;
					line.length = inLine;
					allChains.push_back(line);
				}
			}

		}
	}
	for (int x = 0; x < BOARDSIZE; ++x) {
		for (int y = 0; y < BOARDSIZE; ++y) {
			if (y % minLineLength != 0) {
				continue;
			}
			if (boardToAnalyze->board[y][x] != (whitePlayer ? WHITE : BLACK)) {
				continue;
			}
			chain line;
			int inLine = 1;
			line.startx = x;
			line.starty = y;
			line.endx = x;
			line.endy = y;
			for (int j = 1; j < 4; ++j) {
				if (x + j * dir[0][1] < 0 || x + j * dir[0][1] >= BOARDSIZE || y + j * dir[0][0] < 0 || y + j * dir[0][0] >= BOARDSIZE) {
					break;
				}
				int coord1 = y + j * dir[0][0];
				int coord2 = x + j * dir[0][1];
				if (boardToAnalyze->board[y + j * dir[0][0]][x + j * dir[0][1]] == (whitePlayer ? WHITE : BLACK)) {
					line.endx = x + j * dir[0][1];
					line.endy = y + j * dir[0][0];
					inLine++;
				}
				else if (boardToAnalyze->board[y + j * dir[0][0]][x + j * dir[0][1]] == (whitePlayer ? BLACK : WHITE)) {
					blockingPos[0] = noBlocking;
					break;
				}
				else if (boardToAnalyze->board[y + j * dir[0][0]][x + j * dir[0][1]] == EMPTY) {
					coordinates temp;
					temp.x = x + j * dir[0][1];
					temp.y = y + j * dir[0][0];
					blockingPos[0] = temp;
					break;
				}
			}
			//test w przeciwnym kierunku
			for (int j = 1; j < 4; ++j) {
				if (x + j * dir[1][1] < 0 || x + j * dir[1][1] >= BOARDSIZE || y + j * dir[1][0] < 0 || y + j * dir[1][0] >= BOARDSIZE) {
					break;
				}
				if (boardToAnalyze->board[y + j * dir[1][0]][x + j * dir[1][1]] == (whitePlayer ? WHITE : BLACK)) {
					line.startx = x + j * dir[1][1];
					line.starty = y + j * dir[1][0];
					inLine++;
				}
				else if (boardToAnalyze->board[y + j * dir[1][0]][x + j * dir[1][1]] == (whitePlayer ? BLACK : WHITE)) {
					blockingPos[1] = noBlocking;
					break;
				}
				else if (boardToAnalyze->board[y + j * dir[1][0]][x + j * dir[1][1]] == EMPTY) {
					coordinates temp;
					temp.x = x + j * dir[1][1];
					temp.y = y + j * dir[1][0];
					blockingPos[1] = temp;
					break;
				}
			}
			if (inLine >= minLineLength) {
				line.dirIndex = 0;
				line.length = inLine;
				allChains.push_back(line);
			}

		}
	}
	return allChains;
}



vector<coordinates> botPlayer::analyzeForFourWithGap(penteBoard * boardToAnalyze, bool whitePlayer)
{
	vector<coordinates> fourWithGap;
	int dir[][2] = { {0, 1}, {1, 0}, {1, 1 }, {1, -1} };
	bool scheme[] = { true, true, false, true, true };
	for (int x = 0; x < BOARDSIZE; ++x) {
		for (int y = 0; y < BOARDSIZE; ++y) {
			if (boardToAnalyze->board[y][x] != (whitePlayer ? WHITE : BLACK)) {
				continue;
			}
			for (int i = 0; i < 4; ++i) {
				bool chainPossible = true;
				int inLine[4] = { 0, 0, 0, 0 };
				if (x + 5 * dir[i][1] < 0 && x + 5 * dir[i][1] >= BOARDSIZE && y + 5 * dir[i][0] < 0 && y + 5 * dir[i][0] >= BOARDSIZE) {
					continue;
				}
				if (boardToAnalyze->board[y + dir[i][0]][x + dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 2 * dir[i][0]][x + 2 * dir[i][1]] == EMPTY &&
					boardToAnalyze->board[y + 3 * dir[i][0]][x + 3 * dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 4 * dir[i][0]][x + 4 * dir[i][1]] == (whitePlayer ? WHITE : BLACK)) {
					coordinates temp;
					temp.x = x + 2 * dir[i][1];
					temp.y = y + 2 * dir[i][0];
					fourWithGap.push_back(temp);
				}
				if (boardToAnalyze->board[y + 2 * dir[i][0]][x + 2 * dir[i][1]] == EMPTY &&
					boardToAnalyze->board[y + dir[i][0]][x + dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 3 * dir[i][0]][x + 3 * dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 4 * dir[i][0]][x + 4 * dir[i][1]] == (whitePlayer ? WHITE : BLACK)) {
					coordinates temp;
					temp.x = x + 1 * dir[i][1];
					temp.y = y + 1 * dir[i][0];
					fourWithGap.push_back(temp);
				}
				if (boardToAnalyze->board[y + dir[i][0]][x + dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 3 * dir[i][0]][x + 3 * dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 2 * dir[i][0]][x + 2 * dir[i][1]] == EMPTY &&
					boardToAnalyze->board[y + 4 * dir[i][0]][x + 4 * dir[i][1]] == (whitePlayer ? WHITE : BLACK)) {
					coordinates temp;
					temp.x = x + 3 * dir[i][1];
					temp.y = y + 3 * dir[i][0];
					fourWithGap.push_back(temp);
				}
			}
		}
	}
	return fourWithGap;
}

array<vector<coordinates>, 4> botPlayer::blockingChains(penteBoard * boardToAnalyze, vector<chain> chains)
{
	int dir[][2] = { {1, 0}, {-1, 0}, { 0, 1 }, {0, -1}, {-1, -1}, {1, 1 }, {-1, 1}, {1, -1} };
	array<vector<coordinates>, 4> priorities;
	for (auto & chain : chains) {
		coordinates backBlock;
		coordinates frontBlock;
		bool back = false;
		bool front = false;
		int blocksNum = 0;
		if (chain.endx + dir[chain.dirIndex][1] >= 0 && chain.endx + dir[chain.dirIndex][1] < BOARDSIZE && chain.endy + dir[chain.dirIndex][0] >= 0 && chain.endy + dir[chain.dirIndex][0] < BOARDSIZE) {
			if (boardToAnalyze->board[chain.endy + dir[chain.dirIndex][0]][chain.endx + dir[chain.dirIndex][1]] == EMPTY) {
				backBlock.x = chain.endx + dir[chain.dirIndex][1];
				backBlock.y = chain.endy + dir[chain.dirIndex][0];
				blocksNum++;
				back = true;
			}
		}
		if (chain.startx + dir[chain.dirIndex+1][1] >= 0 && chain.startx + dir[chain.dirIndex + 1][1] < BOARDSIZE && chain.starty + dir[chain.dirIndex + 1][0] >= 0 && chain.starty + dir[chain.dirIndex + 1][0] < BOARDSIZE) {
			if (boardToAnalyze->board[chain.starty + dir[chain.dirIndex + 1][0]][chain.startx + dir[chain.dirIndex + 1][1]] == EMPTY) {
				frontBlock.x = chain.startx + dir[chain.dirIndex + 1][1];
				frontBlock.y = chain.starty + dir[chain.dirIndex + 1][0];
				blocksNum++;
				front = true;
			}
		}
		if (chain.length == 4) {
			if (back) priorities[0].push_back(backBlock);
			if (front) priorities[0].push_back(frontBlock);
		}
		else if (chain.length == 3) {
			if (blocksNum == 1) {
				if (back) priorities[2].push_back(backBlock);
				else priorities[2].push_back(frontBlock);
			}
			else if (blocksNum == 2) {
				priorities[1].push_back(backBlock);
				priorities[1].push_back(frontBlock);
			}
		}
		else if (chain.length == 2) {
			if (blocksNum == 1) {
				;
			}
			else if (blocksNum == 2) {
				priorities[3].push_back(backBlock);
				priorities[3].push_back(frontBlock);
			}
		}
	}
	return priorities;
}



void botPlayer::evaluateBoard(penteBoard * boardToAnalyze)
{
}