#pragma once

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
