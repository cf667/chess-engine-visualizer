#define _CRT_SECURE_NO_WARNINGS

#include "minimax.h"
#include "evaluation.h"
#include "util.h"
#include "socket.h"
#include "transposition_table.h"
#include "engine.h"

#pragma warning(push, 4)

int currentId = 0;
int totalNodesSearched;

void NodeChunk::AddNode(Node node)
{
	if (NodeChunk::count >= 209)
	{
		NodeChunk::Send();
	}

	NodeChunk::list[NodeChunk::count] = node;
	NodeChunk::count++;

	if (node.depth == 0)
	{
		NodeChunk::Send();
		currentId = 0;
	}
}

void NodeChunk::Send()
{
	if (!engineSettings.connectedToGUI)
	{
		NodeChunk::count = 0;
		return;
	}

	json message;
	message["id"] = ID_NODECHUNK;
	
	json nodes = json::array();
	for (int i = 0; i < NodeChunk::count; i++)
	{
		Node& node = NodeChunk::list[i];

		json jNode;
		jNode["id"] = node.id;
		jNode["depth"] = node.depth;
		jNode["parentId"] = node.parentId;
		jNode["previousMove"] = node.previousMove;
		jNode["score"] = node.score;
		jNode["childCount"] = node.childCount;
		jNode["isCutoff"] = node.isCutoff;

		nodes.push_back(jNode);
	}

	message["nodes"] = nodes;
	socketLoop->defer([message]() { mySocket->send(message.dump(), uWS::OpCode::TEXT); });

	NodeChunk::count = 0;
	return;
}

NodeChunk nodeChunk;

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

int Minimax(Game& game, const unsigned int depth, int alpha, int beta, int parentId, std::string previousMove)
{
	if (!engineSettings.searchByDepth && std::chrono::steady_clock::now() >= engineSettings.searchDeadline)
	{ 
		return BREAK_SEARCH; 
	}

	totalNodesSearched++;
	currentId++;

	Node node;
	node.id = currentId;
	node.depth = engineSettings.searchDepth - depth;
	node.parentId = parentId;
	node.previousMove = previousMove;
	node.childCount = 0;
	node.isCutoff = false;

	int originalAlpha = alpha;

	int colorMultiplier = GetColorMultiplier(game.toMove);

	/*SearchInfo* searchInfo = &transpositionTable[game.hashKey];
	if (searchInfo->depth >= depth)
	{
		switch (searchInfo->flag)
		{
		case EXACT:
			game.bestMove = searchInfo->bestMove;

			node.score = searchInfo->score * colorMultiplier;
			nodeChunk.AddNode(node);
			return searchInfo->score;

			break;

		case LOWERBOUND:
			if (searchInfo->score >= alpha) 
			{ 
				game.bestMove = searchInfo->bestMove;

				node.score = searchInfo->score * colorMultiplier;
				nodeChunk.AddNode(node);
				return searchInfo->score; 
			}
			break;

		case UPPERBOUND:
			if (searchInfo->score <= beta) 
			{ 
				game.bestMove = searchInfo->bestMove;

				node.score = searchInfo->score * colorMultiplier;
				nodeChunk.AddNode(node);
				return searchInfo->score; 
			}
			break;
		}
	}*/
	
	if (!depth) 
	{ 
		int score = EvaluatePosition(game);

		node.score = score;
		nodeChunk.AddNode(node);
		return score * colorMultiplier; //return evaluation relative to the side to move -> negative always bad / positive always good
	} 

	unsigned char gameState = GetGameState(game);
	if (!IsRunning(gameState)) 
	{ 
		int score = GetGameStateValue(gameState);

		node.score = score;
		nodeChunk.AddNode(node);
		return score * colorMultiplier; 
	}

	//because the evaluation is always relative, each side always wants to maximize their score (black would typically want to minimize)
	int bestScore = INT32_MIN;
	int currentScore;
	MoveList moveList = game.GetLegalMoves();
	Move bestMove = moveList.list[0];
	if (transpositionTable[game.hashKey].bestMove.origin != -1)
	{
		bestMove = transpositionTable[game.hashKey].bestMove;
	}
	
	if (engineSettings.useMoveSorting)
	{
		std::sort(moveList.list, moveList.list + moveList.count, MoveComparator); //sort moves so that captures are always first
	}
	
	node.childCount = moveList.count;
	Move move;

	for (int i = 0; i < moveList.count; i++)
	{
		move = moveList.list[i];

		Game gameCopy = game;
		game.MakeMove(move);

		currentScore = -Minimax(game, depth - 1, -beta, -alpha, node.id, MoveToAlgebraic(move)); //score of best enemy move

		transpositionTable[game.hashKey].repetition--;
		game = gameCopy;

		if (currentScore == -BREAK_SEARCH) { return BREAK_SEARCH; }

		if (engineSettings.useAlphaBetaPruning && currentScore >= beta)
		{
			/*if (searchInfo->depth < depth) 
			{
				searchInfo->score = beta;
				searchInfo->flag = LOWERBOUND;
				searchInfo->bestMove = move;
				searchInfo->depth = depth; 
			}*/

			game.bestMove = move;

			node.score = beta * colorMultiplier;
			node.isCutoff = true;
			nodeChunk.AddNode(node);
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

	/*if (searchInfo->depth < depth)
	{
		searchInfo->score = alpha;
		searchInfo->bestMove = bestMove;
		searchInfo->depth = depth;
	}*/

	/*if (alpha <= originalAlpha) { searchInfo->flag = UPPERBOUND; }
	else { searchInfo->flag = EXACT; }*/

	node.score = alpha * colorMultiplier;
	nodeChunk.AddNode(node);
	return alpha;
}

#pragma warning(pop)