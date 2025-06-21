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
	bool MakeMove(Move move);
	bool RevertMove();
	bool IsCheck();
	std::vector<Move> GetAllMoves();
	std::vector<Move> GetLegalMoves();

	bool toMove; // 1 for white / 0 for black
	unsigned char position[120]; //10x12

	std::vector<Move> moveHist;
};

unsigned char* printPosition(unsigned char* pos);