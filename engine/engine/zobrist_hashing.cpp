#include "zobrist_hashing.h"

#include "util.h"

std::unordered_map<uint64_t, int> hashMap;

uint64_t GenerateKey(Game& game)
{
	uint64_t result = 0;
	for (int i = 0; i < 120; i++)
	{
		if (GetPiece(game.position[i]))
		{
			result ^= zobrist::position[PieceToHashIndex(game.position[i])][i];
		}
	}
	if (!game.toMove) { result ^= zobrist::blackToMove; }
	result ^= zobrist::castlingRights[game.gameRules.castlingAbility];
	if (game.gameRules.enPassantTarget) { result ^= zobrist::enPassantFile[((game.gameRules.enPassantTarget % 10) - 1)]; }

	return result;
}