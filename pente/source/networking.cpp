#include"networking.h"
#include<iostream>
#include<string>
#include"declarations.h"

#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int runHost(penteBoard*& currentGame, settings *currentSettings, SOCKET *serverSocket, SOCKET *clientSocket) {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "Nie udalo sie zainicjalizowac Winsock." << endl;
		system("pause");
		return 0;
	}

	sockaddr_in serverAddress;
	// Tworzenie gniazda serwera
	*serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (*serverSocket == INVALID_SOCKET) {
		cout << "Nie udalo sie otworzyc gry online." << endl;
		WSACleanup();
		system("pause");
		return 0;
	}

	currentGame = createBoard(currentSettings);
	initalizeBoard(currentSettings, currentGame);
	currentGame->online = true;
	currentGame->isHost = true;

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PENTEPORT);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// Powi¹zanie gniazda serwera z adresem i portem
	if (bind(*serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		cout << "Nie udalo sie powiazac portu serwera." << endl;
		closesocket(*serverSocket);
		WSACleanup();
		system("pause");
		return 0;
	}

	cout << "Oczekiwanie na przychodzace polaczenia przez 30 sekund..." << endl;
	unsigned long nonBlocking = 1;
	// Ustawienie trybu nieblokuj¹cego dla gniazda serwera
	if (ioctlsocket(*serverSocket, FIONBIO, &nonBlocking) != 0) {
		cout << "Nie udalo sie zmienic trybu portu na nieblokujacy." << endl;
		closesocket(*serverSocket);
		WSACleanup();
		system("pause");
		return 0;
	}

	if (listen(*serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		cout << "Nie powiodlo sie nasluchiwanie na przychodzace polaczenia." << endl;
		closesocket(*serverSocket);
		WSACleanup();
		system("pause");
		return 0;
	}

	const int timeoutSecs = 30;
	fd_set readSet;
	FD_ZERO(&readSet);
	FD_SET(*serverSocket, &readSet);
	timeval timeout;
	timeout.tv_sec = timeoutSecs;
	timeout.tv_usec = 0;

	int selectResult = select(0, &readSet, nullptr, nullptr, &timeout);

	if (selectResult == SOCKET_ERROR) {
		cout << "Blad portu." << endl;
		closesocket(*serverSocket);
		WSACleanup();
		system("pause");
		return 0;
	}
	else if (selectResult == 0) {
		cout << "Nikt nie dolaczyl do gry." << endl;
		closesocket(*serverSocket);
		WSACleanup();
		system("pause");
		return 0;
	}
	sockaddr_in clientAddress;
	int clientAddressLength = sizeof(clientAddress);
	// Akceptowanie po³¹czenia od klienta
	*clientSocket = accept(*serverSocket, (sockaddr*)&clientAddress, &clientAddressLength);
	if (*clientSocket == INVALID_SOCKET) {
		cout << "Nie udalo sie polaczyc." << endl;
		WSACleanup();
		system("pause");
		return 0;
	}

	// Wys³anie ustawieñ klientowi
	if (send(*clientSocket, (char*)&currentSettings->isPro, sizeof(currentSettings->isPro), 0) <= 0) {
		cout << "Nie udalo sie wyslac ustawien klientowi." << endl;
		WSACleanup();
		system("pause");
		return 0;
	}

	if (send(*clientSocket, (char*)&currentSettings->prefferedPenteVersion, sizeof(currentSettings->isPro), 0) <= 0) {
		cout << "Nie udalo sie wyslac ustawien klientowi." << endl;
		WSACleanup();
		system("pause");
		return 0;
	}

	return 1;
}

int runClient(penteBoard*& currentGame, settings *currentSettings, SOCKET *clientSocket) {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "Nie udalo sie zainicjalizowac Winsock." << endl;
		system("pause");
		return 0;
	}

	sockaddr_in serverAddress;
	// Tworzenie gniazda klienta
	*clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	currentGame = createBoard(currentSettings);
	currentGame->online = true;
	currentGame->isHost = false;

	string hostIP;
	cout << "Podaj adres hosta: ";
	cin >> hostIP;

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PENTEPORT);
	// Konwersja adresu IP z formatu tekstowego na binarny
	inet_pton(AF_INET, hostIP.c_str(), &(serverAddress.sin_addr));

	int timeoutSec = 5;
	struct timeval timeout;
	timeout.tv_sec = timeoutSec;
	timeout.tv_usec = 0;
	// Ustawienie timeouta dla gniazda klienta
	if (setsockopt(*clientSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0) {
		cout << "Nie udalo sie utworzyc timouta dla portu." << endl;
		closesocket(*clientSocket);
		WSACleanup();
		system("pause");
		return 0;
	}
	cout << "Proba utworzenia polaczenia przez 5 sekund..." << endl;
	if (connect(*clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		cout << "Nie udalo sie dolaczyc do gry." << endl;
		closesocket(*clientSocket);
		WSACleanup();
		system("pause");
		return 0;
	}

	// Odbieranie ustawieñ od serwera
	while (recv(*clientSocket, (char*)&currentGame->isPro, sizeof(currentGame->isPro), 0) <= 0) {
		;
	}
	while (recv(*clientSocket, (char*)&currentGame->penteVariant, sizeof(currentGame->penteVariant), 0) <= 0) {
		;
	}

	cout << (currentGame->isPro ? "Granie w wersji pro, " : "") << (currentGame->penteVariant == REGULARPENTE ? "Zwyczajne pente" : "Keryo pente") << endl;
	return 1;
}