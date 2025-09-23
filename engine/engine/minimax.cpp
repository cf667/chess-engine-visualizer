#define _CRT_SECURE_NO_WARNINGS

#include "minimax.h"
#include "evaluation.h"
#include "util.h"
#include "socket.h"
#include "transposition_table.h"
#include "engine.h"

#pragma warning(push, 4)

int currentId = 1;
int searchDepth = -1;
int totalNodesSearched;

bool MoveComparator(Move a, Move b)
{
	// if (IsCapture(b.flags)) { return false; }	<- old sorting

	if (IsCapture(b.flags) != IsCapture(a.flags))
	{
		return IsCapture(a.flags);
	}

	if (IsCapture(b.flags))
	{
		return PIECE_VALUES[GetPiece(a.capture)] > PIECE_VALUES[GetPiece(b.capture)];
	}

	return false;
}

int Minimax(Game& game, const unsigned int depth, int alpha, int beta)
{
	int originalAlpha = alpha;

	if (!engineSettings.searchByDepth && std::chrono::steady_clock::now() >= engineSettings.searchDeadline)
	{ 
		return BREAK_SEARCH; 
	}
	totalNodesSearched++;

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
		int score = EvaluatePosition(game);
		return score * colorMultiplier; //return evaluation relative to the side to move -> negative always bad / positive always good
	} 

	unsigned char gameState = GetGameState(game);
	if (!IsRunning(gameState)) 
	{ 
		int score = GetGameStateValue(gameState);
		return score * colorMultiplier; 
	}

	//because the evaluation is always relative, each side always wants to maximize their score (black would typically want to minimize)
	int bestScore = INT32_MIN;
	Move bestMove;
	int currentScore;
	MoveList moveList = game.GetLegalMoves();
	bestMove = transpositionTable[game.hashKey].bestMove;
	std::sort(moveList.list, moveList.list + moveList.count, MoveComparator); //sort moves so that captures are always first
	Move move;

	for (int i = 0; i < moveList.count; i++)
	{
		move = moveList.list[i];

		Game gameCopy = game;
		game.MakeMove(move);

		currentScore = -Minimax(game, depth - 1, -beta, -alpha); //score of best enemy move

		transpositionTable[game.hashKey].repetition--;
		game = gameCopy;

		if (currentScore == -BREAK_SEARCH) { return BREAK_SEARCH; }

		if (currentScore >= beta)
		{
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