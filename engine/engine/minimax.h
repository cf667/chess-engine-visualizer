#pragma once
#include "game.h"
#include "iterative_deepening.h"

class Node
{
public:
	int id;
	int depth;
	int parentId;
	std::string previousMove;
	int score;
	int childCount;
	bool isCutoff;
};

class NodeChunk
{
public:
	int count = 0;
	Node list[209];

	void AddNode(Node node);
	void Send();
};

extern int totalNodesSearched;

bool MoveComparator(Move a, Move b); //returns true if a is better than b, used for sorting moves in the move list

int Minimax(Game& game, const unsigned int depth, int alpha, int beta, int parentId = 0, std::string previousMove = "");