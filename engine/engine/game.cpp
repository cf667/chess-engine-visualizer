#include "game.h"

#include <algorithm>
#include <iostream>
#include <bitset>

//codes for each piece
//first 3 bits:
#define KING 0x0
#define QUEEN 0x1
#define PAWN 0x2
#define ROOK 0x3
#define KNIGHT 0x4
#define BISHOP 0x5

#define WHITE 0x8 //4th bit
#define EMPTY 0x10 //5th bit
#define OUTOFBOUND 0x20 //6th bit

//first 4 bits
#define WKING 0x8
#define WQUEEN 0x9
#define WPAWN 0xA
#define WROOK 0xB
#define WKNIGHT 0xC
#define WBISHOP 0xD
#define BKING 0x0
#define BQUEEN 0x1
#define BPAWN 0x2
#define BROOK 0x3
#define BKNIGHT 0x4
#define BBISHOP 0x5

//moveTypes
#define QUIETMOVE 0x0
#define DOUBLEPAWNPUSH 0x1
#define CASTLE_KING 0x2
#define CASTLE_QUEEN 0x3
#define CAPTURE 0x4
#define ENPASSANT 0x5
#define PROMOTION_KNIGHT 0x8
#define PROMOTION_BISHOP 0x9
#define PROMOTION_ROOK 0xA
#define PROMOTION_QUEEN 0xB
#define PROMOTION_KNIGHT_CAPTURE 0xC
#define PROMOTION_BISHOP_CAPTURE 0xD
#define PROMOTION_ROOK_CAPTURE 0xE
#define PROMOTION_QUEEN_CAPTURE 0xF

//position at the start of every game
const unsigned char startingPos[120] =
{	OUTOFBOUND,	OUTOFBOUND,	OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND,
	OUTOFBOUND,	OUTOFBOUND,	OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND,
	OUTOFBOUND, BROOK,		BKNIGHT,	BBISHOP,	BQUEEN,		BKING,		BBISHOP,	BKNIGHT,	BROOK,		OUTOFBOUND,
	OUTOFBOUND,	BPAWN,		BPAWN,		BPAWN,		BPAWN,		BPAWN,		BPAWN,		BPAWN,		BPAWN,		OUTOFBOUND,
	OUTOFBOUND,	EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		OUTOFBOUND,
	OUTOFBOUND,	EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		OUTOFBOUND,
	OUTOFBOUND,	EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		OUTOFBOUND,
	OUTOFBOUND,	EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		OUTOFBOUND,
	OUTOFBOUND,	WPAWN,		WPAWN,		WPAWN,		WPAWN,		WPAWN,		WPAWN,		WPAWN,		WPAWN,		OUTOFBOUND,
	OUTOFBOUND, WROOK,		WKNIGHT,	WBISHOP,	WQUEEN,		WKING,		WBISHOP,	WKNIGHT,	WROOK,		OUTOFBOUND,
	OUTOFBOUND,	OUTOFBOUND,	OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND,
	OUTOFBOUND,	OUTOFBOUND,	OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND };

//offset for each move direction - pawns are handled separately
//most right bit in each piece code = slide
const std::vector<char> kingOffset =	{ -11, -10, -9, -1, 1, 9, 10, 11 };
const std::vector<char> queenOffset =	{ -11, -10, -9, -1, 1, 9, 10, 11 };
const std::vector<char> pawnOffset =	{ 0 };
const std::vector<char> rookOffset =	{ -10, -1, 1, 10 };
const std::vector<char> knightOffset =	{ -21, -19, -12, -8, 8, 12, 19, 21 };
const std::vector<char> bishopOffset =	{ -11, -9, 9, 11 };

const std::vector<std::vector<char>> offsets = { kingOffset, queenOffset, pawnOffset, rookOffset, knightOffset, bishopOffset };

void Move::Init(char origin, char destination, char flags, char capture)
{
	Move::origin = origin;
	Move::destination = destination;
	Move::flags = flags;
	Move::capture = capture;
}

Game::Game()
{
	std::copy(std::begin(startingPos), std::end(startingPos), std::begin(Game::position));
	Game::toMove = 1;
}

bool Game::MakeMove(Move move)
{
	position[move.destination] = position[move.origin];
	position[move.origin] = EMPTY;

	moveHist.push_back(move);

	return 1;
}

std::vector<Move> Game::GetLegalMoves()
{
	std::vector<Move> moveList;

	int curMoveIndex = 0;
	Move curMove;

	int pawnOffset;
	int doublePushRank;

	for (int i = 0; i < 120; i++) //iterate through board
	{
		if (!(i % 10))
		{
			std::cout << std::endl;
		}
		std::cout << i << " ";

		if ((Game::position[i] >> 5) & 1 || (Game::position[i] >> 4) & 1 || ((Game::position[i] >> 3) & 1) != Game::toMove) //if square is out of bound, empty or piece is the wrong color
		{
			continue;
		}

		if ((Game::position[i] & 0b111) == PAWN) //pawn
		{
			if ((Game::position[i] >> 3) & 1) //check for color
			{
				pawnOffset = -10;
				doublePushRank = 80;
			}
			else
			{
				pawnOffset = 10;
				doublePushRank = 30;
			}
			
			if ((Game::position[i + pawnOffset] >> 4) & 1) //normal move
			{
				curMove.Init(i, i + pawnOffset, QUIETMOVE, EMPTY);
				moveList.push_back(curMove);
				curMoveIndex++;

				if (i > doublePushRank && i < doublePushRank + 9 && (Game::position[i + pawnOffset * 2] >> 4) & 1) //double pawn push
				{
					curMove.Init(i, i + pawnOffset * 2, DOUBLEPAWNPUSH, EMPTY);
					moveList.push_back(curMove);
					curMoveIndex++;
				}
			}

			if (!((Game::position[i + pawnOffset + 1] >> 5) & 1) && !((Game::position[i + pawnOffset + 1] >> 4) & 1) && ((Game::position[i + pawnOffset + 1] >> 3) & 1) != Game::toMove) //right side capture
			{
				curMove.Init(i, i + pawnOffset + 1, CAPTURE, Game::position[i + pawnOffset + 1]);
				moveList.push_back(curMove);
				curMoveIndex++;
			}
			if (!((Game::position[i + pawnOffset + 1] >> 5) & 1) && !((Game::position[i + pawnOffset + 1] >> 4) & 1) && ((Game::position[i + pawnOffset - 1] >> 3) & 1) != Game::toMove) //left side capture
			{
				curMove.Init(i, i + pawnOffset - 1, CAPTURE, Game::position[i + pawnOffset - 1]);
				moveList.push_back(curMove);
				curMoveIndex++;
			}
		}
		else //every other piece
		{
			for (int offset : offsets[Game::position[i] & 0b111]) //iterate through moveset
			{
				if (Game::position[i] & 1) //check if piece can slide (queen, rook, bishop)
				{
					for (int slide = 1; !((Game::position[i + offset * slide] >> 5) & 1); slide++) //as long as its not off the board
					{
						if ((Game::position[i + offset * slide] >> 4) & 1) //empty square
						{
							curMove.Init(i, i + offset * slide, QUIETMOVE, EMPTY);
							moveList.push_back(curMove);
							curMoveIndex++;
						}
						else if (((Game::position[i + offset * slide] >> 3) & 1) != Game::toMove) //enemy piece
						{
							curMove.Init(i, i + offset * slide, CAPTURE, Game::position[i + offset * slide]);
							moveList.push_back(curMove);
							curMoveIndex++;
							break;
						}
						else //friendly piece
						{
							break;
						}
					}
				}
				else //pieces which cant slide (king, knight)
				{
					if ((Game::position[i + offset] >> 5) & 1) //out of bound
					{
						continue;
					}

					if ((Game::position[i + offset] >> 4) & 1) //empty square
					{
						curMove.Init(i, i + offset, QUIETMOVE, EMPTY);
						moveList.push_back(curMove);
						curMoveIndex++;
					}
					else if (((Game::position[i + offset] >> 3) & 1) != Game::toMove) //enemy piece
					{
						curMove.Init(i, i + offset, CAPTURE, Game::position[i + offset]);
						moveList.push_back(curMove);
						curMoveIndex++;
					}
				}
			}
		}
	}

	return moveList;
}

unsigned char* printPosition(unsigned char* pos)
{
	unsigned char result[64];
	for (int i = 2; i < 10; i++)
	{
		std::copy(startingPos + i * 10 + 1, startingPos + i * 10 + 9, result + (i - 2) * 8);
	}

	for (int i = 0; i < 64; i++)
	{
		if (!(i % 8)) { std::cout << "\n"; };
		switch (result[i])
		{
		case WKING:
			std::cout << 'K';
			break;
		case WQUEEN:
			std::cout << 'Q';
			break;
		case WPAWN:
			std::cout << 'P';
			break;
		case WROOK:
			std::cout << 'R';
			break;
		case WKNIGHT:
			std::cout << 'N';
			break;
		case WBISHOP:
			std::cout << 'B';
			break;
		case BKING:
			std::cout << 'k';
			break;
		case BQUEEN:
			std::cout << 'q';
			break;
		case BPAWN:
			std::cout << 'p';
			break;
		case BROOK:
			std::cout << 'r';
			break;
		case BKNIGHT:
			std::cout << 'n';
			break;
		case BBISHOP:
			std::cout << 'b';
			break;
		case EMPTY:
			std::cout << '.';
			break;
		}
	}
	return result;
}