#include <intrin.h>
#include <string.h>

#include "MainQueue.hpp"
#include "Converts.hpp"
#include "Cash_memory.hpp" 

MainQueue::MainQueue()
{
	parent = nullptr;
	next_record = nullptr;
	now = nullptr;
	to_draw = 50;
};

MainQueue* MainQueue::next()
{
	this->now->to_draw = this->to_draw;

	MainQueue* selected = this->select_child_first();

	__assume(selected == nullptr);
	if (selected != nullptr)
	{
		return selected;
	}

	MainQueue* view;
	bitboard bbMap[5];
	selected = this;

	do
	{
		view = selected;
		view->set_result();

		__assume(view->to_draw != 0);
		if (view->to_draw != 0)
		{
			bbTolistbb(bbMap, view->now->bbMap, view->now->color, view->now->changing_position);
			save_to_cash_memory(view->now, bbMap);
		}
		selected = view->parent;
		view = view->parent->select_child_continue();
	} while (view == nullptr && selected->parent != nullptr);

	return view;
}

int deepest_place = 0;	/* additional value */

bool MainQueue::add_element(Queue new_elem)
{
	{ /* check 1 times repeated of this positions*/
		MainQueue* parent = this->parent;
		while (parent != nullptr)
		{
			if (((*parent->now).changing_position == new_elem.changing_position) && !(*parent->now != new_elem))
				return false;
			parent = parent->parent;
		}
	}

	__assume(this->next_record != nullptr);
	if (this->next_record == nullptr)
	{
		MainQueue* new_mainQueue = new MainQueue();
		new_mainQueue->parent = this;
		new_mainQueue->now = new Queue();
		new_mainQueue->deep = this->deep + 1;

		deepest_place++;	/* additional value */

		new_mainQueue->now->changing_position = new_elem.changing_position;
		new_mainQueue->now->color = new_elem.color;
		new_mainQueue->now->to_draw = new_elem.to_draw;

		new_mainQueue->now->bbMap = (bitboard*)malloc(sizeof(bitboard) * 4);
		memcpy(new_mainQueue->now->bbMap, new_elem.bbMap, sizeof(bitboard) * 4);

		this->next_record = new_mainQueue;
	}
	else
	{
		this->next_record->now->changing_position = new_elem.changing_position;
		this->next_record->now->result = CHUNKNOWN;
		this->next_record->now->color = new_elem.color;

		list_bitboards* tmp_new_pcs = this->next_record->now->pcs;
		while (tmp_new_pcs != nullptr)
		{
			tmp_new_pcs->bbMap[0] = 0xFFFFFFFF;
			tmp_new_pcs = tmp_new_pcs->next;
		}

		memcpy(this->next_record->now->bbMap, new_elem.bbMap, sizeof(bitboard) * 4);
	}
	return true;
}

MainQueue* MainQueue::select_child_first()
{
	__assume(this->now->result != CHWIN);
	if (this->now->result == CHWIN)
	{
		return nullptr;
	}

	__assume(!(this->now->pcs == nullptr || this->now->pcs->bbMap[0] == 0xFFFFFFFF));
	if (this->now->pcs == nullptr || this->now->pcs->bbMap[0] == 0xFFFFFFFF)
	{
		return nullptr;
	}

	__assume(this->to_draw != 0);
	if (this->to_draw == 0)
	{
		this->add_result(CHDRAW);
		return nullptr;
	}
	
	__assume(this->deep != DEEPEST);
	if (this->deep == DEEPEST)
	{
		this->add_result_deep();
		return nullptr;
	}

	Queue child;
	int to_draw_old, to_draw_new;
	list_bitboards* tmp_pcs = this->now->pcs;

	for (; tmp_pcs != nullptr && this->now->pcs->bbMap[0] != 0xFFFFFFFF; tmp_pcs = tmp_pcs->next)
	{
		bbFromlistbb(tmp_pcs->bbMap, &child.bbMap, &child.color, &child.changing_position);
		to_draw_old = this->to_draw;
		this->find_to_draw(this->to_draw, child.bbMap, child.changing_position);
		to_draw_new = this->to_draw;
		this->to_draw = to_draw_old;

		chresult rst;
		if (in_cash(tmp_pcs->bbMap, to_draw_new, &rst) == true)
		{
			this->set_result(rst);
			continue;
		}

		if (this->add_element(child) == false)
		{
			this->add_result(CHDRAW);
			continue;
		}
		this->next_record->to_draw = to_draw_new;
		return this->next_record;
	}
	return nullptr;
}

void MainQueue::add_result(chresult result)
{
	chresult parent_result = this->now->result;

	switch (result)
	{
	case CHLOSE: parent_result = CHLOSE; break;
	case CHDRAW: if (parent_result != CHWIN) parent_result = CHDRAW; break;
	case CHWIN: if (parent_result != CHWIN && parent_result != CHDRAW) parent_result = CHWIN; break;
	}

	this->now->result = parent_result;
}

void MainQueue::set_result()
{
	__assume(!(this->now->pcs == nullptr || this->now->pcs->bbMap[0] == 0xFFFFFFFF));
	if (this->now->pcs == nullptr || this->now->pcs->bbMap[0] == 0xFFFFFFFF)
	{
		if (this->now->result == CHUNKNOWN) this->now->result = CHLOSE;
	}

	chresult parent_result = this->parent->now->result;
	Queue child;

	switch (this->now->result)
	{
	case CHWIN: if (parent_result != CHWIN && parent_result != CHDRAW) parent_result = CHLOSE; break;
	case CHDRAW: if (parent_result != CHWIN) parent_result = CHDRAW; break;
	case CHLOSE: parent_result = CHWIN; break;
	}

	this->parent->now->result = parent_result;
}

void MainQueue::set_result(chresult result)
{
	chresult parent_result = this->now->result;
	
	switch (result)
	{
	case CHWIN: if (parent_result != CHWIN && parent_result != CHDRAW) parent_result = CHLOSE; break;
	case CHDRAW: if (parent_result != CHWIN) parent_result = CHDRAW; break;
	case CHLOSE: parent_result = CHWIN; break;
	}

	this->now->result = parent_result;
}

MainQueue* MainQueue::select_child_continue()
{
	__assume(this->now->result != CHWIN);
	if (this->now->result == CHWIN)
	{
		return nullptr;
	}

	__assume(!(this->now->pcs == nullptr || this->now->pcs->bbMap[0] == 0xFFFFFFFF));
	if (this->now->pcs == nullptr || this->now->pcs->bbMap[0] == 0xFFFFFFFF)
	{
		return nullptr;
	}

	__assume(this->to_draw != 0);
	if (this->to_draw == 0)
	{
		this->add_result(CHDRAW);
		return nullptr;
	}
	
	__assume(this->deep != DEEPEST);
	if (this->deep == DEEPEST)
	{
		this->add_result_deep();
		return nullptr;
	}

	Queue child;
	list_bitboards* tmp_pcs = this->now->pcs;

	do
	{
		__assume(!(this->now->pcs == nullptr || this->now->pcs->bbMap[0] == 0xFFFFFFFF));
		if (tmp_pcs == nullptr || tmp_pcs->bbMap[0] == 0xFFFFFFFF) 
			return nullptr;

		bbFromlistbb(tmp_pcs->bbMap, &child.bbMap, &child.color, &child.changing_position);
		tmp_pcs = tmp_pcs->next;
	} while (child != *(this->next_record->now));

	int to_draw_old, to_draw_new;
	for (; tmp_pcs != nullptr && tmp_pcs->bbMap[0] != 0xFFFFFFFF; tmp_pcs = tmp_pcs->next)
	{
		bbFromlistbb(tmp_pcs->bbMap, &child.bbMap, &child.color, &child.changing_position);

		to_draw_old = this->to_draw;
		this->find_to_draw(this->to_draw, child.bbMap, child.changing_position);
		to_draw_new = this->to_draw;
		this->to_draw = to_draw_old;

		chresult rst;
		if (in_cash(tmp_pcs->bbMap, to_draw_new, &rst) == true)
		{
			this->set_result(rst);
			continue;
		}

		if (this->add_element(child) == false)
		{
			this->add_result(CHDRAW);
			continue;
		}
		this->next_record->to_draw = to_draw_new;
		return this->next_record;
	}
	return nullptr;
}

void MainQueue::find_to_draw(int count_parent, bitboard* bbMap, bool changing_position)
{
	int checkers[4];

	for (int i = 0; i<4; i++)
	{
		checkers[i] = __popcnt(bbMap[i]);
	}
	checkers[0] -= checkers[2];
	checkers[1] -= checkers[3];

	bool change = changing_position ^ this->now->changing_position;

	int out;

	__assume(change != false);
	if (change == false)
	{
		out = count_parent - 1;
	}
	else
	{
		out = 50;

		if (checkers[2] == 0 && checkers[2] == checkers[3])
		{
			if (checkers[0] + checkers[1] < 4)
				out = 5;

			if ((checkers[0] == 3 && checkers[1] == 1) |
				(checkers[0] == 1 && checkers[1] == 3))
				out = 30;
		}
	}
	this->to_draw = out;
}

void MainQueue::add_result_deep()
{
	int checkers[4];
	bool active = (this->now->color == CHWHITE);

	for (int i = 0; i<4; i++)
	{
		checkers[i] = __popcnt(this->now->bbMap[i]);
	}
	checkers[0] -= checkers[2];
	checkers[1] -= checkers[3];

	int result = ((checkers[2] - checkers[3]) + (checkers[0] - checkers[1]) * 3)*(2 * active - 1);

	if (result > 0) this->now->result = CHWIN;
	else if (result == 0) this->now->result = CHDRAW;
	else this->now->result = CHLOSE;
}