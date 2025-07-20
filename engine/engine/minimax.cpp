#define _CRT_SECURE_NO_WARNINGS

#include "minimax.h"
#include "evaluation.h"
#include "util.h"
#include "socket.h"

#pragma warning(push, 4)

int currentId = 1;
int searchDepth = -1;

int Minimax(Game& game, const unsigned int depth, int parentId)
{
	bool isRoot = false;
	if (searchDepth == -1) { isRoot = true; }

	int nodeId = currentId;
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
	
	int colorMultiplier = GetColorMultiplier(game.toMove);
	if (!depth) 
	{ 
		int score = EvaluatePosition(game);
		SendNodeScore(nodeId, float(score));
		return score * colorMultiplier; //return evaluation relative to the side to move -> negative always bad / positive always good
	} 

	unsigned char gameState = GetGameState(game);
	if (!IsRunning(gameState)) 
	{ 
		int score = GetGameStateValue(gameState);
		SendNodeScore(nodeId, float(score));
		return score * colorMultiplier; 
	}

	//because the evaluation ist always relative, each side always wants to maximize their score (black would typically want to minimize)
	int bestScore = INT32_MIN;
	Move bestMove;
	int currentScore;
	for (Move move : game.GetLegalMoves())
	{
		game.MakeMove(move);
		currentScore = -Minimax(game, depth - 1, nodeId); //score of best enemy move
		if (currentScore > bestScore)
		{
			bestScore = currentScore;
			bestMove = move;
		}
		game.RevertMove();
	}
	game.bestMove = bestMove;
	SendNodeScore(nodeId, float(bestScore) * colorMultiplier);
	if (isRoot) { searchDepth = -1; }
	return bestScore;
}

#pragma warning(pop)