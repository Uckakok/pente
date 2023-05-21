#include<iostream>
#include<vector>
#include<Windows.h>
#include<cstdio>
#include<fstream>
#include<string>
#include<filesystem>
#include"declarations.h"
#include"penteBoard.cpp"
#include"settings.cpp"
#include"graphicalMode.h"

namespace fs = std::experimental::filesystem;
using namespace std;

coordinates gameChoices() {
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
	std::system("cls");
	cout << "Kazdy z dwoch graczy ma do dyspozycji kamienie: jeden koloru bialego, drugi -- czarnego, ktore ukladaja na przemian na wolnych polach planszy." << endl;
	cout << "Celem gry jest ulozenie pieciu lub wiecej kamieni swojego koloru w ciaglej linii -- poziomej, pionowej lub ukosnej -- badz zbicie 10 kamieni przeciwnika. Pierwszy gracz, ktory tego dokona, zostaje zwyciezca; jesli nie uda sie to nikomu (plansza zostanie zapelniona), nastepuje remis." << endl;
	cout << "Bicie kamieni przeciwnika nastepuje przez otoczenie pary jego sasiadujących kamieni z obu stron. Zbite kamienie sa usuwane z planszy, a pola, ktore okupowaly, moga byc ponownie wykorzystane w grze. W jednym ruchu mozna zbic wiecej niz jedna pare." << endl;
	cout << "Istnieje odmiana gry noszaca nazwe KERYO-PENTE, w ktorej dozwolone jest takze bicie trojek, nie tylko par. Zwyciezca zostaje gracz, ktory ulozy piec lub wiecej kamieni swojego koloru w ciaglej linii, albo zbije 15 kamieni przeciwnika." << endl;
	cout << "Zarowno pente jak i keryo-pente posiadaja swoje odmiany PRO, w ktorych występuja nastepujace wymagania dotyczace poczatkowych ruchow na planszy:" << endl;
	cout << "* pierwszy ruch czarnych musi byc na srodku planszy" << endl;
	cout << "* pierwszy ruch bialych moze byc gdziekolwiek" << endl;
	cout << "* drugi ruch czarnych musi byc poza centralnym kwadratem 5x5" << endl;
	cout << "* pozostale ruchy zarowno czarnych jak i bialych moga być gdziekolwiek" << endl << endl;
	cout << "uzyta instrukcja dostepna na: http://gomoku.5v.pl/index.php/zasady-pente" << endl;
	std::system("pause");
	return;
}

void listSavesInWorkingDirectory() {
	vector<string> entries;
	for (fs::path path : fs::directory_iterator(fs::current_path())) {;
		string path_string{ path.string() };
		entries.push_back(path_string);
	}
	fs::path path{ fs::current_path() };
	string path_string{ path.string() };
	cout << endl << endl;
	for (auto & entry : entries) {
		entry.erase(0, path_string.length() + 1);
		if (entry.length() >= 4 && entry.substr(entry.length() - 4, entry.length()) == ".pnt") {
			cout << entry << endl;
		}
	}
}


int mainMenu() {
	int choice;
	std::system("cls");
	cout << "1 - rozpocznij nowa gre hot seat" << endl;
	cout << "2 - rozpocznij nowa gre vs komputer" << endl;
	cout << "3 - wczytaj gre" << endl;
	cout << "4 - ustawienia" << endl;
	cout << "5 - wyswielt zasady pente" << endl;
	cout << "6 - wyjscie z programu" << endl;
	while (!(cin >> choice)) {
		std::system("cls");
		cout << "1 - rozpocznij nowa gre hot seat" << endl;
		cout << "2 - rozpocznij nowa gre vs komputer" << endl;
		cout << "3 - wczytaj gre" << endl;
		cout << "4 - ustawienia" << endl;
		cout << "5 - wyswielt zasady pente" << endl;
		cout << "6 - wyjscie z programu" << endl;
		cin.clear();
		cin.ignore(1000, '\n');
	}
	return choice;
}

void gameLoop(penteBoard *currentGame, settings *currentSettings) {
	if (currentSettings->prefferedConsole == ASCIICONSOLE) {
		currentGame->printBoardToConsoleASCII();
	}
	else if (currentSettings->prefferedConsole == UTF8CONSOLE) {
		currentGame->printBoardToConsoleUTF8();
	}
	while (true) {
		coordinates nextMove;
		nextMove = gameChoices();
		if (nextMove.x == -1) {
			currentGame->savePenteBoard();
		}
		else if (nextMove.x == -2) {
			if (currentSettings->autosaveOnExit) {
				currentGame->savePenteBoard(true);
			}
			delete currentGame;
			break;
		}
		else if (nextMove.x == -3) {
			if (currentSettings->autosaveOnExit) {
				currentGame->savePenteBoard(true);
			}
			delete currentGame;
			delete currentSettings;
			std::exit(EXIT_SUCCESS);
		}
		else {
			currentGame->makeMove(nextMove.x, nextMove.y);
		}
		if (currentSettings->prefferedConsole == ASCIICONSOLE) {
			currentGame->printBoardToConsoleASCII();
		}
		else if (currentSettings->prefferedConsole == UTF8CONSOLE) {
			currentGame->printBoardToConsoleUTF8();
		}
		if (currentGame->gameWon) {
			currentGame->displayCredits();
			std::system("pause");
			delete currentGame;
			break;
		}
	}
}



int main() {
	testWindow();
	settings *currentSettings = new settings();
	while (true) {
		string saveName;
		penteBoard *currentGame;
		int whatToDo = mainMenu();
		switch (whatToDo) {
		case 1:
			currentGame = new penteBoard();
			currentGame->penteVariant = currentSettings->prefferedPenteVersion;
			gameLoop(currentGame, currentSettings);
			break;
		case 2:
			//dodac gracza AI
			cout << "Poki co brak komputera z ktorym mozna zagrac..." << endl;
			break;
		case 3:
			listSavesInWorkingDirectory();
			cout << endl << "Podaj nazwe pliku, ktory chcesz wczytac:" << endl;
			cin >> saveName;
			currentGame = new penteBoard(saveName);
			if (currentGame->penteVariant == -1) {
				cout << "Nie mozna bylo wczytac gry: " << saveName << endl;
				system("pause");
				break;
			}
			else {
				currentSettings->prefferedPenteVersion = currentGame->penteVariant;
			}
			cout << endl << "Aktualnie ruch gracza: " << (currentGame->isWhiteTurn ? "bialy" : "czarny") << endl;
			gameLoop(currentGame, currentSettings);
			break;
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