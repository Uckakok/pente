#include "settings.h"
#include"declarations.h"
#include<iostream>
#include<fstream>
#include<string>
#include <iomanip>
#include<stdio.h>
#include <conio.h>
#include<sstream>

using namespace std;


settings::settings() : prefferedConsole(UTF8CONSOLE), prefferedPenteVersion(REGULARPENTE), autosaveOnExit(true), isPro(false), graphical(false), allowUndo(false), AIDifficulty(2)
{
	ifstream settingsHandle("settings.txt");
	if (settingsHandle.is_open()) {
		string line;
		bool validSettings = true;  //flaga, notuj¹ca czy ustawienia s¹ poprawne

		while (getline(settingsHandle, line)) {
			istringstream iss(line);
			string key, value;
			if (iss >> key >> value) {
				if (key == "penteVersion:") {
					if (value == "regularPente") {
						prefferedPenteVersion = REGULARPENTE;
					}
					else if (value == "keryoPente") {
						prefferedPenteVersion = KERYOPENTE;
					}
					else {
						validSettings = false;  // nieprawidlowa wartosc
						break;
					}
				}
				else if (key == "console:") {
					if (value == "ASCII") {
						prefferedConsole = ASCIICONSOLE;
					}
					else if (value == "UTF8") {
						prefferedConsole = UTF8CONSOLE;
					}
					else {
						validSettings = false;  // Nieprawid³owa wartoœæ
						break;
					}
				}
				else if (key == "autosave:") {
					autosaveOnExit = (value == "true");
				}
				else if (key == "pro:") {
					isPro = (value == "true");
				}
				else if (key == "graphical:") {
					graphical = (value == "true");
				}
				else if (key == "undo:") {
					allowUndo = (value == "true");
				}
				else if (key == "AI:") {
					try {
						AIDifficulty = stoi(value);
					}
					catch (const std::invalid_argument&) {
						validSettings = false;  //Nieprawid³owa wartoœæ
						break;
					}
				}
				else {
					validSettings = false;  // nierozpoznana sekcja ustawieñ
					break;
				}
			}
			else {
				validSettings = false;  
				break;
			}
		}

		if (!validSettings) {
			prefferedConsole = UTF8CONSOLE;
			prefferedPenteVersion = REGULARPENTE;
			autosaveOnExit = true; 
			isPro = false;
			graphical = false;
			allowUndo = false;
			AIDifficulty = 2;
		}

		settingsHandle.close();
	}
	else {
		prefferedConsole = UTF8CONSOLE;
		prefferedPenteVersion = REGULARPENTE;
		autosaveOnExit = true;
		isPro = false;
		graphical = false;
		allowUndo = false;
		AIDifficulty = 2;
	}
}

bool settings::saveCurrentSettings()
{
	bool success = true;
	ofstream ofs("settings.txt", ofstream::trunc);
	ofs << "penteVersion: ";
	if (prefferedPenteVersion == REGULARPENTE) {
		ofs << "regularPente" << endl;
	}
	else if (prefferedPenteVersion == KERYOPENTE) {
		ofs << "keryoPente" << endl;
	}
	else {
		success = false;
	}
	ofs << "console: ";
	if (prefferedConsole == ASCIICONSOLE) {
		ofs << "ASCII" << endl;
	}
	else if (prefferedConsole == UTF8CONSOLE) {
		ofs << "UTF8" << endl;
	}
	else {
		success = false;
	}
	ofs << "autosave: ";
	if (autosaveOnExit) {
		ofs << "true" << endl;
	}
	else if (!autosaveOnExit) {
		ofs << "false" << endl;
	}
	else {
		success = false;
	}
	ofs << "pro: ";
	if (isPro) {
		ofs << "true" << endl;
	}
	else if (!isPro) {
		ofs << "false" << endl;
	}
	else {
		success = false;
	}
	ofs << "graphical: ";
	if (graphical) {
		ofs << "true" << endl;
	}
	else if (!graphical) {
		ofs << "false" << endl;
	}
	else {
		success = false;
	}
	ofs << "undo: ";
	if (allowUndo) {
		ofs << "true" << endl;
	}
	else if (!allowUndo) {
		ofs << "false" << endl;
	}
	else {
		success = false;
	}
	ofs << "AI: ";
	ofs << AIDifficulty;
	ofs.close();
	if (success) {
		return true;
	}
	else {
		ofstream ofs("settings.txt", ofstream::trunc);
		ofs.close();
		return false;
	}
}

void settings::changeSettings()
{
	bool repeat = true;
	do {
		system("cls");
		repeat = true;
		cout << left << setw(50) << "1 - wersja pente. Aktualny wybor: " << REDCOLOUR;
		if (prefferedPenteVersion == REGULARPENTE) {
			cout << "zwyczajne pente" << endl;
		}
		else if (prefferedPenteVersion == KERYOPENTE) {
			cout << "keryo-pente" << endl;
		}
		else {
			cout << "nieznany rodzaj pente." << endl;
		}
		cout << WHITECOLOUR << setw(50) << "2 - rodzaj konsoli. Aktualny wybor: " << REDCOLOUR;
		if (prefferedConsole == ASCIICONSOLE) {
			cout << "ASCII" << endl;
		}
		else if (prefferedConsole == UTF8CONSOLE) {
			cout << "UTF-8" << endl;
		}
		else {
			cout << "nieznany rodzaj konsoli" << endl;
		}
		cout << WHITECOLOUR << setw(50) << "3 - autozapis gry. Aktualny wybor: " << REDCOLOUR << (autosaveOnExit ? "tak" : "nie") << endl;
		cout << WHITECOLOUR << setw(50) << "4 - wersja pro. Aktualny wybor: " << REDCOLOUR << (isPro ? "tak" : "nie") << endl;
		cout << WHITECOLOUR << setw(50) << "5 - wersja graficzna. Aktualny wybor: " << REDCOLOUR << (graphical ? "tak" : "nie") << endl;
		cout << WHITECOLOUR << setw(50) << "6 - zezwol na cofanie ruchow. Aktualny wybor: " << REDCOLOUR << (allowUndo ? "tak" : "nie") << endl;
		cout << WHITECOLOUR << setw(50) << "7 - Poziom trudnosci AI. Aktualny wybor: " << REDCOLOUR;
		switch (AIDifficulty) {
		case -1:
			cout << "losowy";
			break;
		case 0:
			cout << "latwy";
			break;
		case 1:
			cout << "sredni";
			break;
		case 2:
			cout << "zaawansowany";
			break;
		case 3:
			cout << "ekspercki";
			break;
		case 4:
			cout << "mistrzowski";
			break;
		default :
			cout << "nierozpoznany";
		}
		cout << endl;
		cout << WHITECOLOUR << setw(50) << "8 - zapisz ustawienia na dysku  " << endl;
		cout << setw(50) << "9 - powrot  " << endl << endl << endl << endl << endl;
		cout << "na niektorych urzadzeniach znaki UTF moga nie wyswietlac sie poprawnie, oraz tryb graficzny moze nie dzialac." << endl;
		if (AIDifficulty == 4) {
			cout << "UWAGA poziom mistrzowski AI moze dlugo generowac ruchy! Proponuje zmienic konfiguracje z DEBUG na Release by to przyspieszyc." << endl;
		}
		char choice;
		choice = _getch();
		choice -= '0';
		switch (choice) {
		case 1:
			if (prefferedPenteVersion == REGULARPENTE) {
				prefferedPenteVersion = KERYOPENTE;
			}
			else {
				prefferedPenteVersion = REGULARPENTE;
			}
			break;
		case 2:
			if (prefferedConsole == UTF8CONSOLE) {
				prefferedConsole = ASCIICONSOLE;
			}
			else {
				prefferedConsole = UTF8CONSOLE;
			}
			break;
		case 3:
			autosaveOnExit = !autosaveOnExit;
			break;
		case 4:
			isPro = !isPro;
			break;
		case 5:
			graphical = !graphical;
			break;
		case 6:
			allowUndo = !allowUndo;
			break;
		case 7:
			AIDifficulty++;
			if (AIDifficulty > 4) AIDifficulty = -1;
			break;
		case 8:
			saveCurrentSettings();
			cout << endl << endl << "Zapisano nowe ustawienia!" << endl;
			system("pause");
			break;
		case 9:
			repeat = false;
			break;
		default:
			repeat = true;
		}
	} while (repeat);
}
