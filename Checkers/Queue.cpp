#include "Queue.hpp"
#include "Converts.hpp"
#include "BitBoard.hpp"
#include <malloc.h>

Queue::Queue()
{
	bbMap = nullptr;
	pcs=nullptr;
	to_draw = -1;
	result=CHUNKNOWN;
};

bool Queue::operator!=(Queue a)
{
	bool result = false;
	int n = 0;

	while (result == false && n < 4)
	{
		result |= (this->bbMap[n] != a.bbMap[n]);
		n++;
	}
	if (result == false)
	{
		result |= (this->color != a.color);
	}
	return result;
}

void Queue::loading(char coordinate[8][8])
{
	int answer;
	int color;
	bool checker;
	bitboard *bbMap = (bitboard*)malloc(sizeof(bitboard)*4);
	bbMap[0] = bbMap[1] = bbMap[2] = bbMap[3] = 0;

	for (int i=0; i<8; i++)
	for (int j=0; j<8; j++)
	{
		if ( coordinate[i][j] == '#' || coordinate[i][j] == 0)
		{
			continue;
		}
		
		answer = num_in_bitboard(i, j);

		if ( coordinate[i][j] == 2 || coordinate[i][j] == -2 )
		{
			checker = false;
			coordinate[i][j] = coordinate[i][j]/2;
		}
		else
			checker = true;

		color = ( coordinate[i][j] == 1 ) ? 1/*CHBLACK*/ : 0/*CHWHITE*/;

		bbMap[ALL_BLACK & color] |= (1 << answer);
		if (checker) bbMap[CHECKERS_WHITE ^ color] |= (1 << answer);
	}

	this->color = CHBLACK;
	this->changing_position = true;
	this->bbMap = bbMap;
	ptQ = this;
}

void Queue::create(bitboard *bbMap, chcolor color, bool changing_position)
{
	if (this->pcs == nullptr)
	{
		this->pcs = new list_bitboards();
		bbTolistbb(this->pcs->bbMap, bbMap, color, changing_position);
	}
	else if (this->pcs->bbMap[0] == 0xFFFFFFFF)
	{
		bbTolistbb(this->pcs->bbMap, bbMap, color, changing_position);
	}
	else
	{
		list_bitboards* tmp_pcs = this->pcs;
		while (tmp_pcs->next != nullptr && tmp_pcs->next->bbMap[0] != 0xFFFFFFFF)
		{
			tmp_pcs = tmp_pcs->next;
		}
		if (tmp_pcs->next == nullptr)
		{
			tmp_pcs->next = new list_bitboards();
		}
		bbTolistbb(tmp_pcs->next->bbMap, bbMap, color, changing_position);
	}
}