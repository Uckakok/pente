#include "penteEngine.h"
#include"declarations.h"
#include"analyzeBoard.h"
#include<mutex>
#include<thread>
#include<atomic>
#include<iostream>
#include<limits.h>
#include<chrono>
#include <functional>

atomic<int> analyzedPositions(0);

//statycznie ocenia planszê i generuje wszystkie dobre ruchy
array<vector<coordinates>, 16> computerPlayer::getAllGoodMoves(penteBoard * currentBoard, bool whiteTurn)
{
	//sprawdzenie czy mozna wygrac jednym ruchem (priorytet 0)
	array<vector<coordinates>, 16> allMovesWithPriorities;
	allMovesWithPriorities[0] = analyzeBoard::analyzeForFourWithGap(currentBoard, whiteTurn);
	vector<chain> chains1 = analyzeBoard::analyzeForChains(currentBoard, whiteTurn, 4);
	vector<chain> chains2 = analyzeBoard::analyzeForChains(currentBoard, whiteTurn, 3);
	vector<chain> chains3 = analyzeBoard::analyzeForChains(currentBoard, whiteTurn, 2);
	array<vector<coordinates>, 5> chainsBlocks = analyzeBoard::blockingChains(currentBoard, chains1); 
	allMovesWithPriorities[0].insert(end(allMovesWithPriorities[0]), begin(chainsBlocks[0]), end(chainsBlocks[0]));


	//sprawdzenie czy przeciwnik moze wygrac jednym ruchem (priorytet 1)
	allMovesWithPriorities[1] = analyzeBoard::analyzeForFourWithGap(currentBoard, !whiteTurn);
	vector<chain> enemyChains1 = analyzeBoard::analyzeForChains(currentBoard, !whiteTurn, 4);
	vector<chain> enemyChains2 = analyzeBoard::analyzeForChains(currentBoard, !whiteTurn, 3);
	array<vector<coordinates>, 5> chainsBlocksEnemy = analyzeBoard::blockingChains(currentBoard, enemyChains1);
	allMovesWithPriorities[1].insert(end(allMovesWithPriorities[1]), begin(chainsBlocksEnemy[0]), end(chainsBlocksEnemy[0]));

	//sprawdzenie czy mo¿na u³o¿yæ czwórkê odblokowan¹ z dwóch stron lub 2 przerwa 1 i odwrotnie(priorytet 2)
	vector<coordinates> twoGapOnePlayer = analyzeBoard::analyzeForThreeWithGap(currentBoard, whiteTurn);
	chainsBlocks = analyzeBoard::blockingChains(currentBoard, chains2);
	allMovesWithPriorities[2].insert(end(allMovesWithPriorities[2]), begin(chainsBlocks[1]), end(chainsBlocks[1]));
	allMovesWithPriorities[3].insert(end(allMovesWithPriorities[3]), begin(twoGapOnePlayer), end(twoGapOnePlayer));

	//zablokowanie trójki przeciwnika odblokowanej z obu stron i 2 przerwa 1, lub odwrotnie (priorytet 3)
	vector<coordinates> twoGapOneEnemy = analyzeBoard::analyzeForThreeWithGap(currentBoard, !whiteTurn);
	chainsBlocksEnemy = analyzeBoard::blockingChains(currentBoard, enemyChains2);
	allMovesWithPriorities[3].insert(end(allMovesWithPriorities[3]), begin(chainsBlocksEnemy[1]), end(chainsBlocksEnemy[1]));
	allMovesWithPriorities[3].insert(end(allMovesWithPriorities[3]), begin(twoGapOneEnemy), end(twoGapOneEnemy));

	//sprawdzenie czy mo¿na u³o¿yæ czwórkê zablokowan¹ z jednej strony (priorytet 4)
	chainsBlocks = analyzeBoard::blockingChains(currentBoard, chains3);
	allMovesWithPriorities[4].insert(end(allMovesWithPriorities[4]), begin(chainsBlocks[2]), end(chainsBlocks[2]));

	//zbicie przeciwnika (priorytet 5)
	vector<coordinates> takings = analyzeBoard::analyzeForTakings(currentBoard, whiteTurn);
	allMovesWithPriorities[5].insert(end(allMovesWithPriorities[5]), begin(takings), end(takings));

	//zrobienie z odblokowanej dwójki trójki (priorytet 6)
	allMovesWithPriorities[6].insert(end(allMovesWithPriorities[6]), begin(chainsBlocks[3]), end(chainsBlocks[3]));

	//uk³adanie bierek przy innych bierkach (priorytety 7-14)
	array<vector<coordinates>, 8> adjacent = analyzeBoard::findFieldsAdjacentToPieces(currentBoard, whiteTurn);
	allMovesWithPriorities[7] = adjacent[7];
	allMovesWithPriorities[8] = adjacent[6];
	allMovesWithPriorities[9] = adjacent[5];
	allMovesWithPriorities[10] = adjacent[4];
	allMovesWithPriorities[11] = adjacent[3];
	allMovesWithPriorities[12] = adjacent[2];
	allMovesWithPriorities[14] = adjacent[1];
	allMovesWithPriorities[15] = adjacent[0];

	return allMovesWithPriorities;
}

//generuje pocz¹tkowe ruchy dla komputery
coordinates computerPlayer::generateFirstMoves(penteBoard * currentBoard, bool whiteTurn)
{
	if (currentBoard->isPro) {
		int boardMiddle = (int)((BOARDSIZE) / 2);
		if (currentBoard->getMoveHistorySize() == 0) {
			return { (int)((BOARDSIZE) / 2), (int)((BOARDSIZE) / 2) };
		}
		if (currentBoard->getMoveHistorySize() == 2) {
			vector<coordinates> keryoMoves;
			for (int x = boardMiddle - 3; x < boardMiddle + 3; ++x) {
				for (int y = boardMiddle - 3; y < boardMiddle + 3; ++y) {
					if (x < boardMiddle - 2 || x > boardMiddle + 2 || y < boardMiddle - 2 || y > boardMiddle + 2) {
						keryoMoves.push_back({ x, y });
					}
				}
			}
			return keryoMoves[rand() % keryoMoves.size()];
		}
	}
	int boardMiddle = (int)((BOARDSIZE) / 2);
	vector<coordinates> possibleMoves;
	if (currentBoard->getMoveHistorySize() <= 2) {
		if (currentBoard->board[boardMiddle][boardMiddle] == EMPTY) {
			return { boardMiddle, boardMiddle };
		}
		else {
			vector<coordinates> possibleMoves;
			for (int x = boardMiddle - 1; x <= boardMiddle + 1; ++x) {
				for (int y = boardMiddle - 1; y <= boardMiddle + 1; ++y) {
					if (currentBoard->board[y][x] == EMPTY) {
						possibleMoves.push_back({ x, y });
					}
				}
			}
			return possibleMoves[rand() % possibleMoves.size()];
		}
	}
	cout << "Krytyczny blad programu." << __FILE__ << " " << __LINE__ << endl;
	return { -1, -1 };
}

//ocenia jakoœæ pozycji dla pojedyñczego gracza sprawdzaj¹c wszystkie dostêpne dla niego ruchy
int computerPlayer::evaluatePlayer(penteBoard *currentBoard, bool whiteTurn) {
	int score = 0;
	int scoreValues[] = { 100000, 50000, 5000,  5000, 600, 400, 400, 300, 200, 150, 50, 6000};
	//sprawdzenie czy mozna wygrac jednym ruchem 
	score += analyzeBoard::analyzeForFourWithGap(currentBoard, whiteTurn).size() * scoreValues[0];
	vector<chain> chains1 = analyzeBoard::analyzeForChains(currentBoard, whiteTurn, 4);
	vector<chain> chains2 = analyzeBoard::analyzeForChains(currentBoard, whiteTurn, 3);
	vector<chain> chains3 = analyzeBoard::analyzeForChains(currentBoard, whiteTurn, 2);
	array<vector<coordinates>, 5> chainsBlocks = analyzeBoard::blockingChains(currentBoard, chains1);
	score += chainsBlocks[0].size() * scoreValues[0];

	//sprawdzenie czy mo¿na u³o¿yæ czwórkê odblokowan¹ z dwóch stron lub 2 przerwa 1 i odwrotnie
	score += analyzeBoard::analyzeForThreeWithGap(currentBoard, whiteTurn).size() * scoreValues[1];
	chainsBlocks = analyzeBoard::blockingChains(currentBoard, chains2);
	score += chainsBlocks[1].size() * scoreValues[1];

	//sprawdzenie czy mo¿na u³o¿yæ czwórkê zablokowan¹ z jednej strony
	score += analyzeBoard::blockingChains(currentBoard, chains3).size() * scoreValues[2];

	//zbicie przeciwnika
	score += analyzeBoard::analyzeForTakings(currentBoard, whiteTurn).size() * scoreValues[3];

	//zrobienie z odblokowanej dwójki trójki
	score += chainsBlocks[3].size() * scoreValues[4];

	//uk³adanie bierek przy innych bierkach 
	array<vector<coordinates>, 8> adjacent = analyzeBoard::findFieldsAdjacentToPieces(currentBoard, whiteTurn);
	score += adjacent[7].size() * scoreValues[4];
	score += adjacent[6].size() * scoreValues[5];
	score += adjacent[5].size() * scoreValues[6];
	score += adjacent[4].size() * scoreValues[7];
	score += adjacent[3].size() * scoreValues[8];
	score += adjacent[2].size() * scoreValues[9];


	return score;
}

//przypisuje pozycji ocenê
int computerPlayer::staticPositionEvaluation(penteBoard * currentBoard)
{
	//dodatnie dla bia³ego, ujemne dla czarnego
	if (currentBoard->gameWon) {
		if (currentBoard->winner == WHITE) {
			return INT_MAX;
		}
		else if (currentBoard->winner == BLACK) {
			return INT_MIN;
		}
	}
	//sprawdzenie gracza bia³ego
	int score = 0;
	score += evaluatePlayer(currentBoard, true);
	score -= evaluatePlayer(currentBoard, false);
	int takings = currentBoard->takesForWhite - currentBoard->takesForBlack;
	score += takings * 8000;
	return score;
}


//algorytm minmax z zastosowaniem apha-beta pruning
int computerPlayer::minMaxAlgorithm(penteBoard * currentBoard, int depth, int alpha, int beta, bool whiteTurn)
{
	if (depth <= 0 || currentBoard->gameWon) {
		analyzedPositions++;
		return staticPositionEvaluation(currentBoard);
	}
	vector<coordinates> movesToCheck = generateMovesWorthChecking(currentBoard, whiteTurn);
	if (movesToCheck.size() <= 0) {
		analyzedPositions++;
		return staticPositionEvaluation(currentBoard);
	}
	if (whiteTurn) {
		int maxEvaluation = INT_MIN;
		for (auto& move : movesToCheck) {
			int takesForWhite = currentBoard->takesForWhite;
			int takesForBlack = currentBoard->takesForBlack;
			currentBoard->makeMove(move.x, move.y);
			int evaluation = minMaxAlgorithm(currentBoard, depth - 1, alpha, beta, false);
			maxEvaluation = max(maxEvaluation, evaluation);
			currentBoard->unmakeMove();
			currentBoard->winner = -1;
			currentBoard->gameWon = false;
			currentBoard->takesForWhite = takesForWhite;
			currentBoard->takesForBlack = takesForBlack;
			if (maxEvaluation > beta) {
				break;
			}
			alpha = max(alpha, evaluation);
		}
		return maxEvaluation;
	}
	else {
		int minEvaluation = INT_MAX;
		for (auto& move : movesToCheck) {
			int takesForWhite = currentBoard->takesForWhite;
			int takesForBlack = currentBoard->takesForBlack;
			currentBoard->makeMove(move.x, move.y);
			int evaluation = minMaxAlgorithm(currentBoard, depth - 1, alpha, beta, true);
			minEvaluation = min(minEvaluation, evaluation);
			currentBoard->unmakeMove();
			currentBoard->winner = -1;
			currentBoard->gameWon = false;
			currentBoard->takesForWhite = takesForWhite;
			currentBoard->takesForBlack = takesForBlack;
			if (minEvaluation < alpha) {
				break;
			}
			beta = min(beta, minEvaluation);
		}
		return minEvaluation;
	}
}

//generuje wszystkie pola s¹siaduj¹ce z chocia¿ jedn¹ bierk¹
vector<coordinates> computerPlayer::generateMovesWorthChecking(penteBoard * currentBoard, bool whiteTurn)
{
	//tworzenie listy wektorów wszystkich pustych pól s¹siaduj¹cych z bierkami i ich przy³¹czanie do jednego wektora
	vector<coordinates> allMovesWithPriorities;
	array<vector<coordinates>, 8> adjacent = analyzeBoard::findFieldsAdjacentToPieces(currentBoard, whiteTurn);
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[7]), end(adjacent[7]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[6]), end(adjacent[6]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[5]), end(adjacent[5]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[4]), end(adjacent[4]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[3]), end(adjacent[3]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[2]), end(adjacent[2]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[1]), end(adjacent[1])); 
	adjacent = analyzeBoard::findFieldsAdjacentToPieces(currentBoard, !whiteTurn);
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[7]), end(adjacent[7]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[6]), end(adjacent[6]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[5]), end(adjacent[5]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[4]), end(adjacent[4]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[3]), end(adjacent[3]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[2]), end(adjacent[2]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[1]), end(adjacent[1]));
	return allMovesWithPriorities;
}

//sprawdza czy s¹ wyj¹tkowo istotne ruchy - wygranie w jednym ruchu, zablokowanie wygranej przeciwnika itp
vector<coordinates> computerPlayer::forcedOrWinning(penteBoard * currentBoard, bool whiteTurn)
{
	//sprawdzenie czy mozna wygrac jednym ruchem (priorytet 0)
	vector<coordinates> winningMoves;
	winningMoves = analyzeBoard::analyzeForFourWithGap(currentBoard, whiteTurn);
	vector<chain> chains1 = analyzeBoard::analyzeForChains(currentBoard, whiteTurn, 4);
	vector<chain> chains2 = analyzeBoard::analyzeForChains(currentBoard, whiteTurn, 3);
	array<vector<coordinates>, 5> chainsBlocks = analyzeBoard::blockingChains(currentBoard, chains1);
	winningMoves.insert(end(winningMoves), begin(chainsBlocks[0]), end(chainsBlocks[0]));
	if (winningMoves.size() != 0)
		return winningMoves;

	//sprawdzenie czy przeciwnik moze wygrac jednym ruchem (priorytet 1)
	winningMoves = analyzeBoard::analyzeForFourWithGap(currentBoard, !whiteTurn);
	vector<chain> enemyChains1 = analyzeBoard::analyzeForChains(currentBoard, !whiteTurn, 4);
	vector<chain> enemyChains2 = analyzeBoard::analyzeForChains(currentBoard, !whiteTurn, 3);
	array<vector<coordinates>, 5> chainsBlocksEnemy = analyzeBoard::blockingChains(currentBoard, enemyChains1);
	winningMoves.insert(end(winningMoves), begin(chainsBlocksEnemy[0]), end(chainsBlocksEnemy[0]));
	if (winningMoves.size() != 0)
		return winningMoves;
	//sprawdzenie czy mo¿na u³o¿yæ czwórkê odblokowan¹ z dwóch stron lub 2 przerwa 1 i odwrotnie(priorytet 2)
	vector<coordinates> twoGapOnePlayer = analyzeBoard::analyzeForThreeWithGap(currentBoard, whiteTurn);
	chainsBlocks = analyzeBoard::blockingChains(currentBoard, chains2);
	winningMoves.insert(end(winningMoves), begin(chainsBlocks[1]), end(chainsBlocks[1]));
	winningMoves.insert(end(winningMoves), begin(twoGapOnePlayer), end(twoGapOnePlayer));
	if (winningMoves.size() != 0)
		return winningMoves;
	//zablokowanie trójki przeciwnika odblokowanej z obu stron i 2 przerwa 1, lub odwrotnie (priorytet 3)
	vector<coordinates> twoGapOneEnemy = analyzeBoard::analyzeForThreeWithGap(currentBoard, !whiteTurn);
	chainsBlocksEnemy = analyzeBoard::blockingChains(currentBoard, enemyChains2);
	winningMoves.insert(end(winningMoves), begin(chainsBlocksEnemy[1]), end(chainsBlocksEnemy[1]));
	winningMoves.insert(end(winningMoves), begin(twoGapOneEnemy), end(twoGapOneEnemy));

	return winningMoves;
}

//generuje wszystkie pocz¹tkowe ruchy, kopiuje planszê tyle razy ile jest ruchów i w osobnym w¹tku odpala algorytm minmax dla ka¿dego dostêpnego pocz¹tkowego ruchu
//takie podejœcie pozwala skutecznie wykorzystaæ wszystkie rdzenie komputera, by przyspieszyæ obliczenia.
coordinates computerPlayer::prepareAndPerformMinMax(penteBoard * currentBoard, bool whiteTurn, int depth)
{
	using namespace chrono;
	if (currentBoard->getMoveHistorySize() <= 2) {
		return generateFirstMoves(currentBoard, whiteTurn);
	}
	vector<coordinates> winningMoves = forcedOrWinning(currentBoard, whiteTurn);
	if (winningMoves.size() != 0) {
		return (winningMoves[0]);
	}
	analyzedPositions = 0;
	vector<coordinates> moves = generateMovesWorthChecking(currentBoard, whiteTurn);
	if (moves.size() == 0) {
		if (currentBoard->getMoveHistorySize() <= 1) {
			return generateFirstMoves(currentBoard, whiteTurn);
		}
		coordinates moveToTest;
		array<vector<coordinates>, 8> adjacent = analyzeBoard::findFieldsAdjacentToPieces(currentBoard, whiteTurn);
		if (adjacent[0].size() != 0) {
			return (adjacent[0][rand() % adjacent[0].size()]);
		}
		adjacent = analyzeBoard::findFieldsAdjacentToPieces(currentBoard, !whiteTurn);
		for (int i = 0; i < 8; ++i) {
			if (adjacent[i].size() != 0) {
				return (adjacent[i][rand() % adjacent[i].size()]);
			}
		}
		do {
			moveToTest = { rand() % BOARDSIZE, rand() % BOARDSIZE };
		} while (currentBoard->board[moveToTest.y][moveToTest.x] != EMPTY);
		return moveToTest;
	}
	coordinates bestMove = moves[0];
	int bestMoveScore = (whiteTurn ? INT_MIN : INT_MAX);

	vector<thread> threads;
	mutex mutex;
	vector<int> scores(moves.size());

	milliseconds start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	for (int i = 0; i < (signed)moves.size(); ++i) {
		threads.emplace_back([&, i]() {


			penteBoard* boardCopy = new penteBoard(*currentBoard);
			boardCopy->makeMove(moves[i].x, moves[i].y);

			int score = minMaxAlgorithm(boardCopy, depth, INT_MIN, INT_MAX, !whiteTurn);


			lock_guard<std::mutex> lock(mutex);
			scores[i] = score;

			delete boardCopy;
		});
	}
	for (auto& thread : threads) {
		thread.join();
	}

	for (int i = 0; i < (signed)moves.size(); ++i) {
		if ((whiteTurn && scores[i] == bestMoveScore) || (!whiteTurn && scores[i] == bestMoveScore) && rand() % 2 == 0) {
			bestMove = moves[i];
			bestMoveScore = scores[i];
		}
		if ((whiteTurn && scores[i] > bestMoveScore) || (!whiteTurn && scores[i] < bestMoveScore)) {
			bestMove = moves[i];
			bestMoveScore = scores[i];
		}
	}
	milliseconds end = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	cout << "przeanalizowano: " << analyzedPositions.load() << " pozycji w:  " << (end - start).count() << " milisekund" << endl;
	return bestMove;
}


//losowe generowanie ruchów
coordinates computerPlayer::findBestMove(penteBoard * currentBoard, bool whiteTurn)
{
	if (currentBoard->getMoveHistorySize() <= 2) {
		return generateFirstMoves(currentBoard, whiteTurn);
	}
	coordinates moveToTest;
	do {
		moveToTest = { rand() % BOARDSIZE, rand() % BOARDSIZE };
	} while (currentBoard->board[moveToTest.y][moveToTest.x] != EMPTY);
	return moveToTest;
}

//generowanie ruchów przy pomocy statycznej ewaluacji
coordinates mediumComputer::findBestMove(penteBoard * currentBoard, bool whiteTurn)
{
	if (currentBoard->getMoveHistorySize() <= 2) {
		return generateFirstMoves(currentBoard, whiteTurn);
	}
	array<vector<coordinates>, 16> moves = getAllGoodMoves(currentBoard, whiteTurn);
	for (int i = 0; i < 16; ++i) {
		if (moves[i].size() == 0) continue;
		else {
			return moves[i][rand() % moves[i].size()];
		}
	}
	
	coordinates moveToTest;
	do {
		moveToTest = { rand() % BOARDSIZE, rand() % BOARDSIZE };
	} while (currentBoard->board[moveToTest.y][moveToTest.x] != EMPTY);
	return moveToTest;
}

computerPlayer * generateAIInstance(int difficulty)
{
	computerPlayer* instance = nullptr;
	switch (difficulty) {
	case -1:
		instance = new computerPlayer();
		break;
	case 0:
		instance = new easyComputer();
		break;
	case 1:
		instance = new mediumComputer();
		break;
	case 2:
		instance = new advancedComputer();
		break;
	case 3:
		instance = new expertComputer();
		break;
	case 4:
		instance = new masterComputer();
		break;
	default:
		cout << "Nierozpoznany poziom trudnosci AI" << endl;
		cout << "Inicjowanie AI o zaawansowanym poziomie trudnoœci" << endl;
		instance = new advancedComputer();
		break;
	}
	return instance;
}

//generuje ruch przy wykorzystanie algorytmu minmax z g³êbokoœci¹ 3
coordinates expertComputer::findBestMove(penteBoard * currentBoard, bool whiteTurn)
{
	return prepareAndPerformMinMax(currentBoard, whiteTurn, 2);
}

//generuje ruch przy wykorzystanie algorytmu minmax z g³êbokoœci¹ 2
coordinates advancedComputer::findBestMove(penteBoard * currentBoard, bool whiteTurn)
{
	return prepareAndPerformMinMax(currentBoard, whiteTurn, 1);
}

//generuje ruch przy wykorzystanie algorytmu minmax z g³êbokoœci¹ 5
coordinates masterComputer::findBestMove(penteBoard * currentBoard, bool whiteTurn)
{
	return prepareAndPerformMinMax(currentBoard, whiteTurn, 4);
}

//generuje ruch przy pomocy statycznej ewaluacji z dodatkow¹ szans¹ na przegapienie niez³ych ruchów
coordinates easyComputer::findBestMove(penteBoard * currentBoard, bool whiteTurn)
{
	//szanse przedstawione jako 1 na ile przypadkow przegapi dana kategorie ruchow
	int chancesToOverlook[] = { 20, 10, 8, 6, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2};
	array<vector<coordinates>, 16> moves = getAllGoodMoves(currentBoard, whiteTurn);
	for (int i = 0; i < 16; ++i) {
		if (moves[i].size() == 0) continue;
		else if (rand() % chancesToOverlook[i] != 0){
			return moves[i][rand() % moves[i].size()];
		}
	}

	//jeœli nie znaleziono ¿adnego ruchu, musimy wygenerowaæ losowy.
	if (currentBoard->getMoveHistorySize() <= 1) {
		return generateFirstMoves(currentBoard, whiteTurn);
	}
	coordinates moveToTest;
	do {
		moveToTest = { rand() % BOARDSIZE, rand() % BOARDSIZE };
	} while (currentBoard->board[moveToTest.y][moveToTest.x] != EMPTY);
	return moveToTest;
}
