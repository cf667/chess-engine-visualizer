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
	std::vector<Move> GetLegalMoves();

	bool toMove; // 1 for white / 0 for black
	unsigned char position[120]; //10x12
};

unsigned char* printPosition(unsigned char* pos);