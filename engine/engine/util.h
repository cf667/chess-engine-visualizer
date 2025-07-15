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
constexpr unsigned int WQUEEN = 0x9;
constexpr unsigned int WPAWN = 0xA;
constexpr unsigned int WROOK = 0xB;
constexpr unsigned int WKNIGHT = 0xC;
constexpr unsigned int WBISHOP = 0xD;
constexpr unsigned int WKING = 0xE;
constexpr unsigned int BQUEEN = 0x1;
constexpr unsigned int BPAWN = 0x2;
constexpr unsigned int BROOK = 0x3;
constexpr unsigned int BKNIGHT = 0x4;
constexpr unsigned int BBISHOP = 0x5;
constexpr unsigned int BKING = 0x6;

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

void PrintPosition(unsigned char* pos);

std::string IndexToCoord(char i);

char CoordToIndex(const char* c);