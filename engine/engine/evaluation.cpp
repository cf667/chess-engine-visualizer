#define _CRT_SECURE_NO_WARNINGS

#include "evaluation.h"
#include "util.h"
#include "transposition_table.h"

#pragma warning(push, 4)

bool IsThreefoldRepetition(Game& game) 
{ 
	if (transpositionTable[game.hashKey].repetition == 3) { return 1; }
	else { return 0; }
}

unsigned char GetGameState(Game& game)
{
	if (!game.GetLegalMoves().count)
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

	if (transpositionTable[game.hashKey].repetition == 3) { return GAMESTATE_THREEFOLDREPETITION; }

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

float EvaluatePosition(Game& game)
{
	unsigned char gameState = GetGameState(game);
	if (!IsRunning(gameState)) { return GetGameStateValue(gameState); }

	float result = 0;
	result += float(EvaluateMaterial(game));
	return result;
}

#pragma warning(pop)