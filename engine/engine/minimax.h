#pragma once
#include "game.h"
#include "iterative_deepening.h"

extern int totalNodesSearched;

bool MoveComparator(Move a, Move b); //returns true if a is better than b, used for sorting moves in the move list

int Minimax(Game& game, const unsigned int depth, int alpha, int beta);