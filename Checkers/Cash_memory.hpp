#ifndef Cash_memory_hpp
#define Cash_memory_hpp

#include "Queue.hpp"

void save_to_cash_memory(Queue*, bitboard*);
bool in_cash(bitboard*, int, chresult*);
void clean_memory();

class mem_cel
{
public:
	mem_cel* pnhc; /* pointer to next in hash collection */
	mem_cel* pphc; /* pointer to prev in hash collection */
	mem_cel* pns;  /* pointer to next "mem_cel" record in stack */
	mem_cel* pps;  /* pointer to prev "mem_cel" record in stack */
	chresult result;
	int to_draw;
	bitboard* __restrict data;
	int hash_num;

	mem_cel()
	{
		to_draw = -1;
		data = new bitboard[5];
	}
};

#endif /*Cash_memory_hpp*/
