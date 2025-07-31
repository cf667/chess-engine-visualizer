#pragma once
#include "game.h"

float TimeSearch(Game& game, std::chrono::steady_clock::time_point deadline, int* reachedDepth = nullptr);