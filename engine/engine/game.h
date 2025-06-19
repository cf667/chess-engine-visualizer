#pragma once

class Move
{
public:
	char origin = 0;
	char destination = 0;
	char flags = 0;
	char capture = 0;
};

class Game
{
public:
	Game();
	Move* GetLegalMoves();

	bool toMove; // 1 for white / 0 for black
	unsigned char position[120]; //10x12

	Move legalMoves[218];
};

unsigned char* printPosition(unsigned char* pos);