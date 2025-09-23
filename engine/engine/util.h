#pragma once
#include <string>

#include "game.h"

constexpr int translateToBigBoard[64] = {
	21, 22, 23, 24, 25, 26, 27, 28,
	31, 32, 33, 34, 35, 36, 37, 38,
	41, 42, 43, 44, 45, 46, 47, 48,
	51, 52, 53, 54, 55, 56, 57, 58,
	61, 62, 63, 64, 65, 66, 67, 68,
	71, 72, 73, 74, 75, 76, 77, 78,
	81, 82, 83, 84, 85, 86, 87, 88,
	91, 92, 93, 94, 95, 96, 97, 98,
};

constexpr int translateToSmallBoard[120] = {
	-1,	-1,	-1, -1, -1, -1, -1, -1, -1, -1,
	-1,	-1,	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, 0,	1,	2,	3,	4,	5,	6,	7,	-1,
	-1,	8,	9,	10,	11,	12,	13,	14,	15,	-1,
	-1,	16,	17,	18,	19,	20,	21,	22,	23,	-1,
	-1,	24,	25,	26,	27,	28,	29,	30,	31,	-1,
	-1,	32,	33,	34,	35,	36,	37,	38,	39,	-1,
	-1,	40,	41,	42,	43,	44,	45,	46,	47,	-1,
	-1,	48,	49,	50,	51,	52,	53,	54,	55,	-1,
	-1, 56,	57,	58,	59,	60,	61,	62,	63,	-1,
	-1,	-1,	-1, -1, -1, -1, -1, -1, -1, -1,
	-1,	-1,	-1, -1, -1, -1, -1, -1, -1, -1
};

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
constexpr unsigned char BQUEEN = 0x1;
constexpr unsigned char BPAWN = 0x2;
constexpr unsigned char BROOK = 0x3;
constexpr unsigned char BKNIGHT = 0x4;
constexpr unsigned char BBISHOP = 0x5;
constexpr unsigned char BKING = 0x6;
constexpr unsigned char WQUEEN = 0x9;
constexpr unsigned char WPAWN = 0xA;
constexpr unsigned char WROOK = 0xB;
constexpr unsigned char WKNIGHT = 0xC;
constexpr unsigned char WBISHOP = 0xD;
constexpr unsigned char WKING = 0xE;

//helper functions
constexpr unsigned char GetPiece(unsigned char square)
{
	return square & 0b00000111;
}

constexpr bool IsWhite(unsigned char square)
{
	return square & 0b00001000;
}
constexpr signed char GetColorMultiplier(bool color)
{
	if (color) { return 1; }
	else { return -1; }
}

constexpr bool IsEmpty(unsigned char square)
{
	return square & 0b00010000;
}

constexpr bool IsOutOfBound(unsigned char square)
{
	return square & 0b00100000;
}

constexpr std::string IndexToCoord(char i)
{
	std::string result = "  ";
	result[0] = 'a' + ((i % 10) - 1);
	result[1] = '0' + 8 - ((i / 10) - 2);
	return result;
}

constexpr char CoordToIndex(const char* c)
{
	return (120 - ((c[1] - '0' + 2) * 10)) + (c[0] - 'a' + 1);
}

//hash indecies
constexpr unsigned char I_BQUEEN = 0x0;
constexpr unsigned char I_BPAWN = 0x1;
constexpr unsigned char I_BROOK = 0x2;
constexpr unsigned char I_BKNIGHT = 0x3;
constexpr unsigned char I_BBISHOP = 0x4;
constexpr unsigned char I_BKING = 0x5;
constexpr unsigned char I_WQUEEN = 0x6;
constexpr unsigned char I_WPAWN = 0x7;
constexpr unsigned char I_WROOK = 0x8;
constexpr unsigned char I_WKNIGHT = 0x9;
constexpr unsigned char I_WBISHOP = 0xA;
constexpr unsigned char I_WKING = 0xB;

constexpr char PieceToHashIndex(char piece)
{
	if (IsWhite(piece)) { return piece - 3; }
	else { return piece - 1; }
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

constexpr bool IsCapture(unsigned char moveFlags)
{
	return moveFlags & 0b00000100;
}

constexpr bool IsPromotion(unsigned char moveFlags)
{
	return moveFlags & 0b00001000;
}

constexpr char GetPromotionType(unsigned char moveFlags)
{
	return moveFlags & 0b00001011;
}

constexpr char CharToPromotionType(const char c)
{
	switch (c)
	{
	case 'n':
		return PROMOTION_KNIGHT;
		break;
	case 'b':
		return PROMOTION_BISHOP;
		break;
	case 'r':
		return PROMOTION_ROOK;
		break;
	case 'q':
		return PROMOTION_QUEEN;
		break;
	default:
		return QUIETMOVE;
	}
}

constexpr char PromotionTypeToChar(const char c)
{
	switch (c)
	{
	case PROMOTION_KNIGHT:
		return 'n';
		break;
	case PROMOTION_BISHOP:
		return 'b';
		break;
	case PROMOTION_ROOK:
		return 'r';
		break;
	case PROMOTION_QUEEN:
		return 'q';
		break;
	default:
		return ' ';
	}
}

constexpr std::string MoveToAlgebraic(Move move)
{
	std::string result;
	result.append(IndexToCoord(move.origin));
	result.append(IndexToCoord(move.destination));

	if (IsPromotion(move.flags))
	{
		result.push_back(PromotionTypeToChar(GetPromotionType(move.flags)));
	}

	return result;
}

//constexpr Move AlgebraicToMove(std::string algebraicNotation)
//{
//	
//}

//castling ability
constexpr unsigned int BKCASTLE = 0x1; //white king side - 1st bit
constexpr unsigned int BQCASTLE = 0x2; //white queen side - 2nd bit
constexpr unsigned int WKCASTLE = 0x4; //black king side - 3rd bit
constexpr unsigned int WQCASTLE = 0x8; //black queen side - 4th bit

constexpr void DisableCastlingWhite(char& castlingRights) { castlingRights &= 0b00000011; }
constexpr void DisableCastlingBlack(char& castlingRights) { castlingRights &= 0b00001100; }

constexpr void DisableCastlingBlackKing(char& castlingRights) { castlingRights &= 0b00001110; }
constexpr void DisableCastlingBlackQueen(char& castlingRights) { castlingRights &= 0b00001101; }
constexpr void DisableCastlingWhiteKing(char& castlingRights) { castlingRights &= 0b00001011; }
constexpr void DisableCastlingWhiteQueen(char& castlingRights) { castlingRights &= 0b00000111; }

//values
constexpr unsigned int PIECE_VALUES[] = { 0, 900, 100, 500, 300, 300, 20000 };

//square tables
constexpr signed int PIECE_TABLE[2][7][120] =
{
	{
		{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		},
		{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, -20, -10, -10, -5, -5, -10, -10, -20, 0,
			0, -10, 0, 5, 0, 0, 0, 0, -10, 0,
			0, -10, 5, 5, 5, 5, 5, 0, -10, 0,
			0, 0, 0, 5, 5, 5, 5, 0, -5, 0,
			0, -5, 0, 5, 5, 5, 5, 0, -5, 0,
			0, -10, 0, 5, 5, 5, 5, 0, -10, 0,
			0, -10, 0, 0, 0, 0, 0, 0, -10, 0,
			0, -20, -10, -10, -5, -5, -10, -10, -20, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 5, 10, 10, -20, -20, 10, 10, 5, 0,
			0, 5, -5, -10, 0, 0, -10, -5, 5, 0,
			0, 0, 0, 0, 20, 20, 0, 0, 0, 0,
			0, 5, 5, 10, 25, 25, 10, 5, 5, 0,
			0, 10, 10, 20, 30, 30, 20, 10, 10, 0,
			0, 50, 50, 50, 50, 50, 50, 50, 50, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 5, 5, 0, 0, 0, 0,
			0, -5, 0, 0, 0, 0, 0, 0, -5, 0,
			0, -5, 0, 0, 0, 0, 0, 0, -5, 0,
			0, -5, 0, 0, 0, 0, 0, 0, -5, 0,
			0, -5, 0, 0, 0, 0, 0, 0, -5, 0,
			0, -5, 0, 0, 0, 0, 0, 0, -5, 0,
			0, 5, 10, 10, 10, 10, 10, 10, 5, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, -50, -40, -30, -30, -30, -30, -40, -50, 0,
			0, -40, -20, 0, 5, 5, 0, -20, -40, 0,
			0, -30, 5, 10, 15, 15, 10, 5, -30, 0,
			0, -30, 0, 15, 20, 20, 15, 0, -30, 0,
			0, -30, 5, 15, 20, 20, 15, 5, -30, 0,
			0, -30, 0, 10, 15, 15, 10, 0, -30, 0,
			0, -40, -20, 0, 0, 0, 0, -20, -40, 0,
			0, -50, -40, -30, -30, -30, -30, -40, -50, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, -20, -10, -10, -10, -10, -10, -10, -20, 0,
			0, -10, 5, 0, 0, 0, 0, 5, -10, 0,
			0, -10, 10, 10, 10, 10, 10, 10, -10, 0,
			0, -10, 0, 10, 10, 10, 10, 0, -10, 0,
			0, -10, 5, 5, 10, 10, 5, 5, -10, 0,
			0, -10, 0, 5, 10, 10, 5, 0, -10, 0,
			0, -10, 0, 0, 0, 0, 0, 0, -10, 0,
			0, -20, -10, -10, -10, -10, -10, -10, -20, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 20, 30, 10, 0, 0, 10, 30, 20, 0,
			0, 20, 20, 0, 0, 0, 0, 20, 20, 0,
			0, -10, -20, -20, -20, -20, -20, -20, -10, 0,
			0, -20, -30, -30, -40, -40, -30, -30, -20, 0,
			0, -30, -40, -40, -50, -50, -40, -40, -30, 0,
			0, -30, -40, -40, -50, -50, -40, -40, -30, 0,
			0, -30, -40, -40, -50, -50, -40, -40, -30, 0,
			0, -30, -40, -40, -50, -50, -40, -40, -30, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		}
	},
	{
		{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		},
		{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, -20, -10, -10, -5, -5, -10, -10, -20, 0,
			0, -10, 0, 0, 0, 0, 0, 0, -10, 0,
			0, -10, 0, 5, 5, 5, 5, 0, -10, 0,
			0, -5, 0, 5, 5, 5, 5, 0, -5, 0,
			0, 0, 0, 5, 5, 5, 5, 0, -5, 0,
			0, -10, 5, 5, 5, 5, 5, 0, -10, 0,
			0, -10, 0, 5, 0, 0, 0, 0, -10, 0,
			0, -20, -10, -10, -5, -5, -10, -10, -20, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		},
		{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 50, 50, 50, 50, 50, 50, 50, 50, 0,
			0, 10, 10, 20, 30, 30, 20, 10, 10, 0,
			0, 5, 5, 10, 25, 25, 10, 5, 5, 0,
			0, 0, 0, 0, 20, 20, 0, 0, 0, 0,
			0, 5, -5, -10, 0, 0, -10, -5, 5, 0,
			0, 5, 10, 10, -20, -20, 10, 10, 5, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		},
		{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 5, 10, 10, 10, 10, 10, 10, 5, 0,
			0, -5, 0, 0, 0, 0, 0, 0, -5, 0,
			0, -5, 0, 0, 0, 0, 0, 0, -5, 0,
			0, -5, 0, 0, 0, 0, 0, 0, -5, 0,
			0, -5, 0, 0, 0, 0, 0, 0, -5, 0,
			0, -5, 0, 0, 0, 0, 0, 0, -5, 0,
			0, 0, 0, 0, 5, 5, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		},
		{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, -50, -40, -30, -30, -30, -30, -40, -50, 0,
			0, -40, -20, 0, 0, 0, 0, -20, -40, 0,
			0, -30, 0, 10, 15, 15, 10, 0, -30, 0,
			0, -30, 5, 15, 20, 20, 15, 5, -30, 0,
			0, -30, 0, 15, 20, 20, 15, 0, -30, 0,
			0, -30, 5, 10, 15, 15, 10, 5, -30, 0,
			0, -40, -20, 0, 5, 5, 0, -20, -40, 0,
			0, -50, -40, -30, -30, -30, -30, -40, -50, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		},
		{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, -20, -10, -10, -10, -10, -10, -10, -20, 0,
			0, -10, 0, 0, 0, 0, 0, 0, -10, 0,
			0, -10, 0, 5, 10, 10, 5, 0, -10, 0,
			0, -10, 5, 5, 10, 10, 5, 5, -10, 0,
			0, -10, 0, 10, 10, 10, 10, 0, -10, 0,
			0, -10, 10, 10, 10, 10, 10, 10, -10, 0,
			0, -10, 5, 0, 0, 0, 0, 5, -10, 0,
			0, -20, -10, -10, -10, -10, -10, -10, -20, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		},
		{	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, -30, -40, -40, -50, -50, -40, -40, -30, 0,
			0, -30, -40, -40, -50, -50, -40, -40, -30, 0,
			0, -30, -40, -40, -50, -50, -40, -40, -30, 0,
			0, -30, -40, -40, -50, -50, -40, -40, -30, 0,
			0, -20, -30, -30, -40, -40, -30, -30, -20, 0,
			0, -10, -20, -20, -20, -20, -20, -20, -10, 0,
			0, 20, 20, 0, 0, 0, 0, 20, 20, 0,
			0, 20, 30, 10, 0, 0, 10, 30, 20, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		}
	}
};




//game states
constexpr unsigned char GAMESTATE_RUNNING = 0x1;								//1st bit
constexpr unsigned char GAMESTATE_MATE_BLACK = 0x2;								//2nd bit
constexpr unsigned char GAMESTATE_MATE_WHITE = 0x4;								//3rd bit
constexpr unsigned char GAMESTATE_DRAW = 0x8;									//4th bit
constexpr unsigned char GAMESTATE_STALEMATE = GAMESTATE_DRAW + 0x10;			//5th bit
constexpr unsigned char GAMESTATE_THREEFOLDREPETITION = GAMESTATE_DRAW + 0x20;	//6th bit
constexpr unsigned char GAMESTATE_FIFTYMOVERULE = GAMESTATE_DRAW + 0x40;		//7th bit
constexpr unsigned char GAMESTATE_INSUFFICIENTMATERIAL = GAMESTATE_DRAW + 0x80;	//8th bit

//helper functions
constexpr bool IsRunning(unsigned char gameState)
{
	return gameState & 0b00000001;
}

constexpr int GetGameStateValue(unsigned char gameState)
{
	if (gameState & 0b00000010) { return -0x100000; }
	if (gameState & 0b00000100) { return 0x100000; }
	return 0;
}

constexpr int BREAK_SEARCH = 0x10001;

//search info flags
constexpr uint8_t EXACT = 0x1;
constexpr uint8_t LOWERBOUND = 0x2;
constexpr uint8_t UPPERBOUND = 0x3;

//message IDs
constexpr unsigned int ID_RENDERBOARD = 1;
constexpr unsigned int ID_FEN = 2;
constexpr unsigned int ID_MOVE = 3;
constexpr unsigned int ID_MAKEBESTMOVE = 4;
constexpr unsigned int ID_NEWNODE = 5;
constexpr unsigned int ID_NODESCORE = 6;
constexpr unsigned int ID_SETTINGS = 7;

void PrintPosition(unsigned char* pos);