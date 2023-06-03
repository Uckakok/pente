#pragma once
#include"penteBoard.h"
#include"settings.h"
#include <ws2tcpip.h>

int runHost(penteBoard*& currentGame, settings *currentSettings, SOCKET *serverSocket, SOCKET *clientSocket);
int runClient(penteBoard*& currentGame, settings *currentSettings, SOCKET *clientSocket);