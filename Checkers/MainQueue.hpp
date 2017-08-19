#ifndef MAINQUEUE_HPP
#define MAINQUEUE_HPP

#include "Queue.hpp"
#include "Movement.hpp"

#define DEEPEST 20

class MainQueue
{
public:
	MainQueue* __restrict parent;
	MainQueue* __restrict next_record;
	Queue* now;
	int to_draw;
	int deep;
	MainQueue* next();
	MainQueue();

private:
	bool add_element(Queue);
	MainQueue* select_child_first();
	void set_result();
	void set_result(chresult);
	void add_result(chresult);
	MainQueue* select_child_continue();
	void find_to_draw(int count_parent, bitboard* bbMap, bool changing_position);
	void add_result_deep();
};

#endif /* MAINQUEUE_HPP */