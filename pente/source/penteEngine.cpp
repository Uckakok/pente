#include "penteEngine.h"


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

	//sprawdzenie czy mo¿na u³o¿yæ czwórkê odblokowan¹ z dwóch stron (priorytet 2)
	chainsBlocks = analyzeBoard::blockingChains(currentBoard, chains2);
	allMovesWithPriorities[2].insert(end(allMovesWithPriorities[2]), begin(chainsBlocks[1]), end(chainsBlocks[1]));

	//zablokowanie trójki przeciwnika odblokowanej z obu stron (priorytet 3)
	allMovesWithPriorities[3].insert(end(allMovesWithPriorities[3]), begin(chainsBlocksEnemy[1]), end(chainsBlocksEnemy[1]));


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
	allMovesWithPriorities[13].insert(end(allMovesWithPriorities[13]), begin(chainsBlocks[3]), end(chainsBlocks[3]));
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

coordinates advancedComputer::findBestMove(penteBoard * currentBoard, bool whiteTurn)
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
		cout << "Nie istnieje jeszcze latwe AI" << endl;
		instance = new advancedComputer();
		break;
	case 1:
		cout << "Nie istnieje jeszcze srednie AI" << endl;
		instance = new advancedComputer();
		break;
	case 2:
		instance = new advancedComputer();
		break;
	case 3:
		cout << "Nie istnieje jeszcze eksperckie AI" << endl;
		instance = new advancedComputer();
		break;
	case 4:
		cout << "Nie istnieje jeszcze mistrzowskie AI" << endl;
		instance = new advancedComputer();
		break;
	default:
		cout << "Nierozpoznany poziom trudnosci AI" << endl;
		instance = new advancedComputer();
		break;
	}
	return instance;
}
