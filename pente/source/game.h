#include<vector>
#include<penteEngine.h>
#include"settings.h"
#include"graphicalMode.h"

int startGame();
void initalizeBoard(settings *currentSettings, penteBoard* currentBoard);
penteBoard* createBoard(settings *currentSettings);
void gameLoop(penteBoard *currentGame, settings *currentSettings);
vector<pieceToDraw> getAllPieces(penteBoard *currentGame);
int mainMenu();
void listSavesInWorkingDirectory();
void printPenteRules();
coordinates gameChoices(settings *currentSettings);