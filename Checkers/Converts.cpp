#include "Converts.hpp"
#include "Movement.hpp"
#include <malloc.h>

bitboard* GlTmpBbMap = (bitboard*)malloc(sizeof(bitboard) * 4);

void bbTolistbb(bitboard* elemListbbMap, bitboard* bbMap, chcolor color, bool changing_position)
{
	for (int i = 3; i >= 0; i--)
	{
		elemListbbMap[i] = bbMap[i];
	}
	elemListbbMap[4] = changing_position << 1 | ((color == CHWHITE)?0:1);
}

void bbFromlistbb(bitboard* elemListbbMap, bitboard** bbMap, chcolor* color, bool* changing_position)
{
	*bbMap = GlTmpBbMap;
	for (int i = 3; i >= 0; i--)
	{
		(*bbMap)[i] = elemListbbMap[i];
	}
	*color = ((elemListbbMap[4] & 1) == 0) ? CHWHITE : CHBLACK;
	*changing_position = ((elemListbbMap[4] >> 1) & 1);
}