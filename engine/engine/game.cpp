#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "util.h"

#include <iostream>

#pragma warning(push, 4)

//position at the start of every game
constexpr unsigned char startingPos[120] =
{	OUTOFBOUND,	OUTOFBOUND,	OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND,
	OUTOFBOUND,	OUTOFBOUND,	OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND,
	OUTOFBOUND, BROOK,		BKNIGHT,	BBISHOP,	BQUEEN,		BKING,		BBISHOP,	BKNIGHT,	BROOK,		OUTOFBOUND,
	OUTOFBOUND,	BPAWN,		BPAWN,		BPAWN,		BPAWN,		BPAWN,		BPAWN,		BPAWN,		BPAWN,		OUTOFBOUND,
	OUTOFBOUND,	EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		OUTOFBOUND,
	OUTOFBOUND,	EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		OUTOFBOUND,
	OUTOFBOUND,	EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		OUTOFBOUND,
	OUTOFBOUND,	EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		EMPTY,		OUTOFBOUND,
	OUTOFBOUND,	WPAWN,		WPAWN,		WPAWN,		WPAWN,		WPAWN,		WPAWN,		WPAWN,		WPAWN,		OUTOFBOUND,
	OUTOFBOUND, WROOK,		WKNIGHT,	WBISHOP,	WQUEEN,		WKING,		WBISHOP,	WKNIGHT,	WROOK,		OUTOFBOUND,
	OUTOFBOUND,	OUTOFBOUND,	OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND,
	OUTOFBOUND,	OUTOFBOUND,	OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND, OUTOFBOUND };

//offset for each move direction - pawns are handled separately
//most right bit in each piece code = slide
const std::vector<char> kingOffset =	{ -11, -10, -9, -1, 1, 9, 10, 11 };
const std::vector<char> queenOffset =	{ -11, -10, -9, -1, 1, 9, 10, 11 };
const std::vector<char> pawnOffset =	{ 0 };
const std::vector<char> rookOffset =	{ -10, -1, 1, 10 };
const std::vector<char> knightOffset =	{ -21, -19, -12, -8, 8, 12, 19, 21 };
const std::vector<char> bishopOffset =	{ -11, -9, 9, 11 };

const std::vector<std::vector<char>> offsets = { pawnOffset, queenOffset, pawnOffset, rookOffset, knightOffset, bishopOffset, kingOffset }; //pawnOffset -> NULL

void Move::Init(char from, char to, char moveFlags, char capturedPiece)
{
	Move::origin = from;
	Move::destination = to;
	Move::flags = moveFlags;
	Move::capture = capturedPiece;
}

GameRules::GameRules()
{
	GameRules::enPassantTarget = 0;
	GameRules::castlingAbility = 0xF; //0b1111 - every castle allowed
	GameRules::halfMoveCounter = 0;
}

Game::Game()
{
	std::copy(std::begin(startingPos), std::end(startingPos), std::begin(Game::position));
	Game::toMove = 1;
}

Game::Game(const char* fen)
{
	std::copy(std::begin(startingPos), std::end(startingPos), std::begin(Game::position));

	int curPos = 0;
	for (int i = 2; i < 10; i++) //ranks
	{
		for (int j = 1; j < 9; j++) //files
		{
			switch (fen[curPos])
			{
				//pieces
			case 'Q':
				position[i * 10 + j] = WQUEEN;
				break;
			case 'P':
				position[i * 10 + j] = WPAWN;
				break;
			case 'R':
				position[i * 10 + j] = WROOK;
				break;
			case 'N':
				position[i * 10 + j] = WKNIGHT;
				break;
			case 'B':
				position[i * 10 + j] = WBISHOP;
				break;
			case 'K':
				position[i * 10 + j] = WKING;
				break;
			case 'q':
				position[i * 10 + j] = BQUEEN;
				break;
			case 'p':
				position[i * 10 + j] = BPAWN;
				break;
			case 'r':
				position[i * 10 + j] = BROOK;
				break;
			case 'n':
				position[i * 10 + j] = BKNIGHT;
				break;
			case 'b':
				position[i * 10 + j] = BBISHOP;
				break;
			case 'k':
				position[i * 10 + j] = BKING;
				break;
				//next rank - isnt reached, because it loops back after 8 squares (right before the '/')
			case '/':
				break;
			default:
				for (int k = 0; k < fen[curPos] - '0'; k++) //empty squares
				{
					position[i * 10 + j] = EMPTY;
					j++;
				}
				j--;
				break;
			}
			curPos++;
		}
		curPos++;
	}

	//loop skips the space - curPos is now automatically the side to move
	switch (fen[curPos])
	{
	case 'w':
		Game::toMove = 1;
		break;
	case 'b':
		Game::toMove = 0;
		break;
	}
	curPos = curPos + 2;

	//castling ability
	Game::gameRules.castlingAbility = 0x0;
	if (fen[curPos] == '-') //no castle allowed
	{
		curPos = curPos + 2;
		goto skipCastles;
	}

	if (fen[curPos] == 'K')
	{
		Game::gameRules.castlingAbility = Game::gameRules.castlingAbility + WKCASTLE;
		curPos++;
	}
	if (fen[curPos] == 'Q')
	{
		Game::gameRules.castlingAbility = Game::gameRules.castlingAbility + WQCASTLE;
		curPos++;
	}
	if (fen[curPos] == 'k')
	{
		Game::gameRules.castlingAbility = Game::gameRules.castlingAbility + BKCASTLE;
		curPos++;
	}
	if (fen[curPos] == 'q')
	{
		Game::gameRules.castlingAbility = Game::gameRules.castlingAbility + BQCASTLE;
		curPos++;
	}
	curPos++;
skipCastles:

	//en passant targe square
	if (fen[curPos] == '-') //no target
	{
		Game::gameRules.enPassantTarget = 0;
		curPos = curPos + 2;
		goto skipEnPassant;
	}

	Game::gameRules.enPassantTarget = CoordToIndex(fen + curPos); //convert normal coordinates to index in 10x12 position array
	curPos = curPos + 3;
skipEnPassant:

	Game::gameRules.halfMoveCounter = fen[curPos] - '0';
}

bool Game::MakeMove(Move move)
{
	ruleHist.push_back(Game::gameRules);

	position[move.destination] = position[move.origin];
	position[move.origin] = EMPTY;

	if (move.flags == ENPASSANT)
	{
		if(toMove) { position[move.destination + 10] = EMPTY; }
		else { position[move.destination - 10] = EMPTY; }
	}

	if (move.flags == DOUBLEPAWNPUSH)
	{
		if (toMove) { Game::gameRules.enPassantTarget = move.destination + 10; }
		else { Game::gameRules.enPassantTarget = move.destination - 10; }
	}
	else
	{
		Game::gameRules.enPassantTarget = 0;
	}

	switch (move.flags)
	{
	case PROMOTION_KNIGHT: case PROMOTION_KNIGHT_CAPTURE:
		Game::position[move.destination] = (Game::position[move.destination] & ~0b00000111) | (KNIGHT & 0b00000111);
		break;
	case PROMOTION_BISHOP: case PROMOTION_BISHOP_CAPTURE:
		Game::position[move.destination] = (Game::position[move.destination] & ~0b00000111) | (BISHOP & 0b00000111);
		break;
	case PROMOTION_ROOK: case PROMOTION_ROOK_CAPTURE:
		Game::position[move.destination] = (Game::position[move.destination] & ~0b00000111) | (ROOK & 0b00000111);
		break;
	case PROMOTION_QUEEN: case PROMOTION_QUEEN_CAPTURE:
		Game::position[move.destination] = (Game::position[move.destination] & ~0b00000111) | (QUEEN & 0b00000111);
		break;
	}
	
	//castle
	if (move.flags == CASTLE_KING)
	{
		position[move.destination - 1] = position[move.destination + 1];
		position[move.destination + 1] = EMPTY;
		if (Game::toMove) //remove castle ability
		{
			Game::gameRules.castlingAbility = Game::gameRules.castlingAbility - 0xC;
		}
		else
		{
			Game::gameRules.castlingAbility = Game::gameRules.castlingAbility - 0x3;
		}
	}
	else if (move.flags == CASTLE_QUEEN)
	{
		position[move.destination + 1] = position[move.destination - 2];
		position[move.destination - 2] = EMPTY;
		if (Game::toMove) //remove castle ability
		{
			Game::gameRules.castlingAbility = Game::gameRules.castlingAbility - 0xC;
		}
		else
		{
			Game::gameRules.castlingAbility = Game::gameRules.castlingAbility - 0x3;
		}
	}

	switch (move.origin) //rook or king moves
	{
	case 28:
		Game::gameRules.castlingAbility &= ~(1 << 0); //BK
		break;
	case 21:
		Game::gameRules.castlingAbility &= ~(1 << 1); //BQ
		break;
	case 98:
		Game::gameRules.castlingAbility &= ~(1 << 2); //WK
		break;
	case 91:
		Game::gameRules.castlingAbility &= ~(1 << 3); //WQ
		break;
	case 25:
		Game::gameRules.castlingAbility &= ~(1 << 0); //B
		Game::gameRules.castlingAbility &= ~(1 << 1);
		break;
	case 95:
		Game::gameRules.castlingAbility &= ~(1 << 2); //W
		Game::gameRules.castlingAbility &= ~(1 << 3);
		break;
	}

	switch (move.destination) //rook is captured
	{
	case 28:
		Game::gameRules.castlingAbility &= ~(1 << 0); //BK
		break;
	case 21:
		Game::gameRules.castlingAbility &= ~(1 << 1); //BQ
		break;
	case 98:
		Game::gameRules.castlingAbility &= ~(1 << 2); //WK
		break;
	case 91:
		Game::gameRules.castlingAbility &= ~(1 << 3); //WQ
		break;
	}

	moveHist.push_back(move);
	Game::toMove = !Game::toMove;

	return 1;
}

bool Game::RevertMove()
{
	position[moveHist.back().origin] = position[moveHist.back().destination];
	position[moveHist.back().destination] = moveHist.back().capture;

	if (moveHist.back().flags == ENPASSANT)
	{
		if (toMove)
		{ 
			position[moveHist.back().destination - 10] = moveHist.back().capture; 
			position[moveHist.back().destination] = EMPTY;
		}
		else 
		{ 
			position[moveHist.back().destination + 10] = moveHist.back().capture; 
			position[moveHist.back().destination] = EMPTY;
		}
	}

	//promotion
	if ((moveHist.back().flags >> 3) & 1)
	{
		Game::position[moveHist.back().origin] = (Game::position[moveHist.back().origin] & ~0b00000111) | (PAWN & 0b00000111);
	}

	//castle
	if (moveHist.back().flags == CASTLE_KING)
	{
		position[moveHist.back().destination + 1] = position[moveHist.back().destination - 1];
		position[moveHist.back().destination - 1] = EMPTY;
	}
	else if (moveHist.back().flags == CASTLE_QUEEN)
	{
		position[moveHist.back().destination - 2] = position[moveHist.back().destination + 1];
		position[moveHist.back().destination + 1] = EMPTY;
	}

	moveHist.pop_back();

	Game::gameRules = Game::ruleHist.back();
	Game::ruleHist.pop_back();

	Game::toMove = !Game::toMove;

	return 1;
}

bool Game::IsCheck()
{
	std::vector moves = Game::GetAllMoves(0);
	for (int i = 0; i < moves.size(); i++)
	{
		if ((moves[i].capture & 0b111) == KING)
		{
			return 1;
		}
	}
	return 0;
}

std::chrono::duration<float> totalDuration = std::chrono::duration<float>(0.f);

std::vector<Move> Game::GetAllMoves(bool includeCastling)
{
	auto timeStart = std::chrono::high_resolution_clock::now();

	std::vector<Move> moveList;

	int curMoveIndex = 0;
	Move curMove;

	char pawnDirection;
	int doublePushRank;
	int promotionRank;

	bool inCheck;

	for (char i = 0; i < 120; i++) //iterate through board
	{
		if (!(i % 10))
		{
			//std::cout << std::endl;
		}
		//std::cout << i << " ";

		if ((Game::position[i] >> 5) & 1 || (Game::position[i] >> 4) & 1 || ((Game::position[i] >> 3) & 1) != Game::toMove) //if square is out of bound, empty or piece is the wrong color
		{
			continue;
		}

		if ((Game::position[i] & 0b111) == PAWN) //pawn
		{
			if ((Game::position[i] >> 3) & 1) //check for color
			{
				pawnDirection = -10;
				doublePushRank = 80;
				promotionRank = 30;
			}
			else
			{
				pawnDirection = 10;
				doublePushRank = 30;
				promotionRank = 80;
			}
			
			if ((Game::position[i + pawnDirection] >> 4) & 1)
			{
				if (i > promotionRank && i < promotionRank + 9) //promotion
				{
					for (char promotion = PROMOTION_KNIGHT; promotion <= PROMOTION_QUEEN; promotion++)
					{
						curMove.Init(i, i + pawnDirection, promotion, EMPTY);
						moveList.push_back(curMove);
						curMoveIndex++;
					}
				}
				else  //normal move
				{
					curMove.Init(i, i + pawnDirection, QUIETMOVE, EMPTY);
					moveList.push_back(curMove);
					curMoveIndex++;
				}

				if (i > doublePushRank && i < doublePushRank + 9 && (Game::position[i + pawnDirection * 2] >> 4) & 1) //double pawn push
				{
					curMove.Init(i, i + pawnDirection * 2, DOUBLEPAWNPUSH, EMPTY);
					moveList.push_back(curMove);
					curMoveIndex++;
				}
			}

			if (!((Game::position[i + pawnDirection + 1] >> 5) & 1) && !((Game::position[i + pawnDirection + 1] >> 4) & 1) && ((Game::position[i + pawnDirection + 1] >> 3) & 1) != Game::toMove) //right side capture
			{
				if (i > promotionRank && i < promotionRank + 9) //promotion
				{
					for (char promotion = PROMOTION_KNIGHT; promotion <= PROMOTION_QUEEN; promotion++)
					{
						curMove.Init(i, i + pawnDirection + 1, promotion + CAPTURE, Game::position[i + pawnDirection + 1]);
						moveList.push_back(curMove);
						curMoveIndex++;
					}
				}
				else
				{
					curMove.Init(i, i + pawnDirection + 1, CAPTURE, Game::position[i + pawnDirection + 1]);
					moveList.push_back(curMove);
					curMoveIndex++;
				}
			}
			if (!((Game::position[i + pawnDirection - 1] >> 5) & 1) && !((Game::position[i + pawnDirection - 1] >> 4) & 1) && ((Game::position[i + pawnDirection - 1] >> 3) & 1) != Game::toMove) //left side capture
			{
				if (i > promotionRank && i < promotionRank + 9) //promotion
				{
					for (char promotion = PROMOTION_KNIGHT; promotion <= PROMOTION_QUEEN; promotion++)
					{
						curMove.Init(i, i + pawnDirection - (char)1, promotion + CAPTURE, Game::position[i + pawnDirection - 1]);
						moveList.push_back(curMove);
						curMoveIndex++;
					}
				}
				else
				{
					curMove.Init(i, i + pawnDirection - (char)1, CAPTURE, Game::position[i + pawnDirection - 1]);
					moveList.push_back(curMove);
					curMoveIndex++;
				}
				
			}

			//en passant
			if (i + pawnDirection + 1 == Game::gameRules.enPassantTarget) //right side en passant
			{
				curMove.Init(i, i + pawnDirection + (char)1, ENPASSANT, Game::position[i + 1]);
				moveList.push_back(curMove);
				curMoveIndex++;
			}
			else if (i + pawnDirection - 1 == Game::gameRules.enPassantTarget) //left side en passant
			{
				curMove.Init(i, i + pawnDirection - (char)1, ENPASSANT, Game::position[i - 1]);
				moveList.push_back(curMove);
				curMoveIndex++;
			}
		}
		else //every other piece
		{
			for (char offset : offsets[Game::position[i] & 0b111]) //iterate through moveset
			{
				if (Game::position[i] & 1) //check if piece can slide (queen, rook, bishop)
				{
					for (char slide = 1; !((Game::position[i + offset * slide] >> 5) & 1); slide++) //as long as its not off the board
					{
						if ((Game::position[i + offset * slide] >> 4) & 1) //empty square
						{
							curMove.Init(i, i + offset * slide, QUIETMOVE, EMPTY);
							moveList.push_back(curMove);
							curMoveIndex++;
						}
						else if (((Game::position[i + offset * slide] >> 3) & 1) != Game::toMove) //enemy piece
						{
							curMove.Init(i, i + offset * slide, CAPTURE, Game::position[i + offset * slide]);
							moveList.push_back(curMove);
							curMoveIndex++;
							break;
						}
						else //friendly piece
						{
							break;
						}
					}
				}
				else //pieces that cant slide (king, knight)
				{
					if ((Game::position[i + offset] >> 5) & 1) //out of bound
					{
						continue;
					}

					if ((Game::position[i + offset] >> 4) & 1) //empty square
					{
						curMove.Init(i, i + offset, QUIETMOVE, EMPTY);
						moveList.push_back(curMove);
						curMoveIndex++;

						//castling
						if ((Game::position[i] & 0b111) == KING && (offset == 1 || offset == -1) && includeCastling) //if king moves right/left, ...
						{
							Game::toMove = !Game::toMove;
							if (Game::IsCheck()) //... isnt in check right now ...
							{
								Game::toMove = !Game::toMove;
								goto skipCastle;
							}
							Game::toMove = !Game::toMove;

							Game::MakeMove(curMove);
							inCheck = Game::IsCheck();
							Game::RevertMove();
							if (!inCheck && (Game::position[i + offset * 2] >> 4) & 1) //... and isnt in check if he moves right/left
							{
								char flag = CASTLE_KING;
								if (offset == -1)
								{
									flag = CASTLE_QUEEN;
								}
								char temp = 0; //need for castle ability check
								if (flag == CASTLE_QUEEN)
								{
									temp = 1;
								}

								if (((Game::gameRules.castlingAbility >> (Game::toMove * 2 + temp)) & 1) && //check castle ability
									((Game::position[i + offset * 2 - temp] >> 4) & 1))							//check if squares are empty
								{
									curMove.Init(i, i + offset * 2, flag, EMPTY);
									moveList.push_back(curMove);
									curMoveIndex++;
								}
							}
						skipCastle:
							;
						}
					}
					else if (((Game::position[i + offset] >> 3) & 1) != Game::toMove) //enemy piece
					{
						curMove.Init(i, i + offset, CAPTURE, Game::position[i + offset]);
						moveList.push_back(curMove);
						curMoveIndex++;
					}
				}
			}
		}
	}

	auto timeEnd = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = timeEnd - timeStart;
	totalDuration += duration;
	return moveList;
}

std::vector<Move> Game::GetLegalMoves()
{
	std::vector<Move> legalMoves = Game::GetAllMoves(1);
	for (int i = 0; i < legalMoves.size(); i++)
	{
		Game::MakeMove(legalMoves[i]);
		if (IsCheck())
		{
			legalMoves.erase(legalMoves.begin() + i);
			i--;
		}
		Game::RevertMove();
	}
	return legalMoves;
}

size_t Perft(Game game, int depth, bool first)
{
	size_t result = 0;
	size_t currentResult = 0;

	std::vector<Move> moveList = game.GetLegalMoves();
	if (depth == 1) 
	{ 
		if (first)
		{
			for (Move move : moveList)
			{
				std::cout << IndexToCoord(move.origin) << IndexToCoord(move.destination) << std::endl;
			}
		}
		return moveList.size(); 
	}

	for (Move move : moveList)
	{
		game.MakeMove(move);
		currentResult = Perft(game, depth - 1, 0);
		if (first)
		{
			std::cout << IndexToCoord(move.origin) << IndexToCoord(move.destination) << ": " << currentResult << std::endl;
		}
		result += currentResult;
		game.RevertMove();
	}

	if (first)
	{
		std::cout << "PERFT SUM: " << result << std::endl;
	}

	return result;
}

#pragma warning(pop)