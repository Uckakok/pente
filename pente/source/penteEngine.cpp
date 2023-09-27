#include "penteEngine.h"
#include"declarations.h"
#include"analyzeBoard.h"
#include<thread>
#include<limits.h>
#include <functional>
#include <cstdlib>

//generates first move for computer, and obeys pro rules.
coordinates computerPlayer::generateFirstMoves(penteBoard* currentBoard, bool whiteTurn)
{
	int boardMiddle = (int)((BOARDSIZE) / 2);
	if (currentBoard->isPro) {
		if (currentBoard->getMoveHistorySize() == 0) {
			return { (int)((BOARDSIZE) / 2), (int)((BOARDSIZE) / 2) };
		}
		if (currentBoard->getMoveHistorySize() == 2) {
			vector<coordinates> keryoMoves;
			for (int x = boardMiddle - 3; x < boardMiddle + 3; ++x) {
				for (int y = boardMiddle - 3; y < boardMiddle + 3; ++y) {
					if (x < boardMiddle - 2 || x > boardMiddle + 2 || y < boardMiddle - 2 || y > boardMiddle + 2) {
						keryoMoves.push_back({ x, y });
					}
				}
			}
			return keryoMoves[rand() % keryoMoves.size()];
		}
	}
	if (currentBoard->getMoveHistorySize() <= 2) {
		if (currentBoard->board[boardMiddle][boardMiddle] == EMPTY) {
			return { boardMiddle, boardMiddle };
		}
		else {
			vector<coordinates> possibleMoves;
			for (int x = boardMiddle - 1; x <= boardMiddle + 1; ++x) {
				for (int y = boardMiddle - 1; y <= boardMiddle + 1; ++y) {
					if (currentBoard->board[y][x] == EMPTY) {
						possibleMoves.push_back({ x, y });
					}
				}
			}
			return possibleMoves[rand() % possibleMoves.size()];
		}
	}
	return { -1, -1 };
}

//evaluates player, checking all available for moves and rates them
int computerPlayer::evaluatePlayer(penteBoard* currentBoard, bool whiteTurn) {
	int score = 0;
	//one move win
	coordinates gaps = evaluateBoard::analyzeForGap(currentBoard, whiteTurn);
	coordinates chainsFourLong = evaluateBoard::analyzeForChains(currentBoard, whiteTurn, 4);

	score += chainsFourLong.x * ONEMOVEWIN;
	score += chainsFourLong.y * ONEMOVEWIN;
	score += gaps.x * ONEMOVEWIN;

	//creating four (two move win)
	coordinates chainsThreeLong = evaluateBoard::analyzeForChains(currentBoard, whiteTurn, 3);

	score += chainsThreeLong.x * UNLOCKEDFOUR;
	score += gaps.y * UNLOCKEDFOUR;

	//locked four (forcing move from enemy)
	score += chainsThreeLong.y * LOCKEDFOUR;

	//taking enemy
	int takes = evaluateBoard::analyzeForTakings(currentBoard, whiteTurn) * POSSIBLETAKING;
	score += takes;

	//creating open three (forcing move from enemy)
	coordinates chainsTwoLong = evaluateBoard::analyzeForChains(currentBoard, whiteTurn, 2);
	score += chainsTwoLong.x * UNLOCKEDTHREE;

	array<vector<coordinates>, 8> adjacent = analyzeBoard::findFieldsAdjacentToPieces(currentBoard, whiteTurn);
	score += adjacent[7].size() * ADJACENT8;
	score += adjacent[6].size() * ADJACENT7;
	score += adjacent[5].size() * ADJACENT6;
	score += adjacent[4].size() * ADJACENT5;
	score += adjacent[3].size() * ADJACENT4;
	score += adjacent[2].size() * ADJACENT3;
	score += adjacent[1].size() * ADJACENT2;
	score += adjacent[0].size() * ADJACENT1;

	return score;
}

//assign position a score
int computerPlayer::staticPositionEvaluation(penteBoard* currentBoard)
{
	if (currentBoard->gameWon) {
		if (currentBoard->winner == WHITE) {
			return WINVALUE;
		}
		else if (currentBoard->winner == BLACK) {
			return -WINVALUE;
		}
	}
	int score;
	int whiteScore = evaluatePlayer(currentBoard, true);
	int blackScore = evaluatePlayer(currentBoard, false);
	score = whiteScore - blackScore;

	int takes = currentBoard->takesForWhite - currentBoard->takesForBlack;
	score += takes * TAKENPAWNS;
	return score;
}


//self explanatory
void computerPlayer::performMCTSTurn(penteBoard& initialState, treeNode* rootNode)
{
	// Selection phase
	treeNode* node = rootNode;
	while (!node->children.empty()) {
		node = selectNodeUCB(node, initialState.isWhiteTurn);
		initialState.makeMove(node->previousMove.x, node->previousMove.y);
	}

	if (!expandNode(initialState, node)) {
		while (node->parent != nullptr) {
			node = node->parent;
			initialState.unmakeMove();
		}
	}

	double score = (double)staticPositionEvaluation(&initialState);
	if (score > 0) score = 1;
	if (score < 0) score = -1;
	if (initialState.gameWon) score *= 100;
	// Backpropagation phase
	while (node->parent != nullptr) {
		node->valueChangeMute.lock();
		node->visits++;
		node->totalScore += score;
		node->valueChangeMute.unlock();
		node = node->parent;
		initialState.unmakeMove();
	}
	node->valueChangeMute.lock();
	node->visits++;
	node->totalScore += score;
	node->valueChangeMute.unlock();
}

void computerPlayer::runMCTSLoop(penteBoard initialState, std::chrono::high_resolution_clock::time_point startTime, std::chrono::duration<double> timeRestriction, treeNode* root) {
	while (std::chrono::high_resolution_clock::now() - startTime < timeRestriction) {
		performMCTSTurn(initialState, root);
	}
	return;
}


coordinates computerPlayer::monteCarloTreeSearch(penteBoard& initialState, float seconds)
{
	using namespace chrono;
	if (initialState.getMoveHistorySize() == 0) {
		return generateFirstMoves(&initialState, initialState.isWhiteTurn);
	}
	treeNode* rootNode = new treeNode(); // Create the root node of the Monte Carlo tree
	rootNode->visits = 1;
	expandNode(initialState, rootNode);

	std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> timeRestrictionInSeconds = std::chrono::duration<double>(seconds);

	vector<thread> threads;
	vector<penteBoard*> boards;

	for (int i = 0; i < 12; ++i) {
		penteBoard* boardCopy = new penteBoard(initialState);
		boards.push_back(boardCopy);
		threads.emplace_back([&, i]() {
			runMCTSLoop(*boardCopy, startTime, timeRestrictionInSeconds, rootNode);
		});
	}
	for (auto& thread : threads) {
		thread.join();
	}
	for (auto& board : boards) {
		delete board;
	}

	// Choose the best child of the root node as the recommended action
	treeNode* bestChild = nullptr;
	double bestScore;
	if (initialState.isWhiteTurn) {
		bestScore = INT_MIN;
	}
	else {
		bestScore = INT_MAX;
	}
	for (treeNode* child : rootNode->children) {
		double score = child->totalScore / child->visits;
		if (initialState.isWhiteTurn) {
			if (score > bestScore) {
				bestScore = score;
				bestChild = child;
			}
		}
		else {
			if (score < bestScore) {
				bestScore = score;
				bestChild = child;
			}
		}
	}
	coordinates bestMove = bestChild->previousMove;
	delete rootNode;
	return bestMove;
}

//uses a formula found by someone smart to decide whether it should further explore the best move, or give bad moves more tries
treeNode* computerPlayer::selectNodeUCB(treeNode* parent, bool isWhite)
{
	double explorationParameter = 5;
	double bestUCT = INT_MIN;
	treeNode* bestChild = nullptr;
	bool allScoresZero = true;
	parent->childrenMutex.lock();
	for (treeNode* child : parent->children) {
		if (child->visits == 0) {
			allScoresZero = false;
			bestChild = child;
			break;
		}
		double score = (isWhite ? child->totalScore : -child->totalScore);
		double uct = score / child->visits + explorationParameter * sqrt(log(parent->visits) / child->visits);
		if (uct > bestUCT) {
			bestUCT = uct;
			bestChild = child;
			allScoresZero = false;
		}
	}
	if (allScoresZero) {
		int minVisits = INT_MAX;
		for (treeNode* child : parent->children) {
			if (child->visits < minVisits) {
				minVisits = child->visits;
				bestChild = child;
			}
		}
	}
	parent->childrenMutex.unlock();
	return bestChild;
}

//create children for a current node
bool computerPlayer::expandNode(penteBoard& board, treeNode* parent)
{
	parent->expansionMutex.lock();
	parent->childrenMutex.lock();
	if (!parent->children.empty()) {
		parent->expansionMutex.unlock();
		parent->childrenMutex.unlock();
		return false;
	}
	vector<coordinates> allMoves = analyzeBoard::allAdjacent(&board);
	for (auto& move : allMoves) {
		treeNode* newNode = new treeNode();
		newNode->previousMove = move;
		newNode->parent = parent;
		parent->children.push_back(newNode);
	}
	parent->expansionMutex.unlock();
	parent->childrenMutex.unlock();
	return true;
}


//random move generation
coordinates computerPlayer::findBestMove(penteBoard* currentBoard, bool whiteTurn)
{
	if (currentBoard->getMoveHistorySize() <= 2) {
		return generateFirstMoves(currentBoard, whiteTurn);
	}
	coordinates moveToTest;
	do {
		moveToTest = { rand() % BOARDSIZE, rand() % BOARDSIZE };
	} while (currentBoard->board[moveToTest.y][moveToTest.x] != EMPTY);
	return moveToTest;
}



computerPlayer* generateAIInstance(int difficulty)
{
	computerPlayer* instance = nullptr;
	switch (difficulty) {
	case -1:
		instance = new computerPlayer();
		break;
	case 0:
		instance = new easyComputer();
		break;
	case 1:
		instance = new mediumComputer();
		break;
	case 2:
		instance = new advancedComputer();
		break;
	case 3:
		instance = new expertComputer();
		break;
	case 4:
		instance = new masterComputer();
		break;
	default:
		instance = new advancedComputer();
		break;
	}
	return instance;
}


//generates move using Monte Carlo Tree Search with 100 iterations
coordinates easyComputer::findBestMove(penteBoard* currentBoard, bool whiteTurn)
{
	return monteCarloTreeSearch(*currentBoard, 0.2);
}

//generates move using Monte Carlo Tree Search with 500 iterations
coordinates mediumComputer::findBestMove(penteBoard* currentBoard, bool whiteTurn)
{
	return monteCarloTreeSearch(*currentBoard, 0.5);
}

//generates move using Monte Carlo Tree Search with 2000 iterations
coordinates advancedComputer::findBestMove(penteBoard* currentBoard, bool whiteTurn)
{
	return monteCarloTreeSearch(*currentBoard, 1);
}

//generates move using Monte Carlo Tree Search with 8000 iterations
coordinates expertComputer::findBestMove(penteBoard* currentBoard, bool whiteTurn)
{
	return monteCarloTreeSearch(*currentBoard, 2);
}

//generates move using Monte Carlo Tree Search with 100000 iterations
coordinates masterComputer::findBestMove(penteBoard* currentBoard, bool whiteTurn)
{
	return monteCarloTreeSearch(*currentBoard, 5);
}
