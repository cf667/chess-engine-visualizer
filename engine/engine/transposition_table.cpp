#include "transposition_table.h"

#include "util.h"

std::unordered_map<uint64_t, SearchInfo> transpositionTable;

uint64_t GenerateKey(Game& game)
{
	uint64_t result = 0;
	for (int i = 0; i < 64; i++)
	{
		const char square = game.position[translateToBigBoard[i]];
		if (GetPiece(square))
		{
			result ^= zobrist::position[PieceToHashIndex(square)][i];
		}
	}
	if (!game.toMove) { result ^= zobrist::blackToMove; }
	result ^= zobrist::castlingRights[game.gameRules.castlingAbility];
	if (game.gameRules.enPassantTarget) { result ^= zobrist::enPassantFile[(game.gameRules.enPassantTarget % 10) - 1]; }

	return result;
}