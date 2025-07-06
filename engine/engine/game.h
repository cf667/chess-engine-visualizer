#pragma once
#include <vector>
#include <string>

class Move
{
public:
	void Init(char origin, char destination, char flags, char capture);

	char origin = -1;
	char destination = -1;
	char flags = -1;
	char capture = -1;
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

	bool MakeMove(Move move);
	bool RevertMove();
	bool IsCheck();
	std::vector<Move> GetAllMoves(bool includeCastling);
	std::vector<Move> GetLegalMoves();

	std::vector<Move> moveHist;
	std::vector<GameRules> ruleHist;
};

unsigned char* PrintPosition(unsigned char* pos);

int Perft(Game game, int depth, bool first = 1);

std::string IndexToCoord(char i);

char CoordToIndex(const char* c);