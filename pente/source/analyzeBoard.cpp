#include "analyzeBoard.h"
#include <unordered_set>

vector<chain> analyzeBoard::analyzeForChains(penteBoard* boardToAnalyze, bool whitePlayer, int lineLength)
{
	vector<chain> allChains;
	coordinates blockingPos[2];
	coordinates noBlocking = { -1, -1 };
	int dir[][2] = { {1, 0}, {-1, 0}, { 0, 1 }, {0, -1}, {-1, -1}, {1, 1 }, {-1, 1}, {1, -1} };
	for (int x = 0; x < BOARDSIZE; ++x) {
		if (x % lineLength != 0) {
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
				if (inLine == lineLength) {
					line.dirIndex = i;
					line.length = inLine;
					allChains.push_back(line);
				}
			}

		}
	}
	for (int x = 0; x < BOARDSIZE; ++x) {
		for (int y = 0; y < BOARDSIZE; ++y) {
			if (y % lineLength != 0) {
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
			if (inLine == lineLength) {
				line.dirIndex = 0;
				line.length = inLine;
				allChains.push_back(line);
			}

		}
	}
	return allChains;
}

vector<coordinates> analyzeBoard::analyzeForFourWithGap(penteBoard* boardToAnalyze, bool whitePlayer)
{
	vector<coordinates> fourWithGap;
	int dir[][2] = { {0, 1}, {1, 0}, {1, 1 }, {1, -1} };
	for (int x = 0; x < BOARDSIZE; ++x) {
		for (int y = 0; y < BOARDSIZE; ++y) {
			if (boardToAnalyze->board[y][x] != (whitePlayer ? WHITE : BLACK)) {
				continue;
			}
			for (int i = 0; i < 4; ++i) {
				bool chainPossible = true;
				if (x + 5 * dir[i][1] < 0 || x + 5 * dir[i][1] >= BOARDSIZE || y + 5 * dir[i][0] < 0 || y + 5 * dir[i][0] >= BOARDSIZE) {
					continue;
				}
				if (boardToAnalyze->board[y + dir[i][0]][x + dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 2 * dir[i][0]][x + 2 * dir[i][1]] == EMPTY &&
					boardToAnalyze->board[y + 3 * dir[i][0]][x + 3 * dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 4 * dir[i][0]][x + 4 * dir[i][1]] == (whitePlayer ? WHITE : BLACK)) {
					fourWithGap.push_back({ x + 2 * dir[i][1] , y + 2 * dir[i][0] });
				}
				if (boardToAnalyze->board[y + dir[i][0]][x + dir[i][1]] == EMPTY &&
					boardToAnalyze->board[y + 2 * dir[i][0]][x + 2 * dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 3 * dir[i][0]][x + 3 * dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 4 * dir[i][0]][x + 4 * dir[i][1]] == (whitePlayer ? WHITE : BLACK)) {
					fourWithGap.push_back({ x + 1 * dir[i][1] , y + 1 * dir[i][0] });
				}
				if (boardToAnalyze->board[y + dir[i][0]][x + dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 2 * dir[i][0]][x + 2 * dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 3 * dir[i][0]][x + 3 * dir[i][1]] == EMPTY &&
					boardToAnalyze->board[y + 4 * dir[i][0]][x + 4 * dir[i][1]] == (whitePlayer ? WHITE : BLACK)) {
					fourWithGap.push_back({ x + 3 * dir[i][1] , y + 3 * dir[i][0] });
				}
			}
		}
	}
	return fourWithGap;
}

array<vector<coordinates>, 5> analyzeBoard::blockingChains(penteBoard* boardToAnalyze, vector<chain> chains)
{
	int dir[][2] = { {1, 0}, {-1, 0}, { 0, 1 }, {0, -1}, {-1, -1}, {1, 1 }, {-1, 1}, {1, -1} };
	array<vector<coordinates>, 5> priorities;
	for (auto& chain : chains) {
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
		if (chain.startx + dir[chain.dirIndex + 1][1] >= 0 && chain.startx + dir[chain.dirIndex + 1][1] < BOARDSIZE && chain.starty + dir[chain.dirIndex + 1][0] >= 0 && chain.starty + dir[chain.dirIndex + 1][0] < BOARDSIZE) {
			if (boardToAnalyze->board[chain.starty + dir[chain.dirIndex + 1][0]][chain.startx + dir[chain.dirIndex + 1][1]] == EMPTY) {
				frontBlock.x = chain.startx + dir[chain.dirIndex + 1][1];
				frontBlock.y = chain.starty + dir[chain.dirIndex + 1][0];
				blocksNum++;
				front = true;
			}
		}
		if (chain.length == 4) {
			if (back) {
				priorities[0].push_back(backBlock);
			}
			if (front) {
				priorities[0].push_back(frontBlock);
			}
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
				if (back) priorities[4].push_back(backBlock);
				if (front) priorities[4].push_back(frontBlock);
			}
			else if (blocksNum == 2) {
				priorities[3].push_back(backBlock);
				priorities[3].push_back(frontBlock);
			}
		}
	}
	return priorities;
}

//searches for empty squares adjacent to pieces. And then sorts them, by pieces they are adjacent to
array<vector<coordinates>, 8> analyzeBoard::findFieldsAdjacentToPieces(penteBoard* boardToAnalyze, bool whitePlayer)
{
	array<vector<coordinates>, 8> sortedTiles;

	vector<coordinates> allTiles;
	vector<coordinatesAndNum> combined;

	int dir[][2] = { { 0, 1 }, {0, -1}, {1, 0}, {-1, 0}, {-1, -1}, {1, 1 }, {-1, 1}, {1, -1} };

	for (int x = 0; x < BOARDSIZE; ++x) {
		for (int y = 0; y < BOARDSIZE; ++y) {
			if (boardToAnalyze->board[y][x] != (whitePlayer ? WHITE : BLACK)) {
				continue;
			}
			for (int i = 0; i < 8; ++i) {
				if (x + dir[i][1] < 0 || x + dir[i][1] >= BOARDSIZE || y + dir[i][0] < 0 || y + dir[i][0] >= BOARDSIZE) {
					continue;
				}
				if (boardToAnalyze->board[y + dir[i][0]][x + dir[i][1]] == EMPTY) {
					allTiles.push_back({ x + dir[i][1] , y + dir[i][0] });
				}
			}
		}
	}
	while (allTiles.size() != 0) {
		coordinates currentlySearched = allTiles.front();
		combined.push_back({ allTiles.front().x, allTiles.front().y, 1 });
		auto i = begin(allTiles);
		i = allTiles.erase(i);
		while (i != end(allTiles)) {
			if (*i == currentlySearched) {
				combined.back().num++;
				i = allTiles.erase(i);
			}
			else
				++i;
		}
	}
	for (auto& element : combined) {
		sortedTiles[element.num - 1].push_back((coordinates)element);
	}

	return sortedTiles;
}

vector<coordinates> analyzeBoard::analyzeForThreeWithGap(penteBoard* boardToAnalyze, bool whitePlayer)
{
	vector<coordinates> threeWithGap;
	int dir[][2] = { { 0, 1 }, {0, -1}, {1, 0}, {-1, 0}, {-1, -1}, {1, 1 }, {-1, 1}, {1, -1} };
	for (int x = 0; x < BOARDSIZE; ++x) {
		for (int y = 0; y < BOARDSIZE; ++y) {
			if (boardToAnalyze->board[y][x] != (whitePlayer ? WHITE : BLACK)) {
				continue;
			}
			for (int i = 0; i < 8; ++i) {
				bool chainPossible = true;
				if (x + 5 * dir[i][1] < 0 || x + 5 * dir[i][1] >= BOARDSIZE || y + 5 * dir[i][0] < 0 || y + 5 * dir[i][0] >= BOARDSIZE ||
					x + -1 * dir[i][1] < 0 || x + -1 * dir[i][1] >= BOARDSIZE || y + -1 * dir[i][0] < 0 || y + -1 * dir[i][0] >= BOARDSIZE) {
					continue;
				}
				if (boardToAnalyze->board[y + dir[i][0]][x + dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 2 * dir[i][0]][x + 2 * dir[i][1]] == EMPTY &&
					boardToAnalyze->board[y + 3 * dir[i][0]][x + 3 * dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 4 * dir[i][0]][x + 4 * dir[i][1]] == EMPTY &&
					boardToAnalyze->board[y + -1 * dir[i][0]][x + -1 * dir[i][1]] == EMPTY) {
					threeWithGap.push_back({ x + 2 * dir[i][1] , y + 2 * dir[i][0] });
				}
				if (boardToAnalyze->board[y + dir[i][0]][x + dir[i][1]] == EMPTY &&
					boardToAnalyze->board[y + 2 * dir[i][0]][x + 2 * dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 3 * dir[i][0]][x + 3 * dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 4 * dir[i][0]][x + 4 * dir[i][1]] == EMPTY &&
					boardToAnalyze->board[y + -1 * dir[i][0]][x + -1 * dir[i][1]] == EMPTY) {
					threeWithGap.push_back({ x + dir[i][1] , y + dir[i][0] });
				}
			}
		}
	}
	return threeWithGap;
}

//return vector of all empty squares adjacent to any piece. Simmilar to analyzeBoard::findFieldsAdjacentToPieces function
//but ignores number of occurences
vector<coordinates> analyzeBoard::allAdjacent(penteBoard* boardToAnalyze)
{
	unordered_set<int> coordinateSet;
	vector<coordinates> result;
	if (boardToAnalyze->gameWon) return result;
	if (boardToAnalyze->checkIfBoardFull()) return result;

	for (int y = 0; y < BOARDSIZE; ++y) {
		for (int x = 0; x < BOARDSIZE; ++x) {
			if (boardToAnalyze->board[y][x] != EMPTY) {
				for (int i = -1; i <= 1; ++i) {
					for (int j = -1; j <= 1; ++j) {
						int neighborX = x + j;
						int neighborY = y + i;

						if (neighborX < 0 || neighborX >= BOARDSIZE || neighborY < 0 || neighborY >= BOARDSIZE ||
							(neighborX == x && neighborY == y)) {
							continue;
						}

						if (boardToAnalyze->board[neighborY][neighborX] == EMPTY) {
							int key = neighborY * BOARDSIZE + neighborX;
							coordinateSet.insert(key);
						}
					}
				}
			}
		}
	}

	result.reserve(coordinateSet.size());
	for (const auto& coordinate : coordinateSet) {
		result.push_back({ coordinate % BOARDSIZE, coordinate / BOARDSIZE });
	}

	return result;
}

//analyzes for line of pieces. Is quite lengthy, as it searches every nth row or column (n being chainLength) making it faster, but requiring
//it to do two pases. Could possibly be simplified to one pass with parameters. But I'm considering using different approaches for
//board evaluation, so I don't think it's worth spending my time, as it will work the same.
coordinates evaluateBoard::analyzeForChains(penteBoard* boardToAnalyze, bool whitePlayer, int chainLength)
{
	int openChains = 0;
	int oneSideBlockedChains = 0;
	int dir[][2] = { {1, 0}, {-1, 0}, { 0, 1 }, {0, -1}, {-1, -1}, {1, 1 }, {-1, 1}, {1, -1} };
	for (int x = 0; x < BOARDSIZE; x += chainLength) {
		for (int y = 0; y < BOARDSIZE; ++y) {
			if (boardToAnalyze->board[y][x] != (whitePlayer ? WHITE : BLACK)) {
				continue;
			}
			for (int i = 2; i < 8; i += 2) {
				int inLine = 1;
				int openDirs = 2;
				for (int j = 1; j < chainLength + 1; ++j) {
					if (x + j * dir[i][1] < 0 || x + j * dir[i][1] >= BOARDSIZE || y + j * dir[i][0] < 0 || y + j * dir[i][0] >= BOARDSIZE) {
						break;
					}
					if (boardToAnalyze->board[y + j * dir[i][0]][x + j * dir[i][1]] == (whitePlayer ? WHITE : BLACK)) {
						inLine++;
					}
					else if (boardToAnalyze->board[y + j * dir[i][0]][x + j * dir[i][1]] == (whitePlayer ? BLACK : WHITE)) {
						openDirs--;
						break;
					}
					else if (boardToAnalyze->board[y + j * dir[i][0]][x + j * dir[i][1]] == EMPTY) {
						break;
					}
				}
				for (int j = 1; j < chainLength + 1; ++j) {
					if (x + j * dir[i + 1][1] < 0 || x + j * dir[i + 1][1] >= BOARDSIZE || y + j * dir[i + 1][0] < 0 || y + j * dir[i + 1][0] >= BOARDSIZE) {
						break;
					}
					if (boardToAnalyze->board[y + j * dir[i + 1][0]][x + j * dir[i + 1][1]] == (whitePlayer ? WHITE : BLACK)) {
						inLine++;
					}
					else if (boardToAnalyze->board[y + j * dir[i + 1][0]][x + j * dir[i + 1][1]] == (whitePlayer ? BLACK : WHITE)) {
						openDirs--;
						break;
					}
					else if (boardToAnalyze->board[y + j * dir[i + 1][0]][x + j * dir[i + 1][1]] == EMPTY) {
						break;
					}
				}
				if (inLine == chainLength) {
					switch (openDirs) {
					case 2:
						openChains++;
						break;
					case 1:
						oneSideBlockedChains++;
						break;
					default:
						break;
					}
				}
			}

		}
	}
	for (int x = 0; x < BOARDSIZE; ++x) {
		for (int y = 0; y < BOARDSIZE; y += chainLength) {
			if (boardToAnalyze->board[y][x] != (whitePlayer ? WHITE : BLACK)) {
				continue;
			}
			int inLine = 1;
			int openDirs = 2;
			for (int j = 1; j < chainLength + 1; ++j) {
				if (x + j * dir[0][1] < 0 || x + j * dir[0][1] >= BOARDSIZE || y + j * dir[0][0] < 0 || y + j * dir[0][0] >= BOARDSIZE) {
					break;
				}
				if (boardToAnalyze->board[y + j * dir[0][0]][x + j * dir[0][1]] == (whitePlayer ? WHITE : BLACK)) {
					inLine++;
				}
				else if (boardToAnalyze->board[y + j * dir[0][0]][x + j * dir[0][1]] == (whitePlayer ? BLACK : WHITE)) {
					openDirs--;
					break;
				}
				else if (boardToAnalyze->board[y + j * dir[0][0]][x + j * dir[0][1]] == EMPTY) {
					break;
				}
			}
			for (int j = 1; j < chainLength + 1; ++j) {
				if (x + j * dir[1][1] < 0 || x + j * dir[1][1] >= BOARDSIZE || y + j * dir[1][0] < 0 || y + j * dir[1][0] >= BOARDSIZE) {
					break;
				}
				if (boardToAnalyze->board[y + j * dir[1][0]][x + j * dir[1][1]] == (whitePlayer ? WHITE : BLACK)) {
					inLine++;
				}
				else if (boardToAnalyze->board[y + j * dir[1][0]][x + j * dir[1][1]] == (whitePlayer ? BLACK : WHITE)) {
					openDirs--;
					break;
				}
				else if (boardToAnalyze->board[y + j * dir[1][0]][x + j * dir[1][1]] == EMPTY) {
					break;
				}
			}
			if (inLine == chainLength) {
				switch (openDirs) {
				case 2:
					openChains++;
					break;
				case 1:
					oneSideBlockedChains++;
					break;
				default:
					break;
				}
			}

		}
	}
	return { openChains, oneSideBlockedChains };
}

//Returns number of takings available for specified player
int evaluateBoard::analyzeForTakings(penteBoard* boardToAnalyze, bool whitePlayer)
{
	int possibleTakings = 0;
	int dir[][2] = { { 0, 1 }, {0, -1}, {1, 0}, {-1, 0}, {-1, -1}, {1, 1 }, {-1, 1}, {1, -1} };
	int neccessaryChecks = (boardToAnalyze->penteVariant == KERYOPENTE ? 5 : 4);
	for (int x = 0; x < BOARDSIZE; ++x) {
		for (int y = 0; y < BOARDSIZE; ++y) {
			if (boardToAnalyze->board[y][x] != (whitePlayer ? WHITE : BLACK)) {
				continue;
			}
			bool canTake[8] = { false, false, false, false, false, false, false, false };
			for (int i = 0; i < 8; ++i) {
				int tempEnemies = 0;
				for (int j = 1; j < neccessaryChecks; ++j) {
					if (x + j * dir[i][1] < 0 || x + j * dir[i][1] >= BOARDSIZE || y + j * dir[i][0] < 0 || y + j * dir[i][0] >= BOARDSIZE) {
						canTake[i] = false;
						break;
					}
					if (boardToAnalyze->board[y + j * dir[i][0]][x + j * dir[i][1]] == (whitePlayer ? BLACK : WHITE)) {
						tempEnemies++;
					}
					else if (boardToAnalyze->board[y + j * dir[i][0]][x + j * dir[i][1]] == (whitePlayer ? WHITE : BLACK)) {
						canTake[i] = false;
						break;
					}
					else {
						canTake[i] = true;
						break;
					}

				}
				if (!canTake[i]) break;
				switch (tempEnemies) {
				case 3:
					if (boardToAnalyze->penteVariant == REGULARPENTE) break;
				case 2:
					possibleTakings++;
					break;
				default:
					break;
				}
			}
		}
	}
	return possibleTakings;
}

//searches for structures of line, with gap, which probably should be blocked: eg. #_##
coordinates evaluateBoard::analyzeForGap(penteBoard* boardToAnalyze, bool whitePlayer)
{
	coordinates gaps;
	int dir[][2] = { {0, 1}, {1, 0}, {1, 1 }, {1, -1} };
	for (int x = 0; x < BOARDSIZE - 3; ++x) {
		for (int y = 0; y < BOARDSIZE - 3; ++y) {
			if (boardToAnalyze->board[y][x] != (whitePlayer ? WHITE : BLACK)) {
				continue;
			}
			for (int i = 0; i < 4; ++i) {
				bool chainPossible = true;
				if (x + 3 * dir[i][1] < 0 || x + 3 * dir[i][1] >= BOARDSIZE || y + 3 * dir[i][0] < 0 || y + 3 * dir[i][0] >= BOARDSIZE) {
					continue;
				}
				if (boardToAnalyze->board[y + dir[i][0]][x + dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 2 * dir[i][0]][x + 2 * dir[i][1]] == EMPTY &&
					boardToAnalyze->board[y + 3 * dir[i][0]][x + 3 * dir[i][1]] == (whitePlayer ? WHITE : BLACK)) {
					if (x + 4 * dir[i][1] < 0 || x + 4 * dir[i][1] >= BOARDSIZE || y + 4 * dir[i][0] < 0 || y + 4 * dir[i][0] >= BOARDSIZE) {
						gaps.y++;
					}
					else if (boardToAnalyze->board[y + 4 * dir[i][0]][x + 4 * dir[i][1]] == (whitePlayer ? WHITE : BLACK)) {
						gaps.x++;
					}
					else {
						gaps.y++;
					}

				}
				else if (boardToAnalyze->board[y + dir[i][0]][x + dir[i][1]] == EMPTY &&
					boardToAnalyze->board[y + 2 * dir[i][0]][x + 2 * dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 3 * dir[i][0]][x + 3 * dir[i][1]] == (whitePlayer ? WHITE : BLACK)) {
					if (x + 4 * dir[i][1] < 0 || x + 4 * dir[i][1] >= BOARDSIZE || y + 4 * dir[i][0] < 0 || y + 4 * dir[i][0] >= BOARDSIZE) {
						gaps.y++;
					}
					else if (boardToAnalyze->board[y + 4 * dir[i][0]][x + 4 * dir[i][1]] == (whitePlayer ? WHITE : BLACK)) {
						gaps.x++;
					}
					else {
						gaps.y++;
					}
				}
				else if (x + 4 * dir[i][1] < 0 || x + 4 * dir[i][1] >= BOARDSIZE || y + 4 * dir[i][0] < 0 || y + 4 * dir[i][0] >= BOARDSIZE) {
					continue;
				}
				else if (boardToAnalyze->board[y + dir[i][0]][x + dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 2 * dir[i][0]][x + 2 * dir[i][1]] == (whitePlayer ? WHITE : BLACK) &&
					boardToAnalyze->board[y + 3 * dir[i][0]][x + 3 * dir[i][1]] == EMPTY &&
					boardToAnalyze->board[y + 4 * dir[i][0]][x + 4 * dir[i][1]] == (whitePlayer ? WHITE : BLACK)) {
					gaps.x++;
				}
			}
		}
	}
	return gaps;
}