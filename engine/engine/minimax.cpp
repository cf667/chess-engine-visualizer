#define _CRT_SECURE_NO_WARNINGS
#include "minimax.h"
#include "evaluation.h"
#include "util.h"

#pragma warning(push, 4)

int Minimax(Game& game, const unsigned int depth)
{
	int colorMultiplier = GetColorMultiplier(game.toMove);
	if (!depth) { return EvaluatePosition(game) * colorMultiplier; } //return evaluation relative to the side to move -> negative always bad / positive always good

	unsigned char gameState = GetGameState(game);
	if (!IsRunning(gameState)) { return GetGameStateValue(gameState) * colorMultiplier; }

	//because the evaluation ist always relative, each side always wants to maximize their score (black would typically want to minimize)
	int bestScore = INT32_MIN;
	Move bestMove;
	int currentScore;
	for (Move move : game.GetLegalMoves())
	{
		game.MakeMove(move);
		currentScore = -Minimax(game, depth - 1); //score of best enemy move
		if (currentScore > bestScore)
		{
			bestScore = currentScore;
			bestMove = move;
		}
		game.RevertMove();
	}
	game.bestMove = bestMove;
	return bestScore;
}

#pragma warning(pop)