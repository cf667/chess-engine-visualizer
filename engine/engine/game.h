#pragma once

class Move
{
public:
	void Init(char origin, char destination, char flags, char capture);

	char origin;
	char destination;
	char flags;
	char capture;
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