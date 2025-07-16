#pragma once
#include <string>

//codes for each piece
//first 3 bits:
constexpr unsigned char QUEEN = 0x1;
constexpr unsigned char PAWN = 0x2;
constexpr unsigned char ROOK = 0x3;
constexpr unsigned char KNIGHT = 0x4;
constexpr unsigned char BISHOP = 0x5;
constexpr unsigned char KING = 0x6;

constexpr unsigned char WHITE = 0x8; //4th bit
constexpr unsigned char EMPTY = 0x10; //5th bit
constexpr unsigned char OUTOFBOUND = 0x20; //6th bit

//first 4 bits
constexpr unsigned char WQUEEN = 0x9;
constexpr unsigned char WPAWN = 0xA;
constexpr unsigned char WROOK = 0xB;
constexpr unsigned char WKNIGHT = 0xC;
constexpr unsigned char WBISHOP = 0xD;
constexpr unsigned char WKING = 0xE;
constexpr unsigned char BQUEEN = 0x1;
constexpr unsigned char BPAWN = 0x2;
constexpr unsigned char BROOK = 0x3;
constexpr unsigned char BKNIGHT = 0x4;
constexpr unsigned char BBISHOP = 0x5;
constexpr unsigned char BKING = 0x6;

//bitmasks for piece types
constexpr unsigned char PIECE_BITMASK = 0b00000111;
constexpr unsigned char COLOR_BITMASK = 0b00001000;
constexpr unsigned char EMPTY_BITMASK = 0b00010000;
constexpr unsigned char OUTOFBOUND_BITMASK = 0b00100000;

//helper functions for bitmasks
constexpr unsigned char GetPiece(unsigned char square)
{
	return square & PIECE_BITMASK;
}

constexpr bool IsWhite(unsigned char square)
{
	return square & COLOR_BITMASK;
}
constexpr signed char GetColorMultiplier(unsigned char square)
{
	if (square & COLOR_BITMASK) { return 1; }
	else { return -1; }
}

constexpr bool IsEmpty(unsigned char square)
{
	return square & EMPTY_BITMASK;
}

constexpr bool IsOutOfBound(unsigned char square)
{
	return square & OUTOFBOUND_BITMASK;
}

//moveTypes
constexpr unsigned int QUIETMOVE = 0x0;
constexpr unsigned int DOUBLEPAWNPUSH = 0x1;
constexpr unsigned int CASTLE_KING = 0x2;
constexpr unsigned int CASTLE_QUEEN = 0x3;
constexpr unsigned int CAPTURE = 0x4;
constexpr unsigned int ENPASSANT = 0x5;
constexpr unsigned int PROMOTION_KNIGHT = 0x8;
constexpr unsigned int PROMOTION_BISHOP = 0x9;
constexpr unsigned int PROMOTION_ROOK = 0xA;
constexpr unsigned int PROMOTION_QUEEN = 0xB;
constexpr unsigned int PROMOTION_KNIGHT_CAPTURE = 0xC;
constexpr unsigned int PROMOTION_BISHOP_CAPTURE = 0xD;
constexpr unsigned int PROMOTION_ROOK_CAPTURE = 0xE;
constexpr unsigned int PROMOTION_QUEEN_CAPTURE = 0xF;

//castling ability
constexpr unsigned int BKCASTLE = 0x1; //white king side - 1st bit
constexpr unsigned int BQCASTLE = 0x2; //white queen side - 2nd bit
constexpr unsigned int WKCASTLE = 0x4; //black king side - 3rd bit
constexpr unsigned int WQCASTLE = 0x8; //black queen side - 4th bit

//values
constexpr unsigned char PIECE_VALUES[] = { 0, 9, 1, 5, 3, 3 };

void PrintPosition(unsigned char* pos);

std::string IndexToCoord(char i);

char CoordToIndex(const char* c);