#ifndef MOVEMENT_HPP
#define MOVEMENT_HPP

#include "Queue.hpp"
#include "BitBoard.hpp"

enum chtrend
{
	CHUPLEFT = 0,
	CHUPRIGHT = 1,
	CHDOWNLEFT = 2,
	CHDOWNRIGHT = 3
};

enum chres	/* result of operations */
{
	CHSUCCESS,
	CHERROR
};

enum chinfmod
{
	CHCURRENT,
	CHNEXT,
	CHBNEXT	/* before next */
};

#define AntiColor ((character->color == CHWHITE) ? CHBLACK : CHWHITE)

class Movement
{
public:
	void Run(Queue*);
};

#endif // MOVEMENT_HPP