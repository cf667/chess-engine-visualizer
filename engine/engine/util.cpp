#define _CRT_SECURE_NO_WARNINGS
#include "util.h"

#include <iostream>

#pragma warning(push, 4)

void PrintPosition(unsigned char* pos)
{
	unsigned char result[64];
	for (int i = 2; i < 10; i++)
	{
		std::copy(pos + i * 10 + 1, pos + i * 10 + 9, result + (i - 2) * 8);
	}

	for (int i = 0; i < 64; i++)
	{
		if (!(i % 8)) { std::cout << "\n"; };
		switch (result[i])
		{
		case WKING:
			std::cout << 'K';
			break;
		case WQUEEN:
			std::cout << 'Q';
			break;
		case WPAWN:
			std::cout << 'P';
			break;
		case WROOK:
			std::cout << 'R';
			break;
		case WKNIGHT:
			std::cout << 'N';
			break;
		case WBISHOP:
			std::cout << 'B';
			break;
		case BKING:
			std::cout << 'k';
			break;
		case BQUEEN:
			std::cout << 'q';
			break;
		case BPAWN:
			std::cout << 'p';
			break;
		case BROOK:
			std::cout << 'r';
			break;
		case BKNIGHT:
			std::cout << 'n';
			break;
		case BBISHOP:
			std::cout << 'b';
			break;
		case EMPTY:
			std::cout << '.';
			break;
		}
	}
	std::cout << std::endl;
	return;
}

std::string IndexToCoord(char i)
{
	std::string result = "  ";
	result[0] = 'a' + ((i % 10) - 1);
	result[1] = '0' + 8 - ((i / 10) - 2);
	return result;
}

char CoordToIndex(const char* c)
{
	return (120 - ((c[1] - '0' + 2) * 10)) + (c[0] - 'a' + 1);
}

#pragma warning(pop)