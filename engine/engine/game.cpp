#include "game.h"

#include <algorithm>
#include <iostream>
#include <bitset>

#define KING 0x0
#define QUEEN 0x1
#define PAWN 0x2
#define ROOK 0x3
#define KNIGHT 0x4
#define BISHOP 0x5
#define WHITE 0x8
#define EMPTY 0x10
#define OUTOFBOUND 0x20

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

Game::Game()
{
	std::cout << "constructor" << std::endl;
	std::copy(std::begin(startingPos), std::end(startingPos), std::begin(position));
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