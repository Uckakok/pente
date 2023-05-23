#pragma once

#include"settings.h"
#include"declarations.h"
#include<iostream>
#include<fstream>
#include<string>

using namespace std;


class settings {
private:

public:
	int AIDifficulty;
	int prefferedPenteVersion;
	int prefferedConsole;
	bool autosaveOnExit;
	bool isPro;
	bool graphical;
	bool allowUndo;
	settings();
	bool saveCurrentSettings();
	void changeSettings();
};
