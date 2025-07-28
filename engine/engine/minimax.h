#pragma once
#include "game.h"

class SearchTreeNode
{

};

class SearchTree
{

};

extern int totalNodesSearched;

int Minimax(Game& game, const unsigned int depth, int alpha = -0x10000, int beta = 0x10000, int parentId = 0, bool visualize = 0); //returns score of position relative to the side to move