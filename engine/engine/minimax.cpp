#define _CRT_SECURE_NO_WARNINGS

#include "minimax.h"
#include "evaluation.h"
#include "util.h"
#include "socket.h"
#include "transposition_table.h"

#pragma warning(push, 4)

int currentId = 1;
int searchDepth = -1;
int totalNodesSearched;

Move bestMove;

bool MoveComparator(Move a, Move b)
{
	if (IsCapture(b.flags)) { return false; } //always prefer captures
	if (b.origin == bestMove.origin && b.destination == bestMove.destination && b.flags == bestMove.flags) { return false; }
	return true;
}

float Minimax(Game& game, const unsigned int depth, float alpha, float beta, int parentId, bool visualize, std::optional<std::chrono::steady_clock::time_point> deadline)
{
	float originalAlpha = alpha;

	if (deadline.has_value() && std::chrono::steady_clock::now() >= deadline.value()) 
	{ 
		return BREAK_SEARCH; 
	}
	totalNodesSearched++;
	int nodeId = 0;
	bool isRoot;
	if (visualize)
	{
		isRoot = false;
		if (searchDepth == -1) { isRoot = true; }

		nodeId = currentId;
		currentId++;
		if (isRoot)
		{
			searchDepth = depth;
			//SendNewNode(nodeId, 0, 0, MoveToAlgebraic(game.moveHist.back()));
		}
		else
		{
			//SendNewNode(nodeId, searchDepth - depth, parentId, MoveToAlgebraic(game.moveHist.back()));
		}
	}

	SearchInfo* searchInfo = &transpositionTable[game.hashKey];
	if (searchInfo->depth >= depth)
	{
		switch (searchInfo->flag)
		{
		case EXACT:
			game.bestMove = searchInfo->bestMove;
			return searchInfo->score;
			break;

		case LOWERBOUND:
			if (searchInfo->score >= alpha) 
			{ 
				game.bestMove = searchInfo->bestMove;
				return searchInfo->score; 
			}
			break;

		case UPPERBOUND:
			if (searchInfo->score <= beta) 
			{ 
				game.bestMove = searchInfo->bestMove;
				return searchInfo->score; 
			}
			break;
		}
	}
	
	int colorMultiplier = GetColorMultiplier(game.toMove);
	if (!depth) 
	{ 
		float score = EvaluatePosition(game);
		if (visualize) { SendNodeScore(nodeId, float(score)); }
		return score * colorMultiplier; //return evaluation relative to the side to move -> negative always bad / positive always good
	} 

	unsigned char gameState = GetGameState(game);
	if (!IsRunning(gameState)) 
	{ 
		float score = GetGameStateValue(gameState);
		if (visualize) { SendNodeScore(nodeId, float(score)); }
		return score * colorMultiplier; 
	}

	//because the evaluation is always relative, each side always wants to maximize their score (black would typically want to minimize)
	int bestScore = INT32_MIN;
	Move bestMove;
	float currentScore;
	MoveList moveList = game.GetLegalMoves();
	bestMove = transpositionTable[game.hashKey].bestMove;
	std::sort(moveList.list, moveList.list + moveList.count, MoveComparator); //sort moves so that captures are always first
	Move move;

	for (int i = 0; i < moveList.count; i++)
	{
		move = moveList.list[i];

		Game gameCopy = game;
		game.MakeMove(move);

		currentScore = -Minimax(game, depth - 1, -beta, -alpha, nodeId, visualize, deadline); //score of best enemy move

		transpositionTable[game.hashKey].repetition--;
		game = gameCopy;

		if (currentScore == -BREAK_SEARCH) { return BREAK_SEARCH; }

		if (currentScore >= beta)
		{
			if (visualize) { SendNodeScore(nodeId, alpha * colorMultiplier); }

			if (searchInfo->depth < depth) 
			{
				searchInfo->score = beta;
				searchInfo->flag = LOWERBOUND;
				searchInfo->bestMove = move;
				searchInfo->depth = depth; 
			}

			game.bestMove = move;

			return beta;
		}

		if (currentScore > alpha)
		{
			alpha = currentScore;
			bestMove = move;
		}
		
		//this code is for normal minimax
		/*if (currentScore > bestScore)
		{
			bestScore = currentScore;
			bestMove = move;
		}*/
	}

	if (visualize)
	{
		SendNodeScore(nodeId, float(alpha) * colorMultiplier);
		//SendNodeScore(nodeId, float(bestScore) * colorMultiplier);
		if (isRoot) { searchDepth = -1; }
	}

	game.bestMove = bestMove;

	if (searchInfo->depth < depth)
	{
		searchInfo->score = alpha;
		searchInfo->bestMove = bestMove;
		searchInfo->depth = depth;
	}

	if (alpha <= originalAlpha) { searchInfo->flag = UPPERBOUND; }
	else { searchInfo->flag = EXACT; }

	return alpha;
}

#pragma warning(pop)