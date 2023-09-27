#pragma once

#include"penteBoard.h"
#include "declarations.h"
#include<array>
#include<vector>
#include<mutex>

using namespace std;

struct treeNode {
	int visits;
	double totalScore;
	vector<treeNode*> children;
	treeNode* parent;
	coordinates previousMove;
	mutex expansionMutex;
	mutex valueChangeMute;
	mutex childrenMutex;
	treeNode() : visits(0), totalScore(0.0), parent(nullptr), children() {}
	~treeNode() {
		for (auto& child : children)
			delete child;
	}
};

class penteBoard;

class computerPlayer {
private:
public:
	void runMCTSLoop(penteBoard initialState, std::chrono::high_resolution_clock::time_point startTime, std::chrono::duration<double> timeRestriction, treeNode* root);
	virtual coordinates findBestMove(penteBoard* currentBoard, bool whiteTurn);
	coordinates generateFirstMoves(penteBoard* currentBoard, bool whiteTurn);
	int evaluatePlayer(penteBoard* currentBoard, bool whiteTurn);
	int staticPositionEvaluation(penteBoard* currentBoard);
	void performMCTSTurn(penteBoard& initialState, treeNode* rootNode);
	coordinates monteCarloTreeSearch(penteBoard& initialState, float seconds);
	treeNode* selectNodeUCB(treeNode* parent, bool isWhite);
	bool expandNode(penteBoard& board, treeNode* parent);
};

class advancedComputer : public computerPlayer {
private:
public:

	coordinates findBestMove(penteBoard* currentBoard, bool whiteTurn) override;

};

class expertComputer : public computerPlayer {
private:
public:
	coordinates findBestMove(penteBoard* currentBoard, bool whiteTurn) override;

};

class mediumComputer : public computerPlayer {
private:
public:
	coordinates findBestMove(penteBoard* currentBoard, bool whiteTurn) override;

};

class masterComputer : public computerPlayer {
private:
public:
	coordinates findBestMove(penteBoard* currentBoard, bool whiteTurn) override;

};

class easyComputer : public computerPlayer {
private:
public:
	coordinates findBestMove(penteBoard* currentBoard, bool whiteTurn) override;

};

computerPlayer* generateAIInstance(int difficulty);