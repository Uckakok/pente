#pragma once

#include<array>
#include"declarations.h"
#include<vector>
#include"analyzeBoard.h"
#include"penteBoard.h"




class computerPlayer {
private:
public:
	static computerPlayer* currentInstance;
	virtual coordinates findBestMove(penteBoard *currentBoard, bool whiteTurn);
	array<vector<coordinates>, 16> getAllGoodMoves(penteBoard *currentBoard, bool whiteTurn);
	coordinates generateFirstMoves(penteBoard *currentBoard, bool whiteTurn);
};

class advancedComputer : public computerPlayer{
private:
public:

	coordinates findBestMove(penteBoard *currentBoard, bool whiteTurn) override;

};

computerPlayer* generateAIInstance(int difficulty); 
