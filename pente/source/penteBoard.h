#pragma once

#include"declarations.h"
#include<vector>
#include<iostream>
#include<vector>
#include<Windows.h>
#include<cstdio>
#include<fstream>
#include<string>
#include<filesystem>
#include<iterator>

using namespace std;

class penteBoard {
private:
	int takesForWhite = 0;
	int takesForBlack = 0;
	vector<vector<coordinates>> moveHistory;
	int winner = -1;
	bool isPro;
public:
	bool isWhiteTurn = false;
	int penteVariant;
	bool gameWon = false;
	int board[BOARDSIZE][BOARDSIZE];

	penteBoard();
	//wczytywanie zaklada, ¿e plik zawsze jest poprawny
	penteBoard(string pathToLoad);
	penteBoard(int variant, bool pro);
	void savePenteBoard(bool autosave = false);
	void printBoardToConsoleASCII();
	void printBoardToConsoleUTF8();
	bool checkIfMoveLegal(int x, int y);
	bool checkIfMoveLegalProPente(int x, int y);
	bool makeMove(int x, int y);
	void displayCredits();
	void unmakeMove();
	
};