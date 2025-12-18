#pragma once
#include <vector>

#include <chrono>

class Move
{
public:
	void Init(char origin, char destination, char flags, char capture);

	char origin = -1;
	char destination = -1;
	char flags = -1;
	char capture = -1;
};

class MoveList
{
public:
	int count = 0;
	Move list[256];
};

class GameRules
{
public:
	GameRules();

	char enPassantTarget;
	char castlingAbility;
	char halfMoveCounter; //for 50 move rule
};

class Game
{
public:
	Game();
	Game(const char* fen); //import position in FEN-notation

	//game rules
	unsigned char position[120]; //10x12
	bool toMove; // 1 for white / 0 for black
	GameRules gameRules;

	uint64_t hashKey; //to detect position repetitions

	bool MakeMove(Move move);
	bool IsCheck(bool white);
	MoveList GetAllMoves();
	MoveList GetLegalMoves();

	Move bestMove;
};

size_t Perft(Game& game, int depth, bool first = 1);

extern std::chrono::duration<float> totalDuration;