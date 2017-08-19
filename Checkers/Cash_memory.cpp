#include "Cash_memory.hpp"
#include "Converts.hpp"
#include "LocalCashMemory.hpp"
#include <string.h>

#define HASH_NUM (0x7FFFFFF + 1)	/* only 2^value */
#define MAX_COUNT_CEL 25000000

int count_similar = 0;
mem_cel* head_stack = nullptr;
mem_cel* memory[HASH_NUM];
int count_cel = 0;

void clean_memory()
{
	for (int i=0; i < HASH_NUM; i++)
	{
		memory[i] = nullptr;
	}
}

bitboard pack_code(bitboard number) {
	bitboard tmp;
	tmp = (number >> 4) & ~0x1FF;
	tmp |= number & 0x1F;
	tmp |= (number >> 2) & 0x1E0; 

	return tmp;
}

void save_to_cash_memory(Queue* queue, bitboard* bbMap)
{
	int hash_num = pack_code(bbMap[0] ^ bbMap[1]) & (HASH_NUM - 1);
	add_in_cash_memory(queue, bbMap, hash_num);
}

bool in_cash(bitboard *bbMap, int draw, chresult* result)
{
	int hash_num = pack_code(bbMap[0] ^ bbMap[1]) & (HASH_NUM - 1);
	return find_in_hash(bbMap, hash_num, draw, result);
}

bool find_in_hash(bitboard* bbMap, int hash_num, int draw, chresult* result)
{
	//__assume(memory[hash_num] == nullptr);
	if (memory[hash_num] == nullptr) return false;

	int i;
	mem_cel *cur_mem_cel, *start_mem_cel;
	start_mem_cel = cur_mem_cel = memory[hash_num];
	
	do
	{
		if (!memcmp(cur_mem_cel->data, bbMap, 5 * sizeof(bitboard)))
		{
			count_similar++;
			if (cur_mem_cel->result == CHDRAW && draw <= cur_mem_cel->to_draw)
			{
				*result = CHDRAW;
				return true;
			}
			if (cur_mem_cel->result != CHDRAW && draw >= cur_mem_cel->to_draw)
			{
				*result = cur_mem_cel->result;
				return true;
			}
		}
		else count_similar--;
		cur_mem_cel = cur_mem_cel->pphc;
	} while (cur_mem_cel != start_mem_cel);

	return false;
}

void add_in_cash_memory(Queue *queue, bitboard* condition, int hash_num)
{
	mem_cel*  tmp_mem_cel;
	if (head_stack == nullptr)
	{
		tmp_mem_cel = new mem_cel();
		add_first_cel(tmp_mem_cel);
		push_hash(tmp_mem_cel, hash_num);
		add_data(tmp_mem_cel, queue, condition);
		count_cel++;
	}
	else
	{
		if (count_cel < MAX_COUNT_CEL)
		{
			tmp_mem_cel = new mem_cel();
			push_stack(tmp_mem_cel);
			push_hash(tmp_mem_cel, hash_num);
			add_data(tmp_mem_cel, queue, condition);
			count_cel++;
		}
		else
		{
			tmp_mem_cel = head_stack->pns;
			pop_stack_and_hash();
			push_stack(tmp_mem_cel);
			push_hash(tmp_mem_cel, hash_num);
			add_data(tmp_mem_cel, queue, condition);
		}
	}
}

void add_first_cel(mem_cel *add_cel)
{
	add_cel->pns = add_cel;
	add_cel->pps = add_cel;
	head_stack = add_cel;
}

void push_stack(mem_cel* push_cel)
{
	push_cel->pns = head_stack->pns;
	push_cel->pps = head_stack;
	head_stack->pns->pps = push_cel;
	head_stack->pns = push_cel;
	head_stack = push_cel;
}

void push_hash(mem_cel* add_cel, int hash_num)
{
	if (memory[hash_num] == nullptr)
	{
		memory[hash_num] = add_cel;
		add_cel->pnhc = add_cel;
		add_cel->pphc = add_cel;
		add_cel->hash_num = hash_num;
	}
	else
	{
		add_cel->pnhc = memory[hash_num]->pnhc;
		add_cel->pphc = memory[hash_num];
		memory[hash_num]->pnhc->pphc = add_cel;
		memory[hash_num]->pnhc = add_cel;
		memory[hash_num] = add_cel;
		add_cel->hash_num = hash_num;
	}
}

void add_data(mem_cel* push_cel, Queue* queue, bitboard* condition)
{
	for (int i = 0; i < 5; i++)
		push_cel->data[i] = condition[i];
	push_cel->result = queue->result;
	push_cel->to_draw = queue->to_draw;
}

void pop_stack_and_hash()
{
	int hash_num = head_stack->pns->hash_num;

	head_stack->pns->pns->pps = head_stack;
	head_stack->pns = head_stack->pns->pns;

	if (memory[hash_num] == memory[hash_num]->pnhc)
	{
		memory[hash_num] = nullptr;
	}
	else
	{
		memory[hash_num]->pnhc->pnhc->pphc = memory[hash_num];
		memory[hash_num]->pnhc = memory[hash_num]->pnhc->pnhc;
	}
}