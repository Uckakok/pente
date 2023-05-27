#include<iostream>
#include<fstream>
#include<string>
#include<filesystem>
#include"declarations.h"
#include<conio.h>
#include"game.h"
#include <bitset>
#include<Windows.h>

using namespace std;

coordinates gameChoices(settings *currentSettings) {
	bool success = false;
	char answer[4] = "";
	int coord1;
	int coord2;
	coordinates returnValue;
	do {
		char coordChar[3];
		cout << "Podaj ruch w postaci np 'a19', lubi wybierz opcje:" << endl;
		cout << "1 - zapisz gre" << endl;
		cout << "2 - wyjdz do menu" << endl;
		cout << "3 - wyjdz do windows" << endl;
		if (currentSettings->allowUndo) cout << "4 - cofnij poprzedni ruch" << endl;
		scanf_s("%s", answer, _countof(answer));
		if (isdigit(answer[0])) {
			switch (answer[0]) {
			case '1':
				returnValue.x = -1;
				return returnValue;
			case '2':
				returnValue.x = -2;
				return returnValue;
			case '3':
				returnValue.x = -3;
				return returnValue;
			case '4':
				returnValue.x = -4;
				return returnValue;
			}
		}
		else {
			success = true;
			coord1 = answer[0] - 96;
			coordChar[0] = answer[1];
			coordChar[1] = answer[2];
			coordChar[2] = '\0';
			if (sscanf_s(coordChar, "%d", &coord2) != 1) {
				success = false;
			}
			if (success) {
				if (coord2 < 1 || coord2 > BOARDSIZE || coord1 < 1 || coord1 > BOARDSIZE) {
					success = false;
				}
			}
		}
	} while (!success);
	returnValue.y = BOARDSIZE - coord2;
	returnValue.x = coord1 - 1;
	return returnValue;
}



void printPenteRules() {
	system("cls");
	cout << "Kazdy z dwoch graczy ma do dyspozycji kamienie: jeden koloru bialego, drugi -- czarnego, ktore ukladaja na przemian na wolnych polach planszy." << endl;
	cout << "Celem gry jest ulozenie pieciu lub wiecej kamieni swojego koloru w ciaglej linii -- poziomej, pionowej lub ukosnej -- badz zbicie 10 kamieni przeciwnika. Pierwszy gracz, ktory tego dokona, zostaje zwyciezca; jesli nie uda sie to nikomu (plansza zostanie zapelniona), nastepuje remis." << endl;
	cout << "Bicie kamieni przeciwnika nastepuje przez otoczenie pary jego sasiaduj¹cych kamieni z obu stron. Zbite kamienie sa usuwane z planszy, a pola, ktore okupowaly, moga byc ponownie wykorzystane w grze. W jednym ruchu mozna zbic wiecej niz jedna pare." << endl;
	cout << "Istnieje odmiana gry noszaca nazwe KERYO-PENTE, w ktorej dozwolone jest takze bicie trojek, nie tylko par. Zwyciezca zostaje gracz, ktory ulozy piec lub wiecej kamieni swojego koloru w ciaglej linii, albo zbije 15 kamieni przeciwnika." << endl;
	cout << "Zarowno pente jak i keryo-pente posiadaja swoje odmiany PRO, w ktorych wystêpuja nastepujace wymagania dotyczace poczatkowych ruchow na planszy:" << endl;
	cout << "* pierwszy ruch czarnych musi byc na srodku planszy" << endl;
	cout << "* pierwszy ruch bialych moze byc gdziekolwiek" << endl;
	cout << "* drugi ruch czarnych musi byc poza centralnym kwadratem 5x5" << endl;
	cout << "* pozostale ruchy zarowno czarnych jak i bialych moga byc gdziekolwiek" << endl << endl;
	cout << "uzyta instrukcja dostepna na: http://gomoku.5v.pl/index.php/zasady-pente" << endl;
	system("pause");
	return;
}


void listSavesInWorkingDirectory() {
	
	vector<string> entries;
	WIN32_FIND_DATA findData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);
	strcat_s(buffer, MAX_PATH, TEXT("\\*"));
	hFind = FindFirstFile(buffer, &findData);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(findData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)) {
				entries.push_back(findData.cFileName);
			}
		} while (FindNextFile(hFind, &findData) != 0);
		FindClose(hFind);
	}
	cout << endl << endl;
	for (auto & entry : entries) {
		if (entry.length() >= 4 && entry.substr(entry.length() - 4, entry.length()) == ".pnt") {
			cout << entry << endl;
		}
	}
}


int mainMenu() {
	do {
		system("cls");
		cout << "1 - rozpocznij nowa gre hot seat" << endl;
		cout << "2 - rozpocznij nowa gre vs komputer" << endl;
		cout << "3 - wczytaj gre" << endl;
		cout << "4 - ustawienia" << endl;
		cout << "5 - wyswielt zasady pente" << endl;
		cout << "6 - wyjscie z programu" << endl;
		char choice = _getch();
		choice -= '0';
		if (choice >= 1 && choice <= 6) return choice;
	} while (true);
}

vector<pieceToDraw> getAllPieces(penteBoard *currentGame) {
	vector<pieceToDraw> allPieces;
	for (int i = 0; i < BOARDSIZE; ++i) {
		for (int j = 0; j < BOARDSIZE; ++j) {
			if (currentGame->board[i][j] != EMPTY) {
				pieceToDraw temp;
				if (currentGame->board[i][j] == WHITE) {
					temp.colour = WHITE;
				}
				else {
					temp.colour = BLACK;
				}
				temp.x = i;
				temp.y = j;
				allPieces.push_back(temp);
			}
		}
	}
	return allPieces;
}


void gameLoop(penteBoard *currentGame, settings *currentSettings) {
	currentGame->AIInstance = generateAIInstance(currentSettings->AIDifficulty);
	graphicalInterface *window = NULL;
	if (!currentSettings->graphical) {
		currentGame->printBoardToConsole();
	}
	else {
		window = new graphicalInterface();
		window->blendEnable();
		window->prepareVertexArray();
		window->prepareVertexBuffer();
		window->prepareShaders();
		window->unbindStuff();
		window->setupCallbacks();
		window->setupMatrices();
	}
	vector<pieceToDraw> allPieces;
	while (true) {
		coordinates nextMove;
		if (currentGame->isAgainstAI && currentGame->isAIWhite == currentGame->isWhiteTurn) {
			nextMove = currentGame->AIInstance->findBestMove(currentGame, currentGame->isWhiteTurn);
			cout << "Ruch AI: " << (char)(nextMove.x + 97) << " " << nextMove.y + 1 << endl;
			currentGame->gameWon = false;
			currentGame->winner = -1;
		}
		else {
			if (!currentSettings->graphical) {
				nextMove = gameChoices(currentSettings);
			}
			else {
				allPieces = getAllPieces(currentGame);
				window->newPiecesToDraw(allPieces);
				window->windowUpdate();
				nextMove = fetchPosition();
			}
		}
		if (!currentSettings->graphical) {
			switch (nextMove.x) {
			case -1:
				currentGame->savePenteBoard();
				break;
			case -2:
				if (currentSettings->autosaveOnExit) {
					currentGame->savePenteBoard(true);
				}
				delete currentGame;
				return;
			case -3:
				if (currentSettings->autosaveOnExit) {
					currentGame->savePenteBoard(true);
				}
				delete currentGame;
				delete currentSettings;
				exit(EXIT_SUCCESS);
			case -4:
				if (currentSettings->allowUndo) {
					currentGame->unmakeMove();
				}
				break;
			default:
				currentGame->makeMove(nextMove.x, nextMove.y);
			}
			currentGame->printBoardToConsole();
			cout << "Zbicia dla gracza bialego: " << currentGame->takesForWhite << endl;
			cout << "Zbicia dla gracza czarnego: " << currentGame->takesForBlack << endl;
			if (currentGame->checkIfBoardFull()) {
				cout << "Plansza zostala zapelniona! remis." << endl;
				system("pause");
				delete currentGame;
				return;
			}
			if (currentGame->gameWon) {
				currentGame->displayCredits();
				system("pause");
				delete currentGame;
				break;
			}
		}
		else {
			switch (nextMove.x) {
			case -1:
				break;
			case -2: {
				window->resetPosition();
				window->closeWindow();
				char saveChoice;
				cout << "Czy chcesz zapisac gre? (t/n)" << endl;
				saveChoice = _getch();
				if (saveChoice == 't') {
					currentGame->savePenteBoard();
				}
				else {
					;
				}
				if (currentSettings->autosaveOnExit) {
					currentGame->savePenteBoard(true);
				}
				delete currentGame;
				delete window;
				return; }
			default:
				currentGame->makeMove(nextMove.x, nextMove.y);
				window->resetPosition();
				allPieces = getAllPieces(currentGame);
				window->newPiecesToDraw(allPieces);
				window->windowUpdate();
				currentGame->printBoardToConsole();
				cout << "Zbicia dla gracza bialego: " << currentGame->takesForWhite << endl;
				cout << "Zbicia dla gracza czarnego: " << currentGame->takesForBlack << endl;
			}
			if (currentGame->checkIfBoardFull()) {
				cout << "Plansza zostala zapelniona! remis." << endl;
				system("pause");
				delete currentGame;
				return;
			}
			if (currentGame->gameWon) {
				allPieces = getAllPieces(currentGame);
				window->newPiecesToDraw(allPieces);
				window->windowUpdate();
				currentGame->displayCredits();
				while (true) {
					if (window->shouldCloseWindow()) break;
					window->windowUpdate();
				}
				window->closeWindow();
				system("pause");
				delete currentGame;
				break;
			}
		}
	}
}

penteBoard* createBoard(settings *currentSettings) {
	penteBoard *currentGame;
	if (currentSettings->prefferedConsole == ASCIICONSOLE) {
		currentGame = new ASCIIPente();
	}
	else {
		currentGame = new UTF8Pente();
	}
	return currentGame;
}

void initalizeBoard(settings *currentSettings, penteBoard* currentBoard) {
	currentBoard->isPro = currentSettings->isPro;
	currentBoard->penteVariant = currentSettings->prefferedPenteVersion;
}

int startGame() {
	srand((unsigned)time(NULL));
	settings *currentSettings = new settings();
	while (true) {
		string saveName;
		penteBoard *currentGame;
		int whatToDo = mainMenu();
		switch (whatToDo) {
		case 1:
			currentGame = createBoard(currentSettings);
			initalizeBoard(currentSettings, currentGame);
			gameLoop(currentGame, currentSettings);
			break;
		case 2: {
			char playerColour;
			cout << "Jako ktory gracz chcesz zagrac? (b/c)" << endl;
			playerColour = _getch();
			if (playerColour == 'b') {
				currentGame = createBoard(currentSettings);
				initalizeBoard(currentSettings, currentGame);
				currentGame->isAIWhite = false;
				currentGame->isAgainstAI = true;
				gameLoop(currentGame, currentSettings);
			}
			else if (playerColour == 'c') {
				currentGame = createBoard(currentSettings);
				initalizeBoard(currentSettings, currentGame);
				currentGame->isAIWhite = true;
				currentGame->isAgainstAI = true;
				gameLoop(currentGame, currentSettings);
			}
			break; }
		case 3: {
			listSavesInWorkingDirectory();
			cout << endl << "Podaj nazwe pliku, ktory chcesz wczytac:" << endl;
			cin >> saveName;
			if (currentSettings->prefferedConsole == ASCIICONSOLE) {
				currentGame = new ASCIIPente(saveName);
			}
			else {
				currentGame = new UTF8Pente(saveName);
			}
			if (currentGame->penteVariant == -1) {
				cout << "Nie mozna bylo wczytac gry: " << saveName << endl;
				system("pause");
				break;
			}
			else {
				currentSettings->prefferedPenteVersion = currentGame->penteVariant;
			}
			cout << "Chcesz zagrac w trybie hotseat, czy przeciwko AI? (h/a)" << endl;
			char gameEnemy;
			gameEnemy = _getch();
			if (gameEnemy == 'h') {
				cout << endl << "Aktualnie ruch gracza: " << (currentGame->isWhiteTurn ? "bialy" : "czarny") << endl;
				gameLoop(currentGame, currentSettings);
			}
			else if (gameEnemy == 'a') {
				char playerColour;
				cout << "Jako ktory gracz chcesz zagrac? (b/c)" << endl;
				playerColour = _getch();
				if (playerColour == 'b') {
					currentGame->isAIWhite = false;
					currentGame->isAgainstAI = true;
					gameLoop(currentGame, currentSettings);
				}
				else if (playerColour == 'c') {
					currentGame->isAIWhite = true;
					currentGame->isAgainstAI = true;
					gameLoop(currentGame, currentSettings);
				}
				else {
					cout << "Niepoprawne dane" << endl;
				}
			}
			else {
				cout << "Niepoprawne dane" << endl;
			}
			break; }
		case 4:
			currentSettings->changeSettings();
			break;
		case 5:
			printPenteRules();
			break;
		case 6:
			cout << "dziekuje za gre!" << endl;
			system("pause");
			exit(EXIT_SUCCESS);
			break;
		}
	}
}