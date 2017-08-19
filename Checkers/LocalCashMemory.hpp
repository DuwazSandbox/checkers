#ifndef LOCALCASHMEMORY
#define LOCALCASHMEMORY

bool find_in_hash(bitboard* data, int hash_num, int draw, chresult* result);
void add_in_cash_memory(Queue *queue, bitboard* condition, int hash_num);
void add_first_cel(mem_cel *add_cel);
void push_stack(mem_cel* push_cel);
void push_hash(mem_cel* add_cel, int hash_num);
void add_data(mem_cel* push_cel, Queue* queue, bitboard* condition);
void pop_stack_and_hash();

#endif //LOCALCASHMEMORY