#define _CRT_SECURE_NO_WARNINGS

#include "evaluation.h"
#include "util.h"
#include "zobrist_hashing.h"

#pragma warning(push, 4)

bool IsThreefoldRepetition(Game& game) 
{ 
	if (hashMap[game.hashKey] == 3) { return 1; }
	else { return 0; }
}

unsigned char GetGameState(Game& game)
{
	if (!game.GetLegalMoves().size())
	{
		bool isMate = game.IsCheck(game.toMove);

		/*bool isMate = game.IsCheck(!game.toMove);*/

		if (isMate)
		{
			if (game.toMove) { return GAMESTATE_MATE_BLACK; }
			else { return GAMESTATE_MATE_WHITE; }
		}
		else { return GAMESTATE_STALEMATE; }
	}

	if (game.gameRules.halfMoveCounter > 99) { return GAMESTATE_FIFTYMOVERULE; }

	if (hashMap[game.hashKey] == 3) { return GAMESTATE_THREEFOLDREPETITION; }

	return GAMESTATE_RUNNING;
}

int EvaluateMaterial(Game& game)
{
	int result = 0;
	for (unsigned char square : game.position)
	{
		result += PIECE_VALUES[GetPiece(square)] * GetColorMultiplier(IsWhite(square));
	}
	return result;
}

int EvaluatePosition(Game& game)
{
	unsigned char gameState = GetGameState(game);
	if (!IsRunning(gameState)) { return GetGameStateValue(gameState); }

	int result = 0;
	result += EvaluateMaterial(game);
	return result;
}

#pragma warning(pop)