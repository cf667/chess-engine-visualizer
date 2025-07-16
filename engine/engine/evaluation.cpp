#define _CRT_SECURE_NO_WARNINGS
#include "evaluation.h"
#include "util.h"

#pragma warning(push, 4)

int EvaluateMaterial(Game& game)
{
	int result = 0;
	for (unsigned char square : game.position)
	{
		result += PIECE_VALUES[GetPiece(square)] * GetColorMultiplier(square);
	}
	return result;
}

int EvaluatePosition(Game& game)
{
	int result = 0;
	result += EvaluateMaterial(game);
	return result;
}

#pragma warning(pop)