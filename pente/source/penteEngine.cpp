#include "penteEngine.h"

int analyzedPositions;

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
	enemyChains1.insert(end(enemyChains1), begin(enemyChains2), end(enemyChains2));
	array<vector<coordinates>, 5> chainsBlocksEnemy = analyzeBoard::blockingChains(currentBoard, enemyChains1);
	allMovesWithPriorities[1].insert(end(allMovesWithPriorities[1]), begin(chainsBlocksEnemy[0]), end(chainsBlocksEnemy[0]));

	//sprawdzenie czy mo¿na u³o¿yæ czwórkê odblokowan¹ z dwóch stron lub 2 przerwa 1 i odwrotnie(priorytet 2)
	vector<coordinates> twoGapOnePlayer = analyzeBoard::analyzeForThreeWithGap(currentBoard, whiteTurn);
	chainsBlocks = analyzeBoard::blockingChains(currentBoard, chains2);
	allMovesWithPriorities[2].insert(end(allMovesWithPriorities[2]), begin(chainsBlocks[1]), end(chainsBlocks[1]));
	allMovesWithPriorities[3].insert(end(allMovesWithPriorities[3]), begin(twoGapOnePlayer), end(twoGapOnePlayer));

	//zablokowanie trójki przeciwnika odblokowanej z obu stron i 2 przerwa 1, lub odwrotnie (priorytet 3)
	vector<coordinates> twoGapOneEnemy = analyzeBoard::analyzeForThreeWithGap(currentBoard, !whiteTurn);
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

	//jeœli wszystkie listy mocnych ruchow s¹ puste, sprawdŸ czy wariant to Keryo Pente, jeœli tak, to dodaj ruchy na które pozwalaj¹ zasady i zwróæ wartoœæ
	if (currentBoard->isPro) {
		int boardMiddle = (int)((BOARDSIZE) / 2);
		if (currentBoard->getMoveHistorySize() == 0) {
			allMovesWithPriorities[15] = { (int)((BOARDSIZE) / 2), (int)((BOARDSIZE) / 2) };
		}
		if (currentBoard->getMoveHistorySize() == 2) {
			vector<coordinates> keryoMoves;
			for (int x = boardMiddle - 4; x < boardMiddle + 4; ++x) {
				for (int y = boardMiddle - 4; y < boardMiddle + 4; ++y) {
					if (x < boardMiddle - 2 || x > boardMiddle + 2 || y < boardMiddle - 2 || y > boardMiddle + 2) {
						keryoMoves.push_back({ x, y });
					}
				}
			}
			allMovesWithPriorities[15] = keryoMoves;
			return allMovesWithPriorities;
		}
	}
	allMovesWithPriorities[15] = adjacent[0];

	return allMovesWithPriorities;
}


coordinates computerPlayer::generateFirstMoves(penteBoard * currentBoard, bool whiteTurn)
{
	int boardMiddle = (int)((BOARDSIZE) / 2);
	vector<coordinates> possibleMoves;
	if (currentBoard->getMoveHistorySize() == 0 || currentBoard->getMoveHistorySize()) {
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
}

int computerPlayer::evaluatePlayer(penteBoard *currentBoard, bool whiteTurn) {
	int score = 0;
	int scoreValues[] = { 100000, 50000, 3000,  2500, 2300, 800, 750, 700, 600, 300, 100, };
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

int computerPlayer::staticPositionEvaluation(penteBoard * currentBoard)
{
	//dodatnie dla bia³ego, ujemne dla czarnego
	if (currentBoard->gameWon) {
		if (currentBoard->winner == WHITE) {
			return MAXINT;
		}
		else if (currentBoard->winner == BLACK) {
			return MININT;
		}
	}
	//sprawdzenie gracza bia³ego
	int score = 0;
	score += evaluatePlayer(currentBoard, true);
	score -= evaluatePlayer(currentBoard, false);
	return score;
}


int computerPlayer::minMaxAlgorithm(penteBoard * currentBoard, int depth, bool whiteTurn)
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
		int maxEvaluation = MININT;
		for (auto& move : movesToCheck) {
			int takesForWhite = currentBoard->takesForWhite;
			int takesForBlack = currentBoard->takesForBlack;
			currentBoard->makeMove(move.x, move.y);
			int evaluation = minMaxAlgorithm(currentBoard, depth - 1, false);
			maxEvaluation = max(maxEvaluation, evaluation);
			currentBoard->unmakeMove();
			currentBoard->winner = -1;
			currentBoard->gameWon = false;
			currentBoard->takesForWhite = takesForWhite;
			currentBoard->takesForBlack = takesForBlack;
		}
		return maxEvaluation;
	}
	else {
		int minEvaluation = MAXINT;
		for (auto& move : movesToCheck) {
			int takesForWhite = currentBoard->takesForWhite;
			int takesForBlack = currentBoard->takesForBlack;
			currentBoard->makeMove(move.x, move.y);
			int evaluation = minMaxAlgorithm(currentBoard, depth - 1, true);
			minEvaluation = min(minEvaluation, evaluation);
			currentBoard->unmakeMove();
			currentBoard->winner = -1;
			currentBoard->gameWon = false;
			currentBoard->takesForWhite = takesForWhite;
			currentBoard->takesForBlack = takesForBlack;
		}
		return minEvaluation;
	}
}

vector<coordinates> computerPlayer::generateMovesWorthChecking(penteBoard * currentBoard, bool whiteTurn)
{
	//sprawdzenie czy mozna wygrac jednym ruchem (priorytet 0)
	vector<coordinates> allMovesWithPriorities;
	allMovesWithPriorities = analyzeBoard::analyzeForFourWithGap(currentBoard, whiteTurn);
	vector<chain> chains1 = analyzeBoard::analyzeForChains(currentBoard, whiteTurn, 4);
	vector<chain> chains2 = analyzeBoard::analyzeForChains(currentBoard, whiteTurn, 3);
	vector<chain> chains3 = analyzeBoard::analyzeForChains(currentBoard, whiteTurn, 2);
	array<vector<coordinates>, 5> chainsBlocks = analyzeBoard::blockingChains(currentBoard, chains1);
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(chainsBlocks[0]), end(chainsBlocks[0]));


	//sprawdzenie czy przeciwnik moze wygrac jednym ruchem (priorytet 1)
	allMovesWithPriorities = analyzeBoard::analyzeForFourWithGap(currentBoard, !whiteTurn);
	vector<chain> enemyChains1 = analyzeBoard::analyzeForChains(currentBoard, !whiteTurn, 4);
	vector<chain> enemyChains2 = analyzeBoard::analyzeForChains(currentBoard, !whiteTurn, 3);
	enemyChains1.insert(end(enemyChains1), begin(enemyChains2), end(enemyChains2));
	array<vector<coordinates>, 5> chainsBlocksEnemy = analyzeBoard::blockingChains(currentBoard, enemyChains1);
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(chainsBlocksEnemy[0]), end(chainsBlocksEnemy[0]));

	//sprawdzenie czy mo¿na u³o¿yæ czwórkê odblokowan¹ z dwóch stron lub 2 przerwa 1 i odwrotnie(priorytet 2)
	vector<coordinates> twoGapOnePlayer = analyzeBoard::analyzeForThreeWithGap(currentBoard, whiteTurn);
	chainsBlocks = analyzeBoard::blockingChains(currentBoard, chains2);
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(chainsBlocks[1]), end(chainsBlocks[1]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(twoGapOnePlayer), end(twoGapOnePlayer));

	//zablokowanie trójki przeciwnika odblokowanej z obu stron i 2 przerwa 1, lub odwrotnie (priorytet 3)
	chainsBlocksEnemy = analyzeBoard::blockingChains(currentBoard, enemyChains2);
	vector<coordinates> twoGapOneEnemy = analyzeBoard::analyzeForThreeWithGap(currentBoard, !whiteTurn);
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(chainsBlocksEnemy[1]), end(chainsBlocksEnemy[1]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(twoGapOneEnemy), end(twoGapOneEnemy));

	//sprawdzenie czy mo¿na u³o¿yæ czwórkê zablokowan¹ z jednej strony (priorytet 4)
	chainsBlocks = analyzeBoard::blockingChains(currentBoard, chains3);
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(chainsBlocks[2]), end(chainsBlocks[2]));

	//zbicie przeciwnika (priorytet 5)
	vector<coordinates> takings = analyzeBoard::analyzeForTakings(currentBoard, whiteTurn);
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(takings), end(takings));

	//zrobienie z odblokowanej dwójki trójki (priorytet 6)
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(chainsBlocks[3]), end(chainsBlocks[3]));

	//uk³adanie bierek przy innych bierkach (priorytety 7-14)
	array<vector<coordinates>, 8> adjacent = analyzeBoard::findFieldsAdjacentToPieces(currentBoard, whiteTurn);
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[7]), end(adjacent[7]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[6]), end(adjacent[6]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[5]), end(adjacent[5]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[4]), end(adjacent[4]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[3]), end(adjacent[3]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[2]), end(adjacent[2]));
	allMovesWithPriorities.insert(end(allMovesWithPriorities), begin(adjacent[1]), end(adjacent[1]));
	return allMovesWithPriorities;
}


coordinates computerPlayer::findBestMove(penteBoard * currentBoard, bool whiteTurn)
{
	coordinates testMove;
	if (currentBoard->isPro) {
		int boardMiddle = (int)((BOARDSIZE) / 2);
		if (currentBoard->getMoveHistorySize() == 0) {
			testMove = { (int)((BOARDSIZE) / 2), (int)((BOARDSIZE) / 2) };
		}
		if (currentBoard->getMoveHistorySize() == 2) {
			vector<coordinates> keryoMoves;
			for (int x = boardMiddle - 4; x < boardMiddle + 4; ++x) {
				for (int y = boardMiddle - 4; y < boardMiddle + 4; ++y) {
					if (x < boardMiddle - 2 || x > boardMiddle + 2 || y < boardMiddle - 2 || y > boardMiddle + 2) {
						keryoMoves.push_back({ x, y });
					}
				}
			}
			return (keryoMoves[rand() % keryoMoves.size()]);
		}
	}
	else if (currentBoard->getMoveHistorySize() <= 1) {
		return generateFirstMoves(currentBoard, whiteTurn);
	}
	return coordinates({rand() % BOARDSIZE, rand() % BOARDSIZE});
}

coordinates mediumComputer::findBestMove(penteBoard * currentBoard, bool whiteTurn)
{
	array<vector<coordinates>, 16> moves = getAllGoodMoves(currentBoard, whiteTurn);
	for (int i = 0; i < 16; ++i) {
		if (moves[i].size() == 0) continue;
		else {
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

computerPlayer * generateAIInstance(int difficulty)
{
	computerPlayer* instance = nullptr;
	switch (difficulty) {
	case -1:
		instance = new computerPlayer();
		break;
	case 0:
		instance = new mediumComputer();
		break;
	case 1:
		cout << "Nie istnieje jeszcze srednie AI" << endl;
		instance = new advancedComputer();
		break;
	case 2:
		instance = new advancedComputer();
		break;
	case 3:
		instance = new expertComputer();
		break;
	default:
		cout << "Nierozpoznany poziom trudnosci AI" << endl;
		instance = new advancedComputer();
		break;
	}
	return instance;
}

coordinates expertComputer::findBestMove(penteBoard * currentBoard, bool whiteTurn)
{
	//wygraj jednym ruchem
	vector<coordinates> winningMoves;
	winningMoves = analyzeBoard::analyzeForFourWithGap(currentBoard, whiteTurn);
	vector<chain> chains1 = analyzeBoard::analyzeForChains(currentBoard, whiteTurn, 4);
	array<vector<coordinates>, 5> chainsBlocks = analyzeBoard::blockingChains(currentBoard, chains1);
	winningMoves.insert(end(winningMoves), begin(chainsBlocks[0]), end(chainsBlocks[0]));
	if (winningMoves.size() != 0) {
		return (winningMoves[rand() % winningMoves.size()]);
	}
	analyzedPositions = 0;
	vector<coordinates> moves = generateMovesWorthChecking(currentBoard, whiteTurn);
	if (moves.size() == 0) {
		if (currentBoard->getMoveHistorySize() <= 1) {
			return generateFirstMoves(currentBoard, whiteTurn);
		}
		coordinates moveToTest;
		//sprobuj ulozyc pionek obok innego pionka
		array<vector<coordinates>, 8> adjacent = analyzeBoard::findFieldsAdjacentToPieces(currentBoard, whiteTurn);
		if (adjacent[0].size() != 0) {
			return (adjacent[0][rand() % adjacent[0].size()]);
		}
		do {
			moveToTest = { rand() % BOARDSIZE, rand() % BOARDSIZE };
		} while (currentBoard->board[moveToTest.y][moveToTest.x] != EMPTY);
		return moveToTest;
	}
	coordinates bestMove = moves[0];
	int bestMoveScore = (whiteTurn ? MININT : MAXINT);
	for (int i = 0; i < moves.size(); ++i) {
		int takesForWhite = currentBoard->takesForWhite;
		int takesForBlack = currentBoard->takesForBlack;
		currentBoard->makeMove(moves[i].x, moves[i].y);
		int currentScore = minMaxAlgorithm(currentBoard, 3, !whiteTurn);
		if (whiteTurn && currentScore > bestMoveScore) {
			bestMove = moves[i];
			bestMoveScore = currentScore;
		}
		else if (!whiteTurn && currentScore < bestMoveScore) {
			bestMove = moves[i];
			bestMoveScore = currentScore;
		}
		currentBoard->unmakeMove();
		currentBoard->winner = -1;
		currentBoard->gameWon = false;
		currentBoard->takesForWhite = takesForWhite;
		currentBoard->takesForBlack = takesForBlack;
	}
	cout << "przeanalizowanych pozycji: " << analyzedPositions << endl;
	return bestMove;
}

coordinates advancedComputer::findBestMove(penteBoard * currentBoard, bool whiteTurn)
{
	//wygraj jednym ruchem
	vector<coordinates> winningMoves;
	winningMoves = analyzeBoard::analyzeForFourWithGap(currentBoard, whiteTurn);
	vector<chain> chains1 = analyzeBoard::analyzeForChains(currentBoard, whiteTurn, 4);
	array<vector<coordinates>, 5> chainsBlocks = analyzeBoard::blockingChains(currentBoard, chains1);
	winningMoves.insert(end(winningMoves), begin(chainsBlocks[0]), end(chainsBlocks[0]));
	if (winningMoves.size() != 0) {
		return (winningMoves[rand() % winningMoves.size()]);
	}
	analyzedPositions = 0;
	vector<coordinates> moves = generateMovesWorthChecking(currentBoard, whiteTurn);
	if (moves.size() == 0) {
		if (currentBoard->getMoveHistorySize() <= 1) {
			return generateFirstMoves(currentBoard, whiteTurn);
		}
		coordinates moveToTest;
		//sprobuj ulozyc pionek obok innego pionka
		array<vector<coordinates>, 8> adjacent = analyzeBoard::findFieldsAdjacentToPieces(currentBoard, whiteTurn);
		if (adjacent[0].size() != 0) {
			return (adjacent[0][rand() % adjacent[0].size()]);
		}
		do {
			moveToTest = { rand() % BOARDSIZE, rand() % BOARDSIZE };
		} while (currentBoard->board[moveToTest.y][moveToTest.x] != EMPTY);
		return moveToTest;
	}
	coordinates bestMove = moves[0];
	int bestMoveScore = (whiteTurn ? MININT : MAXINT);
	for (int i = 0; i < moves.size(); ++i) {
		int takesForWhite = currentBoard->takesForWhite;
		int takesForBlack = currentBoard->takesForBlack;
		currentBoard->makeMove(moves[i].x, moves[i].y);
		int currentScore = minMaxAlgorithm(currentBoard, 1, !whiteTurn);
		if (whiteTurn && currentScore > bestMoveScore) {
			bestMove = moves[i];
			bestMoveScore = currentScore;
		}
		else if (!whiteTurn && currentScore < bestMoveScore) {
			bestMove = moves[i];
			bestMoveScore = currentScore;
		}
		currentBoard->unmakeMove();
		currentBoard->winner = -1;
		currentBoard->gameWon = false;
		currentBoard->takesForWhite = takesForWhite;
		currentBoard->takesForBlack = takesForBlack;
	}
	cout << "przeanalizowanych pozycji: " << analyzedPositions << endl;
	return bestMove;
}
