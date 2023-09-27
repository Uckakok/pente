#include<iostream>
#include<fstream>
#include<string>
#include<filesystem>
#include"declarations.h"
#include<conio.h>
#include"game.h"
#include<Windows.h>
#include"networking.h"


using namespace std;

//w trybie konsolowym opcje gry
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
		cout << "4 - przelacz na tryb graficzny" << endl;
		if (currentSettings->allowUndo) cout << "5 - cofnij poprzedni ruch" << endl;
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
			case '5':
				returnValue.x = -5;
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
		while (getchar() != '\n') {
			;
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
	cout << "Bicie kamieni przeciwnika nastepuje przez otoczenie pary jego sasiadujacych kamieni z obu stron. Zbite kamienie sa usuwane z planszy, a pola, ktore okupowaly, moga byc ponownie wykorzystane w grze. W jednym ruchu mozna zbic wiecej niz jedna pare." << endl;
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

//wypisuje wszystkie pliki z rozszerzeniem .pnt w obecnym katalogu
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
		cout << "4 - gra online" << endl;
		cout << "5 - ustawienia" << endl;
		cout << "6 - wyswielt zasady pente" << endl;
		cout << "7 - wyjscie z programu" << endl;
		char choice = _getch();
		choice -= '0';
		if (choice >= 1 && choice <= 7) return choice;
	} while (true);
}

int onlineMenu() {
	do {
		system("cls");
		cout << "1 - hostuj gre LAN." << endl;
		cout << "2 - dolacz do gry LAN." << endl;
		cout << "3 - powrot" << endl;
		cout << endl << endl << endl << "Funkcja eksperymentalna. Gracz bedacy zawsze zaczyna grajac czarnymi i program slabo obsluguje bledy w ";
		cout << "polaczeniu, wiec w przypadku utraty polaczenia gra bedzie wiecznie czekala na odpowiedz drugiej strony. W takim wypadku trzeba gre wylaczyc ";
		cout << "i uruchomic ponownie." << endl;
		char choice = _getch();
		choice -= '0';
		if (choice >= 1 && choice <= 3) return choice;
	} while (true);
}

//tworzy wektor ze wszystkimi bierkami na planszy, by okno mog³o je narysowaæ
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

//zwracane wartosc 0 -ruch wykonany, 1 - kontynuuj gre, 2 - wyjscie z gry, 3 - koniec gry
int handleGameEvents(coordinates nextMove, penteBoard *currentGame, settings *currentSettings, graphicalInterface*& window) {
	vector<pieceToDraw> allPieces;
	if (nextMove.x == -6) {
		return 1;
	}

	if (!currentSettings->graphical) {
		switch (nextMove.x) {
		case -1:
			currentGame->savePenteBoard();
			return 1;
		case -2:
			if (currentSettings->autosaveOnExit) {
				currentGame->savePenteBoard(true);
			}
			delete currentGame;
			return 2;
		case -3:
			if (currentSettings->autosaveOnExit) {
				currentGame->savePenteBoard(true);
			}
			delete currentGame;
			delete currentSettings;
			exit(EXIT_SUCCESS);
		case -4:
			currentSettings->graphical = true;
			window = new graphicalInterface();
			window->blendEnable();
			window->prepareVertexArray();
			window->prepareVertexBuffer();
			window->prepareShaders();
			window->unbindStuff();
			window->setupCallbacks();
			window->setupMatrices();
			allPieces = getAllPieces(currentGame);
			window->newPiecesToDraw(allPieces);
			window->windowUpdate();
			return 1;
		case -5:
			if (currentGame->online) {
				cout << "Cofanie ruchow niejest mozliwe w grze online." << endl;
				return 1;
			}
			if (currentSettings->allowUndo) {
				if (currentGame->isAgainstAI) {
					currentGame->unmakeMove();
				}
				currentGame->unmakeMove();
			}
			return 1;
		default:
			if (!currentGame->makeMove(nextMove.x, nextMove.y)) {
				currentGame->printBoardToConsole();
				return 1;
			}
		}
		currentGame->printBoardToConsole();
		cout << "Zbicia dla gracza bialego: " << currentGame->takesForWhite << endl;
		cout << "Zbicia dla gracza czarnego: " << currentGame->takesForBlack << endl;
		if (currentGame->checkIfBoardFull()) {
			cout << "Plansza zostala zapelniona! remis." << endl;
			system("pause");
			delete currentGame;
			return 3;
		}
		else if (currentGame->gameWon) {
			currentGame->displayCredits();
			system("pause");
			delete currentGame;
			return 3;
		}
		else {
			return 0;
		}
	}
	else {
		switch (nextMove.x) {
		case -1:
			return 1;
		case -2:
			currentSettings->graphical = false;
			window->resetPosition();
			window->closeWindow();
			currentGame->printBoardToConsole();
			delete window;
			return 1;
		default:
			if (!currentGame->makeMove(nextMove.x, nextMove.y)) {
				currentGame->printBoardToConsole();
				return 1;
			}
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
			allPieces = getAllPieces(currentGame);
			window->newPiecesToDraw(allPieces);
			window->windowUpdate();
			MessageBox(NULL, "Gra zakonczona!", "Pente!", MB_ICONINFORMATION | MB_OK);
			while (true) {
				if (window->shouldCloseWindow()) break;
				window->windowUpdate();
			}
			window->closeWindow();
			system("pause");
			delete currentGame;
			return 3;
		}
		else if (currentGame->gameWon) {
			allPieces = getAllPieces(currentGame);
			window->newPiecesToDraw(allPieces);
			window->windowUpdate();
			currentGame->displayCredits();
			MessageBox(NULL, "Gra zakonczona!", "Pente!", MB_ICONINFORMATION | MB_OK);
			while (true) {
				if (window->shouldCloseWindow()) break;
				window->windowUpdate();
			}
			window->closeWindow();
			system("pause");
			delete currentGame;
			return 3;
		}
		else {
			return 0;
		}
	}
}

void gameLoop(penteBoard *currentGame, settings *currentSettings, SOCKET enemySocket) {
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
	currentGame->printBoardToConsole();
	while (true) {
		int eventAnswer = -1;
		coordinates nextMove;
		if (currentGame->isAgainstAI && currentGame->isAIWhite == currentGame->isWhiteTurn) {
			nextMove = currentGame->AIInstance->findBestMove(currentGame, currentGame->isWhiteTurn);
			cout << "Ruch AI: " << (char)(nextMove.x + 97) << BOARDSIZE - (nextMove.y) << endl;
			currentGame->gameWon = false;
			currentGame->winner = -1;
			eventAnswer = handleGameEvents(nextMove, currentGame, currentSettings, window);
			switch (eventAnswer) {
			case 0:
				break;
			case 1:
				break;
			case 2:
				return;
			case 3:
				return;
			}
		}
		else {
			if (currentGame->online) {
				if ((currentGame->isHost && !currentGame->isWhiteTurn) || (!currentGame->isHost && currentGame->isWhiteTurn)) {
					do {
						nextMove = getNextMove(currentSettings, window, currentGame);
					} while (nextMove.x == -6);
					eventAnswer = handleGameEvents(nextMove, currentGame, currentSettings, window);
					switch (eventAnswer) {
					case 0:
						if (send(enemySocket, (char*)&nextMove, sizeof(nextMove), 0) <= 0) {
							cout << "Blad przy przesylaniu ruchu do przeciwnika." << endl;
							system("pause");
							return;
						}
						break;
					case 1:
						break;
					case 2:
						nextMove.x = -1;
						if (send(enemySocket, (char*)&nextMove, sizeof(nextMove), 0) <= 0) {
							cout << "Blad przy wysylaniu przeciwnikowi informacji o koncu gry." << endl;
							system("pause");
							return;
						}
						cout << "Wychodzenie z gry..." << endl;
						system("pause");
						return;
					case 3:
						if (send(enemySocket, (char*)&nextMove, sizeof(nextMove), 0) <= 0) {
							cout << "Blad przy wysylaniu ruchu przeciwnikowi." << endl;
							system("pause");
							return;
						}
						return;
					}
				}
				else {
					int recvReturn;
					while ((recvReturn = recv(enemySocket, (char*)&nextMove, sizeof(nextMove), 0)) <= 0) {
						if (recvReturn < 0) {
							cout << "Blad polaczenia." << endl;
							system("pause");
							return;
						}
						if (currentSettings->graphical) {
							window->windowUpdate();
						}
					}
					if (nextMove.x == -1) {
						cout << "Przeciwnik rozlaczyl sie z gry." << endl;
						system("pause");
						return;
					}
					eventAnswer = handleGameEvents(nextMove, currentGame, currentSettings, window);
					switch (eventAnswer) {
					case 0:
						break;
					case 1:
						cout << "Nastapila desynchronizacja klienta i hosta. Wychodzenie z gry..." << endl;
						system("pause");
						return;
					case 2:
						cout << "Gra zostala zakonczona!" << endl;
						system("pause");
						return;
					case 3:
						cout << "Gra zostala zakonczona!" << endl;
						system("pause");
						return;
					}
				}
			}
			else {
				nextMove = getNextMove(currentSettings, window, currentGame);
				eventAnswer = handleGameEvents(nextMove, currentGame, currentSettings, window);
				switch (eventAnswer) {
				case 0:
					break;
				case 1:
					break;
				case 2:
					return;
				case 3:
					return;
				}
			}
		}
	}
}

coordinates getNextMove(settings *currentSettings, graphicalInterface *window, penteBoard *currentGame) {
	coordinates nextMove;
	vector<pieceToDraw> allPieces;
	if (!currentSettings->graphical) {
		nextMove = gameChoices(currentSettings);
	}
	else {
		allPieces = getAllPieces(currentGame);
		window->newPiecesToDraw(allPieces);
		window->windowUpdate();
		nextMove = fetchPosition();
		window->resetPosition();
		if (nextMove.x == -1) {
			nextMove.x = -6;
		}
	}
	return nextMove;
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
			gameLoop(currentGame, currentSettings, NULL);
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
				gameLoop(currentGame, currentSettings, NULL);
			}
			else if (playerColour == 'c') {
				currentGame = createBoard(currentSettings);
				initalizeBoard(currentSettings, currentGame);
				currentGame->isAIWhite = true;
				currentGame->isAgainstAI = true;
				gameLoop(currentGame, currentSettings, NULL);
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
				gameLoop(currentGame, currentSettings, NULL);
			}
			else if (gameEnemy == 'a') {
				char playerColour;
				cout << "Jako ktory gracz chcesz zagrac? (b/c)" << endl;
				playerColour = _getch();
				if (playerColour == 'b') {
					currentGame->isAIWhite = false;
					currentGame->isAgainstAI = true;
					gameLoop(currentGame, currentSettings, NULL);
				}
				else if (playerColour == 'c') {
					currentGame->isAIWhite = true;
					currentGame->isAgainstAI = true;
					gameLoop(currentGame, currentSettings, NULL);
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
		{
			int onlineChoice = onlineMenu();
			switch (onlineChoice) {
			case 1: {
				currentGame = new penteBoard(); //inicjalizowanie by kompilator nie narzeka³
				SOCKET clientSocket = INVALID_SOCKET;
				SOCKET serverSocket = INVALID_SOCKET;
				if (runHost(currentGame, currentSettings, &serverSocket, &clientSocket)) {
					gameLoop(currentGame, currentSettings, clientSocket);
				}
				closesocket(serverSocket);
				closesocket(clientSocket);
				WSACleanup();
				break; 
			}
			case 2: {
				currentGame = new penteBoard(); //inicjalizowanie by kompilator nie narzeka³
				SOCKET clientSocket = INVALID_SOCKET;
				if (runClient(currentGame, currentSettings, &clientSocket)) {
					gameLoop(currentGame, currentSettings, clientSocket);
				}
				closesocket(clientSocket);
				WSACleanup();
				break;
			}
			case 3:
				break;
			}
			break;
		}
		case 5:
			currentSettings->changeSettings();
			break;
		case 6:
			printPenteRules();
			break;
		case 7:
			cout << "dziekuje za gre!" << endl;
			system("pause");
			exit(EXIT_SUCCESS);
		}
	}
}