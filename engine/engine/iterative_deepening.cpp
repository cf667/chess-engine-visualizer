#define _CRT_SECURE_NO_WARNINGS

#include "iterative_deepening.h"

#include "minimax.h"
#include "util.h"

#pragma warning(push, 4)

Move TimeSearch(Game& game, std::chrono::steady_clock::time_point deadline, int* reachedDepth)
{
	Move result;
	int currentDepth = 1;
	while (true)
	{
		if (Minimax(game, currentDepth, -0x10000, 0x10000, 0, false, deadline) == BREAK_SEARCH) { break; }
		result = game.bestMove;
		if (reachedDepth) { *reachedDepth = currentDepth; }
		currentDepth++;
	}
	return result;
}

#pragma warning(pop)