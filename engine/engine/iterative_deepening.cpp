#define _CRT_SECURE_NO_WARNINGS

#include "iterative_deepening.h"

#include "minimax.h"
#include "util.h"

#pragma warning(push, 4)

int TimeSearch(Game& game, std::chrono::steady_clock::time_point deadline, int* reachedDepth)
{
	int result = 0;
	Move bestMove;
	int currentScore = 0;
	int currentDepth = 1;
	while (true)
	{
		currentScore = Minimax(game, currentDepth, -0x10000, 0x10000, 0, false, deadline);
		if (currentScore == BREAK_SEARCH) { break; }

		result = currentScore;
		bestMove = game.bestMove;
		if (reachedDepth) { *reachedDepth = currentDepth; }
		currentDepth++;
	}
	game.bestMove = bestMove;
	return result;
}

#pragma warning(pop)