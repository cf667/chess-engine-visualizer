#define _CRT_SECURE_NO_WARNINGS

#include "iterative_deepening.h"

#include "minimax.h"
#include "util.h"
#include "engine.h"

#pragma warning(push, 4)

float TimeSearch(Game& game, std::chrono::milliseconds searchTime, int* reachedDepth)
{
	int result = 0;
	Move bestMove;
	int currentScore = 0;
	int currentDepth = 1;
	bool oldSearch = engineSettings.searchByDepth;

	engineSettings.searchByDepth = false;
	engineSettings.searchDeadline = std::chrono::high_resolution_clock::now() + searchTime;

	while (true)
	{
		currentScore = Minimax(game, currentDepth, -0x10000, 0x10000);
		if (currentScore == BREAK_SEARCH) { break; }

		result = currentScore;
		bestMove = game.bestMove;
		if (reachedDepth) { *reachedDepth = currentDepth; }
		currentDepth++;
	}

	game.bestMove = bestMove;
	engineSettings.searchByDepth = oldSearch;
	return result;
}

#pragma warning(pop)