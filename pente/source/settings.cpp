#include "settings.h"

settings::settings()
{
	ifstream settingsHandle;
	bool possibleToRead = true;
	string version;
	int versionInt;
	int consoleInt;
	bool graphicalBool;
	bool proBool;
	bool autosaveBool;
	bool undoBool;
	settingsHandle.open("settings.txt");
	if (settingsHandle.is_open()) {
		string console;
		string autosave;
		string pro;
		string undo;
		string graphicalStr;
		string word;
		if (!(settingsHandle >> word)) {
			possibleToRead = false;
		}
		if (word != "penteVersion:") {
			possibleToRead = false;
		}
		if (!(settingsHandle >> version)) {
			possibleToRead = false;
		}
		if (!(settingsHandle >> word)) {
			possibleToRead = false;
		}
		if (word != "console:") {
			possibleToRead = false;
		}
		if (!(settingsHandle >> console)) {
			possibleToRead = false;
		}
		if (version == "regularPente") {
			versionInt = REGULARPENTE;
		}
		else if (version == "keryoPente") {
			versionInt = KERYOPENTE;
		}
		else {
			possibleToRead = false;
		}
		if (console == "ASCII") {
			consoleInt = ASCIICONSOLE;
		}
		else if (console == "UTF8") {
			consoleInt = UTF8CONSOLE;
		}
		else {
			possibleToRead = false;
		}
		if (!(settingsHandle >> word)) {
			possibleToRead = false;
		}
		if (word != "autosave:") {
			possibleToRead = false;
		}
		if (!(settingsHandle >> autosave)) {
			possibleToRead = false;
		}
		if (autosave == "true") {
			autosaveBool = true;
		}
		else if (autosave == "false") {
			autosaveBool = false;
		}
		else {
			possibleToRead = false;
		}
		if (!(settingsHandle >> word)) {
			possibleToRead = false;
		}
		if (word != "pro:") {
			possibleToRead = false;
		}
		if (!(settingsHandle >> pro)) {
			possibleToRead = false;
		}
		if (pro == "true") {
			proBool = true;
		}
		else if (pro == "false") {
			proBool = false;
		}
		else {
			possibleToRead = false;
		}
		if (!(settingsHandle >> word)) {
			possibleToRead = false;
		}
		if (word != "graphical:") {
			possibleToRead = false;
		}
		if (!(settingsHandle >> graphicalStr)) {
			possibleToRead = false;
		}
		if (graphicalStr == "true") {
			graphicalBool = true;;
		}
		else if (graphicalStr == "false") {
			graphicalBool = false;
		}
		else {
			possibleToRead = false;
		}
		if (!(settingsHandle >> word)) {
			possibleToRead = false;
		}
		if (word != "undo:") {
			possibleToRead = false;
		}
		if (!(settingsHandle >> undo)) {
			possibleToRead = false;
		}
		if (undo == "true") {
			undoBool = true;;
		}
		else if (undo == "false") {
			undoBool = false;
		}
		else {
			possibleToRead = false;
		}
		settingsHandle.close();
		if (possibleToRead) {
			prefferedConsole = consoleInt;
			prefferedPenteVersion = versionInt;
			autosaveOnExit = autosaveBool;
			isPro = proBool;
			graphical = graphicalBool;
			allowUndo = undoBool;
			return;
		}
	}
	prefferedConsole = UTF8CONSOLE;
	prefferedPenteVersion = REGULARPENTE;
	autosaveOnExit = true;
	isPro = false;
	graphical = false;
	allowUndo = false;
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
		cout << "1 - wersja pente. Aktualny wybor: ";
		if (prefferedPenteVersion == REGULARPENTE) {
			cout << "zwyczajne pente" << endl;
		}
		else if (prefferedPenteVersion == KERYOPENTE) {
			cout << "keryo-pente" << endl;
		}
		else {
			cout << "nieznany rodzaj pente." << endl;
		}
		cout << "2 - rodzaj konsoli. Aktualny wybor: ";
		if (prefferedConsole == ASCIICONSOLE) {
			cout << "ASCII" << endl;
		}
		else if (prefferedConsole == UTF8CONSOLE) {
			cout << "UTF-8" << endl;
		}
		else {
			cout << "nieznany rodzaj konsoli" << endl;
		}
		cout << "3 - autozapis gry. Aktualny wybor: " << (autosaveOnExit ? "tak" : "nie") << endl;
		cout << "4 - wersja pro. Aktualny wybor: " << (isPro ? "tak" : "nie") << endl;
		cout << "5 - wersja graficzna. Aktualny wybor: " << (graphical ? "tak" : "nie") << endl;
		cout << "6 - zezwol na cofanie ruchow. Aktualny wybor: " << (allowUndo ? "tak" : "nie") << endl;
		cout << "7 - zapisz ustawienia na dysku" << endl;
		cout << "8 - powrot" << endl << endl << endl << endl << endl;
		cout << "na niektorych urzadzeniach znaki UTF moga nie wyswietlac sie poprawnie, wiec wtedy trzeba wybrac opcje ASCII." << endl;
		int choice;
		if (!(cin >> choice)) {
			system("cls");
			repeat = true;
			cin.clear();
			cin.ignore(1000, '\n');
			continue;
		}
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
		case 7:
			saveCurrentSettings();
			break;
		case 8:
			repeat = false;
			break;
		default:
			repeat = true;
		}
	} while (repeat);
}
