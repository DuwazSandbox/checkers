#include "MainQueue.hpp"
#include "Cash_memory.hpp"

int main(void)
{
	int color = CHBLACK;
	Movement Work;
	MainQueue *viewQueue = new MainQueue();
	viewQueue->deep = 1;
	Queue startQueue1, *__restrict startQueue;
	startQueue=(&startQueue1);

	char Map[8][8]={
		/*'#',-1, '#',-1, '#', 1, '#',-2, // first position
		-1, '#',-1, '#',-1, '#', 0, '#',
		'#',-1,	'#',-1, '#',-1, '#',-1,
		-1,	'#', 1, '#',-1, '#',-1, '#',
		'#', 1, '#', 1, '#', 1, '#', 1,
		 0, '#', 1,	'#', 1, '#', 1, '#',
		'#', 0, '#', 1,	'#', 1,	'#', 1,
		-1,	'#', 2,	'#', 1,	'#', 1,	'#'*/
		'#', 0, '#', 0, '#', 0, '#', 0, // second position
		 0, '#', 0, '#', 0, '#',-1, '#',
		'#', 0,	'#', 0, '#', 0, '#', 0,
		 0,	'#', 0, '#',-1, '#', 0, '#',
		'#', 0, '#', 1, '#', 0, '#', 0,
		 0, '#', 1, '#', 0, '#', 0, '#',
		'#', 1, '#', 0,	'#', 0,	'#', 1,
		 1, '#', 0,	'#', 0,	'#', 0,	'#'
		/*'#',-1, '#',-1, '#',-1, '#',-1, // third position
		-1, '#',-1, '#',-1, '#',-1, '#',
		'#',-1,	'#',-1, '#',-1, '#',-1,
		 0,	'#', 0, '#', 0, '#', 0, '#',
		'#', 0, '#', 0, '#', 0, '#', 0,
		 1, '#', 1,	'#', 1, '#', 1, '#',
		'#', 1, '#', 1,	'#', 1,	'#', 1,
		 1,	'#', 1,	'#', 1,	'#', 1,	'#'*/
	};

	(*startQueue).loading(Map);
	viewQueue->now = startQueue;
	viewQueue->parent = nullptr;
	clean_memory();
	
	static int a1 = 0;
	static int a2 = 0;
	static int a3 = 0;

	Work.Run(viewQueue->now);
	viewQueue = viewQueue->next();

	__assume(viewQueue != nullptr);
	while (viewQueue != nullptr)
	{
		a1++;
		a2 += a1 / 1000000;
		a3 += a2 / 1000000;
		a1 %= 1000000;
		a2 %= 1000000;

		Work.Run(viewQueue->now);
		viewQueue = viewQueue->next();
	}
	viewQueue = viewQueue;
}