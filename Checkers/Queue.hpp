#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "BitBoard.hpp"

typedef char* stringMap;

/* color checker */
enum chcolor
{
	CHBLACK,
	CHNONE,			/* Doesn't have a checker */
	CHWHITE
};

class list_bitboards
{
public:
	bitboard* __restrict bbMap;
	list_bitboards* __restrict next;

	list_bitboards()
	{
		bbMap = new bitboard[5];
		next = nullptr;
	}
};

class Queue
{
public:
	bitboard* __restrict bbMap;
	list_bitboards* __restrict pcs;		/*list of childs*/
	chcolor color;
	enum chresult result;
	int to_draw;
	bool changing_position;

	Queue();
	void  loading(char[8][8]);
	void create(bitboard*, chcolor, bool);
	bool operator!=(Queue);
};

enum chresult
{
	CHWIN,
	CHDRAW,
	CHLOSE,
	CHUNKNOWN
};

static Queue* ptQ;			/*pointer of top Queue*/

#endif /*QUEUE_HPP*/