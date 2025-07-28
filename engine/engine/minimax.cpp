#define _CRT_SECURE_NO_WARNINGS

#include "minimax.h"
#include "evaluation.h"
#include "util.h"
#include "socket.h"

#pragma warning(push, 4)

int currentId = 1;
int searchDepth = -1;
int totalNodesSearched;

int Minimax(Game& game, const unsigned int depth, int alpha, int beta, int parentId, bool visualize)
{
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
			SendNewNode(nodeId, 0, 0, MoveToAlgebraic(game.moveHist.back()));
		}
		else
		{
			SendNewNode(nodeId, searchDepth - depth, parentId, MoveToAlgebraic(game.moveHist.back()));
		}
	}
	
	int colorMultiplier = GetColorMultiplier(game.toMove);
	if (!depth) 
	{ 
		int score = EvaluatePosition(game);
		if (visualize) { SendNodeScore(nodeId, float(score)); }
		return score * colorMultiplier; //return evaluation relative to the side to move -> negative always bad / positive always good
	} 

	unsigned char gameState = GetGameState(game);
	if (!IsRunning(gameState)) 
	{ 
		int score = GetGameStateValue(gameState);
		if (visualize) { SendNodeScore(nodeId, float(score)); }
		return score * colorMultiplier; 
	}

	//because the evaluation ist always relative, each side always wants to maximize their score (black would typically want to minimize)
	int bestScore = INT32_MIN;
	Move bestMove;
	int currentScore;
	for (Move move : game.GetLegalMoves())
	{
		game.MakeMove(move);
		currentScore = -Minimax(game, depth - 1, -beta, -alpha, nodeId, visualize); //score of best enemy move
		game.RevertMove();

		/*if (currentScore >= beta)
		{
			if (visualize) { SendNodeScore(nodeId, float(alpha) * colorMultiplier); }
			return beta;
		}

		if (currentScore > alpha)
		{
			alpha = currentScore;
			bestMove = move;
		}
		*/
		//this code is for normal minimax
		if (currentScore > bestScore)
		{
			bestScore = currentScore;
			bestMove = move;
		}
	}
	game.bestMove = bestMove;
	if (visualize)
	{
		SendNodeScore(nodeId, float(alpha) * colorMultiplier);
		//SendNodeScore(nodeId, float(bestScore) * colorMultiplier);
		if (isRoot) { searchDepth = -1; }
	}
	return bestScore;
}

#pragma warning(pop)