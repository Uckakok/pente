#pragma once

#include<array>
#include<vector>
#include"penteBoard.h"
#include <bitset>




class computerPlayer {
private:
public:
	static computerPlayer* currentInstance;
	virtual coordinates findBestMove(penteBoard *currentBoard, bool whiteTurn);
	array<vector<coordinates>, 16> getAllGoodMoves(penteBoard *currentBoard, bool whiteTurn);
	coordinates generateFirstMoves(penteBoard *currentBoard, bool whiteTurn);
	int evaluatePlayer(penteBoard * currentBoard, bool whiteTurn);
	int staticPositionEvaluation(penteBoard *currentBoard);
	int minMaxAlgorithm(penteBoard *currentBoard, int depth, int alpha, int beta, bool whiteTurn);
	vector<coordinates> generateMovesWorthChecking(penteBoard *currentBoard, bool whiteTurn);
	vector<coordinates> forcedOrWinning(penteBoard *currentBoard, bool whiteTurn);
	coordinates prepareAndPerformMinMax(penteBoard *currentBoard, bool whiteTurn, int depth);
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

class masterComputer : public computerPlayer {
private:
public:
	coordinates findBestMove(penteBoard *currentBoard, bool whiteTurn) override;

};

class easyComputer : public computerPlayer {
private:
public:
	coordinates findBestMove(penteBoard *currentBoard, bool whiteTurn) override;

};

computerPlayer* generateAIInstance(int difficulty); 
bitset<730> boardHash(penteBoard *boardToHash);
void unhashBoard(bitset<730> hash);