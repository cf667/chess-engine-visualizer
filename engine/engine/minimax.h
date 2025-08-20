#pragma once
#include "game.h"
#include "iterative_deepening.h"

extern int totalNodesSearched;

bool MoveComparator(Move a, Move b); //returns true if a is better than b, used for sorting moves in the move list

float Minimax(Game& game, const unsigned int depth, float alpha = -0x10000, float beta = 0x10000, int parentId = 0, bool visualize = 0, std::optional<std::chrono::steady_clock::time_point> deadline = std::nullopt); //returns score of position relative to the side to move