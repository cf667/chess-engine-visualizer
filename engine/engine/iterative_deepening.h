#pragma once
#include "game.h"

float TimeSearch(Game& game, std::chrono::milliseconds searchTime, int* reachedDepth = nullptr);