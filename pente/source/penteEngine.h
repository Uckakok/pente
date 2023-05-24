#pragma once

#include<array>
#include"declarations.h"
#include<vector>
#include"analyzeBoard.h"
#include"penteBoard.h"
#include<mutex>
#include<thread>
#include<atomic>




class computerPlayer {
private:
public:
	static computerPlayer* currentInstance;
	virtual coordinates findBestMove(penteBoard *currentBoard, bool whiteTurn);
	array<vector<coordinates>, 16> getAllGoodMoves(penteBoard *currentBoard, bool whiteTurn);
	coordinates generateFirstMoves(penteBoard *currentBoard, bool whiteTurn);
	int evaluatePlayer(penteBoard * currentBoard, bool whiteTurn);
	int staticPositionEvaluation(penteBoard *currentBoard);
	int minMaxAlgorithm(penteBoard *currentBoard, int depth, bool whiteTurn);
	vector<coordinates> generateMovesWorthChecking(penteBoard *currentBoard, bool whiteTurn);
	int minMaxWraper(penteBoard currentBoard, int depth, bool whiteTurn);
};

class advancedComputer : public computerPlayer{
private:
public:

	coordinates findBestMove(penteBoard *currentBoard, bool whiteTurn) override;

};

class expertComputer : public computerPlayer {
private:
public:
	coordinates findBestMove(penteBoard *currentBoard, bool whiteTurn) override;

};

class mediumComputer : public computerPlayer {
private:
public:
	coordinates findBestMove(penteBoard *currentBoard, bool whiteTurn) override;

};

computerPlayer* generateAIInstance(int difficulty); 
