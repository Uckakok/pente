#include"penteBoard.h"
#include<iostream>
#include<Windows.h>
#include<cstdio>
#include<fstream>
#include<string>
#include<filesystem>
#include<iterator>

penteBoard::penteBoard()
{
	isPro = false;
	penteVariant = REGULARPENTE;
	for (int i = 0; i < BOARDSIZE; ++i) {
		for (int j = 0; j < BOARDSIZE; ++j) {
			board[i][j] = EMPTY;
		}
	}
}

penteBoard::penteBoard(int variant, bool pro)
{
	isPro = pro;
	if (variant == REGULARPENTE || variant == KERYOPENTE) {
		penteVariant = variant;
	}
	else {
		penteVariant = REGULARPENTE;
	}
	for (int i = 0; i < BOARDSIZE; ++i) {
		for (int j = 0; j < BOARDSIZE; ++j) {
			board[i][j] = EMPTY;
		}
	}
}

penteBoard::penteBoard(const penteBoard& original)
{
	isPro = original.isPro;
	penteVariant = original.penteVariant;
	moveHistory = original.moveHistory;
	minMoves = original.minMoves;
	takesForWhite = original.takesForWhite;
	takesForBlack = original.takesForBlack;
	winner = original.winner;
	AIInstance = original.AIInstance;
	isAgainstAI = original.isAgainstAI;
	isAIWhite = original.isAIWhite;
	isWhiteTurn = original.isWhiteTurn;
	gameWon = original.gameWon;
	for (int i = 0; i < BOARDSIZE; ++i) {
		for (int j = 0; j < BOARDSIZE; ++j) {
			board[i][j] = original.board[i][j];
		}
	}
}

penteBoard::penteBoard(string pathToLoad)
{
	if (pathToLoad.length() <= 4 || pathToLoad.substr(pathToLoad.length() - 4, pathToLoad.length()) != ".pnt") {
		pathToLoad.append(".pnt");
	}
	ifstream gameHandle;
	gameHandle.open(pathToLoad);
	if (gameHandle.is_open()) {
		gameHandle >> isPro;
		gameHandle >> penteVariant;
		gameHandle >> takesForWhite;
		gameHandle >> takesForBlack;
		gameHandle >> isWhiteTurn;
		gameHandle >> minMoves;
		moveHistory.resize(minMoves);
		for (int i = 0; i < BOARDSIZE; ++i) {
			for (int j = 0; j < BOARDSIZE; ++j) {
				gameHandle >> board[i][j];
			}
		}

	}
	else {
		penteVariant = -1;
	}
	gameHandle.close();
}

void penteBoard::savePenteBoard(bool autosave)
{
	bool success = true;
	string pathToSave;
	if (!autosave) {
		cout << "Jak chcesz nazwac plik?" << endl;
		cin >> pathToSave;
	}
	else {
		pathToSave = "autosave";
	}
	pathToSave.append(".pnt");
	ofstream ofs(pathToSave, ofstream::trunc);
	if (!ofs.is_open()) {
		cout << "Nie udalo sie zapisac pliku gry." << endl;
		system("pause");
	}
	ofs << isPro << " " << penteVariant << " " << takesForWhite << " " << takesForBlack << " " << isWhiteTurn << " " << moveHistory.size() << endl;;
	for (int i = 0; i < BOARDSIZE; ++i) {
		for (int j = 0; j < BOARDSIZE; ++j) {
			ofs << board[i][j] << " ";
		}
		ofs << endl;
	}
	ofs.close();
}

bool penteBoard::checkIfMoveLegal(int x, int y)
{
	if (x < 0 || x >= BOARDSIZE || y < 0 || y >= BOARDSIZE) {
		return false;
	}
	if (board[y][x] != EMPTY) {
		return false;
	}
	return true;
}

int penteBoard::getMoveHistorySize() {
	return max((signed)moveHistory.size(), minMoves);
}

bool penteBoard::checkIfMoveLegalProPente(int x, int y)
{
	int boardMiddle = (int)((BOARDSIZE) / 2);
	if (x < 0 || x >= BOARDSIZE || y < 0 || y >= BOARDSIZE) {
		return false;
	}
	if (board[y][x] != EMPTY) {
		return false;
	}
	if (moveHistory.size() == 0) {
		if (x != boardMiddle || y != boardMiddle) {
			return false;
		}
		else {
			return true;
		}
	}
	if (moveHistory.size() == 2) {
		if (x < boardMiddle - 2 || x > boardMiddle + 2 || y < boardMiddle - 2 || y > boardMiddle + 2) {
			return true;
		}
		else {
			return false;
		}
	}
	return true;
}

bool penteBoard::makeMove(int x, int y)
{
	if (!isPro && !checkIfMoveLegal(x, y)) {
		cout << "Podany zostal nieprawidlowy ruch!" << x << ", " << y << endl;
		return false;
	}
	if (isPro && !checkIfMoveLegalProPente(x, y)) {
		cout << "Podany zostal nieprawidlowy ruch!" << x << ", " << y << endl;
		return false;
	}
	board[y][x] = isWhiteTurn ? WHITE : BLACK;

	//sprawdzanie czy gra zosta³a wygrana
	int dir[][2] = { { 0, 1 }, {0, -1}, {1, 0}, {-1, 0}, {-1, -1}, {1, 1 }, {-1, 1}, {1, -1} };
	for (int i = 0; i < 4; ++i) {
		int rowCount = 1;
		bool blockedFirstDir = false;
		bool blockedSecondDir = false;
		for (int j = 1; j < 5; ++j) {
			if (x + j * dir[i * 2][1] < 0 || x + j * dir[i * 2][1] >= BOARDSIZE || y + j * dir[i * 2][0] < 0 || y + j * dir[i * 2][0] >= BOARDSIZE || blockedFirstDir) {
				continue;
			}
			if (board[y + j * dir[i * 2][0]][x + j * dir[i * 2][1]] == (isWhiteTurn ? WHITE : BLACK)) {
				rowCount += 1;
			}
			else {
				blockedFirstDir = true;
			}
			
			if (x + j * dir[i * 2 + 1][1] < 0 || x + j * dir[i * 2 + 1][1] >= BOARDSIZE || y + j * dir[i * 2 + 1][0] < 0 || y + j * dir[i * 2 + 1][0] >= BOARDSIZE || blockedSecondDir) {
				continue;
			}
			if (board[y + j * dir[i * 2 + 1][0]][x + j * dir[i * 2 + 1][1]] == (isWhiteTurn ? WHITE : BLACK)) {
				rowCount += 1;
			}
			else {
				blockedSecondDir = true;
			}
			
		}
		if (rowCount >= 5) {
			gameWon = true;
			winner = isWhiteTurn ? WHITE : BLACK;
		}
	}

	//zbijanie pionkow przeciwnika i zapisywanie ruchu
	vector<coordinates> moveToSave;
	coordinates coordToSave;
	coordToSave.x = x;
	coordToSave.y = y;
	moveToSave.push_back(coordToSave);
	for (int i = 0; i < 8; ++i) {
		int enemies[8] = { 0 };
		bool canContinue[8] = { true, true, true, true, true, true, true, true };
		bool shouldRemove[8] = { false };
		for (int j = 1; j < 5; ++j) {
			if (x + j * dir[i][1] < 0 || x + j * dir[i][1] >= BOARDSIZE || y + j * dir[i][0] < 0 || y + j * dir[i][0] >= BOARDSIZE) {
				continue;
			}
			if (board[y + j * dir[i][0]][x + j * dir[i][1]] == (isWhiteTurn ? BLACK : WHITE) && canContinue[i]) {
				enemies[i]++;
			}
			else if (board[y + j * dir[i][0]][x + j * dir[i][1]] == (isWhiteTurn ? WHITE : BLACK) && canContinue[i]) {
				shouldRemove[i] = true;
				canContinue[i] = false;
			}
			else if (board[y + j * dir[i][0]][x + j * dir[i][1]] == EMPTY) {
				canContinue[i] = false;
			}
			
		}
		if (shouldRemove[i]) {
			switch (enemies[i]) {
			case 3:
				if (penteVariant != KERYOPENTE) {
					break;
				}
			case 2:
				for (int j = 1; j < 4; ++j) {
					if (board[y + j * dir[i][0]][x + j * dir[i][1]] == (isWhiteTurn ? BLACK : WHITE)) {
						board[y + j * dir[i][0]][x + j * dir[i][1]] = EMPTY;
						if (isWhiteTurn) takesForWhite++;
						else takesForBlack++;
						coordToSave.x = x + j * dir[i][1];
						coordToSave.y = y + j * dir[i][0];
						moveToSave.push_back(coordToSave);
					}
					else if (board[y + j * dir[i][0]][x + j * dir[i][1]] == (isWhiteTurn ? WHITE : BLACK)) {
						break;
					}
				}
				break;
			case 1:
				break;
			case 0:
				break;
			default:
				cout << "Nierozpoznany blad programu podczas zbijania : " << enemies[i] << endl;
			}
		}
	}

	switch (penteVariant) {
	case REGULARPENTE:
		if (takesForWhite >= 10 || takesForBlack >= 10) {
			gameWon = true;
			winner = isWhiteTurn ? WHITE : BLACK;
		}
		break;
	case KERYOPENTE:
		if (takesForWhite >= 15 || takesForBlack >= 15) {
			gameWon = true;
			winner = isWhiteTurn ? WHITE : BLACK;
		}
	}

	moveHistory.push_back(moveToSave);
	isWhiteTurn = !isWhiteTurn;
	return true;
}

void penteBoard::displayCredits()
{
	cout << "Gratulacje! " << endl << "Wygrywa  gracz: " << (isWhiteTurn ? "czarny!" : "bialy!") << endl;
	cout << "poprzez: ";
	if (penteVariant == KERYOPENTE && (takesForBlack >= 15 || takesForWhite >= 15)) {
		cout << "zbicie 15 pionkow przeciwnika." << endl;
	}
	else if (penteVariant == REGULARPENTE && (takesForBlack >= 10 || takesForWhite >= 10)) {
		cout << "zbicie 10 pionkow przeciwnika." << endl;
	}
	else {
		cout << "ulozenie 5 kamykow w linii." << endl;
	}
}

void penteBoard::unmakeMove()
{
	if (moveHistory.size() == 0 || (signed)moveHistory.size() <= minMoves) {
		cout << "brak ruchów do cofania" << endl;
		return;
	}
	board[moveHistory.back().front().y][moveHistory.back().front().x] = EMPTY;
	moveHistory.back().erase(moveHistory.back().begin());
	for (auto& move : moveHistory.back()) {
		board[move.y][move.x] = isWhiteTurn ? WHITE : BLACK;
	}
	moveHistory.pop_back();
	isWhiteTurn = !isWhiteTurn;
}

bool penteBoard::checkIfBoardFull()
{
	for (int i = 0; i < BOARDSIZE; ++i) {
		for (int j = 0; j < BOARDSIZE; ++j) {
			if (board[i][j] == EMPTY) return false;
		}
	}
	return true;
}

void UTF8Pente::printBoardToConsole()
{
	SetConsoleOutputCP(CP_UTF8);
	printf("%s", WHITECOLOUR);
	printf("  ");
	for (int i = 'a'; i < 't'; ++i) {
		printf("%c", i);
	}
	cout << endl;
	for (int i = 0; i < BOARDSIZE; ++i) {
		printf("%s", WHITECOLOUR);
		printf("%2d", BOARDSIZE - i);
		for (int j = 0; j < BOARDSIZE; ++j) {
			switch (board[i][j]) {
			case EMPTY:
				printf(" ");
				break;
			case WHITE:
				printf("%s", WHITECOLOUR);
				cout << u8"•";
				break;
			case BLACK:
				printf("%s", REDCOLOUR);
				cout << u8"•";
				break;
			default:
				system("cls");
				cout << "Niepoprawny kod znaku: " << board[i][j] << " znajduje sie na planszy." << endl;
			}
		}
		printf("%s", WHITECOLOUR);
		printf("%2d", BOARDSIZE - i);
		cout << endl;
	}
	printf("%s", WHITECOLOUR);
	printf("  ");
	for (int i = 'a'; i < 't'; ++i) {
		printf("%c", i);
	}
	cout << endl;
}

void ASCIIPente::printBoardToConsole()
{
	printf("%s", WHITECOLOUR);
	printf("  ");
	for (int i = 'a'; i < 't'; ++i) {
		printf("%c", i);
	}
	cout << endl;
	for (int i = 0; i < BOARDSIZE; ++i) {
		printf("%s", WHITECOLOUR);
		printf("%2d", BOARDSIZE - i);
		for (int j = 0; j < BOARDSIZE; ++j) {
			switch (board[i][j]) {
			case EMPTY:
				printf(" ");
				break;
			case WHITE:
				printf("%s", WHITECOLOUR);
				printf("o");
				break;
			case BLACK:
				printf("%s", REDCOLOUR);
				printf("o");
				break;
			default:
				system("cls");
				cout << "Niepoprawny kod znaku: " << board[i][j] << " znajduje sie na planszy." << endl;
			}
		}
		printf("%s", WHITECOLOUR);
		printf("%2d", BOARDSIZE - i);
		cout << endl;
	}
	printf("%s", WHITECOLOUR);
	printf("  ");
	for (int i = 'a'; i < 't'; ++i) {
		printf("%c", i);
	}
	cout << endl;
}
