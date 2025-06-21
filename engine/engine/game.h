#pragma once
#include <vector>

class Move
{
public:
	void Init(char origin, char destination, char flags, char capture);

	char origin = -1;
	char destination = -1;
	char flags = -1;
	char capture = -1;
};

class Game
{
public:
	Game();
	Game(const char* fen); //import position in FEN-notation
	bool MakeMove(Move move);
	bool RevertMove();
	bool IsCheck();
	std::vector<Move> GetAllMoves();
	std::vector<Move> GetLegalMoves();

	//game rules
	unsigned char position[120]; //10x12
	bool toMove; // 1 for white / 0 for black
	char enPassantTarget;
	char castlingAbility;
	char halfMoveCounter; //for 50 move rule

	std::vector<Move> moveHist;
};

unsigned char* printPosition(unsigned char* pos);