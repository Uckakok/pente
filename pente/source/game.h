#include<vector>
#include <winsock2.h>
#include<penteEngine.h>
#include"settings.h"
#include"graphicalMode.h"

int startGame();
int handleGameEvents(coordinates nextMove, penteBoard * currentGame, settings * currentSettings, graphicalInterface*& window);
void gameLoop(penteBoard *currentGame, settings *currentSettings, SOCKET enemySocket);
coordinates getNextMove(settings * currentSettings, graphicalInterface * window, penteBoard * currentGame);
vector<pieceToDraw> getAllPieces(penteBoard *currentGame);
int mainMenu();
int onlineMenu();
void listSavesInWorkingDirectory();
void printPenteRules();
coordinates gameChoices(settings *currentSettings);