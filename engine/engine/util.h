#pragma once
#include <string>

#include "game.h"

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

//values
constexpr unsigned char PIECE_VALUES[] = { 0, 9, 1, 5, 3, 3 };

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
	if (gameState & 0b00000010) { return -0x1000; }
	if (gameState & 0b00000100) { return 0x1000; }
	return 0;
}

void PrintPosition(unsigned char* pos);