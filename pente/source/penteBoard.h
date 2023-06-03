#pragma once

#include"declarations.h"
#include<vector>
#include"settings.h"

class computerPlayer;

using namespace std;

class penteBoard {
private:
	vector<vector<coordinates>> moveHistory;
	int minMoves = 0;
public:
	bool online = false;
	bool isHost = false;
	int takesForWhite = 0;
	int takesForBlack = 0;
	int winner = -1;
	computerPlayer *AIInstance;
	bool isAgainstAI = false;
	bool isAIWhite = false;
	bool isPro;
	bool isWhiteTurn = false;
	int penteVariant;
	bool gameWon = false;
	int board[BOARDSIZE][BOARDSIZE];

	penteBoard();
	explicit penteBoard(string pathToLoad);
	penteBoard(int variant, bool pro);
	penteBoard(const penteBoard& original);
	void savePenteBoard(bool autosave = false);
	bool checkIfMoveLegal(int x, int y);
	int getMoveHistorySize();
	bool checkIfMoveLegalProPente(int x, int y);
	bool makeMove(int x, int y);
	void displayCredits();
	void unmakeMove();
	virtual void printBoardToConsole() { ; };
	bool checkIfBoardFull();
};

class UTF8Pente : public penteBoard {
public:
	void printBoardToConsole() override;
	UTF8Pente(string pathToLoad) : penteBoard(pathToLoad) {}
	UTF8Pente() : penteBoard() {}
	UTF8Pente(const penteBoard& original) : penteBoard(original) {};
};

class ASCIIPente : public penteBoard {
public:
	void printBoardToConsole() override;
	ASCIIPente(string pathToLoad) : penteBoard(pathToLoad) {}
	ASCIIPente() : penteBoard() {}
	ASCIIPente(const penteBoard& original) : penteBoard(original) {};
};

void initalizeBoard(settings *currentSettings, penteBoard* currentBoard);
penteBoard* createBoard(settings *currentSettings);