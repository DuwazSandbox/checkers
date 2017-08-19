#include "BitBoard.hpp"

int num_bitboard[] = {
	31,	5,	11,	17,
	24,	30,	4,	10,
	23,	29,	3,	9,
	16,	22,	28,	2,
	15,	21,	27,	1,
	8,	14,	20,	26,
	7,	13,	19,	25,
	0,	6,	12,	18 };

int num_in_bitboard(int i, int j)
{
	return num_bitboard[j / 2 + i * 4];
}