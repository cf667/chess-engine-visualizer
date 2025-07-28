#pragma once
#include "game.h"

Move TimeSearch(Game& game, std::chrono::steady_clock::time_point deadline, int* reachedDepth = nullptr);