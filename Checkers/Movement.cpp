#include "Movement.hpp"
#include "LocalMovement.hpp"
#include "Converts.hpp"
#include "BitBoard.hpp"

#define active (character->color == CHWHITE)

void Movement::Run(Queue *character)
{
	bitboard *bbMap = character->bbMap;

	bitboard all = bbMap[ALL_BLACK] | bbMap[ALL_WHITE];
	bitboard enemy = bbMap[ALL_WHITE ^ active];
	bitboard empty = ~all;
	bitboard checkers = bbMap[CHECKERS_BLACK ^ active];
	bitboard kings = bbMap[CHECKERS_BLACK ^ active] ^ bbMap[ALL_BLACK ^ active];

	__assume(checkers != 0);
	if (checkers != 0) ChopChecker(character, checkers, enemy, empty, bbMap);
	__assume(kings != 0);
	if (kings != 0) ChopKing(character, kings, enemy, empty, bbMap);

	if (character->pcs == nullptr || character->pcs->bbMap[0] == 0xFFFFFFFF)
	{
		__assume(checkers != 0);
		if (checkers != 0) MoveCheckers(character, checkers, empty, bbMap);
		__assume(kings != 0);
		if (kings != 0) MoveKing(character, kings, empty, bbMap);
	}
}

void ChopByCheckers(Queue *character, bitboard hero, bitboard enemy, bitboard empty, bitboard *bbMap, bitboard choped)
{
	bitboard tmp = 0;
	bitboard new_hero;

	bitboard can_chop_left_up = ((((hero & possible_left_up) << 1) & (enemy ^ choped) & possible_left_up) << 1) & empty;
	bitboard can_chop_right_up = SCL7(SCL7(hero & possible_right_up) & (enemy ^ choped) & possible_right_up) & empty;
	bitboard can_chop_left_down = SCR7(SCR7(hero & possible_left_down) & (enemy ^ choped) & possible_left_down) & empty;
	bitboard can_chop_right_down = ((((hero & possible_right_down) >> 1) & (enemy ^ choped) & possible_right_down) >> 1) & empty;

	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			if (can_chop_left_up)
			{
				tmp = hero << 1;
				new_hero = hero << 2;
				goto create_new;
			}
			i++;
		case 1:
			if (can_chop_right_up)
			{
				tmp = SCL7(hero);
				new_hero = SCL14(hero);
				goto create_new;
			}
			i++;
		case 2:
			if (can_chop_left_down)
			{
				tmp = SCR7(hero);
				new_hero = SCR14(hero);
				goto create_new;
			}
			i++;
		case 3:
			if (can_chop_right_down)
			{
				tmp = hero >> 1;
				new_hero = hero >> 2;
				goto create_new;
			}
			goto exit;
		}
	create_new:
		choped |= tmp;
		if (new_hero & is_new_king(character->color))
		{
			ChopByKing(character, new_hero, enemy, empty, bbMap, choped, (chtrend)i);
		}
		else
		{
			ChopByCheckers(character, new_hero, enemy, empty, bbMap, choped);
		}
		choped &= ~tmp;
	exit:
		choped = choped;
	}

	__assume(tmp == 0);
	if (tmp == 0)
	{
		bitboard a, c;
		a = bbMap[ALL_WHITE ^ active];
		c = bbMap[CHECKERS_WHITE ^ active];
		bbMap[ALL_WHITE ^ active] &= ~choped;
		bbMap[CHECKERS_WHITE ^ active] &= ~choped;
		bbMap[ALL_BLACK ^ active] |= hero;
		bbMap[CHECKERS_BLACK ^ active] |= hero;
		character->create(bbMap, AntiColor, !character->changing_position);
		bbMap[ALL_WHITE ^ active] = a;
		bbMap[CHECKERS_WHITE ^ active] = c;
		bbMap[ALL_BLACK ^ active] &= ~hero;
		bbMap[CHECKERS_BLACK ^ active] &= ~hero;
	}
}

void ChopChecker(Queue *character, bitboard checkers, bitboard enemy, bitboard empty, bitboard *bbMap)
{
	bitboard answer = 0;
	bitboard toChop;

	bitboard try_left_up = (((((checkers & possible_left_up) << 1) & enemy & possible_left_up) << 1) & empty) >> 2;
	bitboard try_right_up = SCR14(SCL7(SCL7(checkers & possible_right_up) & enemy & possible_right_up) & empty);
	bitboard try_left_down = SCL14(SCR7(SCR7(checkers & possible_left_down) & enemy & possible_left_down) & empty);
	bitboard try_right_down = (((((checkers & possible_right_down) >> 1) & enemy & possible_right_down) >> 1) & empty) << 2;

	answer = try_left_up | try_right_up | try_left_down | try_right_down;

	__assume(answer == 0);
	if (answer == 0) return;

	for (int i = 0; i < 32; i++)
	{
		if ((answer >> i) & 1)
		{
			toChop = (1 << i);
			bbMap[ALL_BLACK ^ active] &= ~toChop;
			bbMap[CHECKERS_BLACK ^ active] &= ~toChop;
			ChopByCheckers(character, toChop, enemy, empty | toChop, bbMap, (bitboard)0);
			bbMap[ALL_BLACK ^ active] |= toChop;
			bbMap[CHECKERS_BLACK ^ active] |= toChop;
		}
	}
}

bitboard FindChopKing(bitboard hero, bitboard enemy, bitboard empty, chtrend trend, bitboard* toChop)
{
	while (true)
	{
		if (hero & empty)
		{
			if (InertialMove(&hero, enemy, trend))
			{
				bitboard tmp = hero;
				if (InertialMove(&tmp, empty, trend))
				{
					*toChop = hero;
					return tmp;
				}
				else return 0;
			}
			else if (InertialMove(&hero, empty, trend))
			{
				continue;
			}
			else
			{
				return 0;
			}
		}
		else return 0;
	}
}

bool InertialMove(bitboard* hero, bitboard empty, chtrend trend)
{
	switch (trend)
	{
	case CHUPLEFT:
		if (((*hero & possible_left_up) << 1) & empty)
		{
			*hero = *hero << 1;
			return true;
		}
		return false;
	case CHUPRIGHT:
		if (SCL7(*hero & possible_right_up) & empty)
		{
			*hero = SCL7(*hero);
			return true;
		}
		return false;
	case CHDOWNLEFT:
		if (SCR7(*hero & possible_left_down) & empty)
		{
			*hero = SCR7(*hero);
			return true;
		}
		return false;
	case CHDOWNRIGHT:
		if (((*hero & possible_right_down) >> 1) & empty)
		{
			*hero = *hero >> 1;
			return true;
		}
		return false;
	}
}

void ChopByKing(Queue *character, bitboard hero, bitboard enemy, bitboard empty, bitboard *bbMap, bitboard choped, chtrend trend)
{
	bitboard toChop = 0, next_pos;

	do {
		for (int j = 0; j < 4; j++)
		{
			if (j != trend && j != (trend ^ 3))
			{
				next_pos = FindChopKing(hero, enemy ^ choped, empty, (chtrend)j, &toChop);
				if (next_pos > 0)
				{
					ChopByKing(character, next_pos, enemy, empty, bbMap, choped | toChop, (chtrend)j);
				}
			}
		}
	} while (InertialMove(&hero, empty, trend));

	if (toChop == 0)
	{
		bitboard a, c;
		a = bbMap[ALL_WHITE ^ active];
		c = bbMap[CHECKERS_WHITE ^ active];
		bbMap[ALL_WHITE ^ active] &= ~choped;
		bbMap[CHECKERS_WHITE ^ active] &= ~choped;
		MoveByKing(character, hero, empty, trend, bbMap, true);
		bbMap[ALL_WHITE ^ active] = a;
		bbMap[CHECKERS_WHITE ^ active] = c;
	}
}

void ChopKing(Queue *character, bitboard kings, bitboard enemy, bitboard empty, bitboard *bbMap)
{
	bitboard toChop;
	bitboard next_pos, to_del;

	for (int i = 0; i < 32; i++)
	{
		if ((kings >> i) & 1)
		{
			toChop = (1 << i);
			bbMap[ALL_BLACK ^ active] &= ~toChop;
			for (int j = 0; j < 4; j++)
			{
				next_pos = FindChopKing(toChop, enemy, empty | toChop, (chtrend)j, &to_del);
				if (next_pos > 0)
				{
					ChopByKing(character, next_pos, enemy, empty | toChop, bbMap, to_del, (chtrend)j);
				}
			}
			bbMap[ALL_BLACK ^ active] |= toChop;
		}
	}
}

void MoveByKing(Queue *character, bitboard hero, bitboard empty, chtrend trend, bitboard *bbMap, bool afterChop)
{
	if (afterChop)
	{
		bbMap[ALL_BLACK ^ active] |= hero;
		character->create(bbMap, AntiColor, !character->changing_position);
		bbMap[ALL_BLACK ^ active] &= ~hero;
	}

	while (InertialMove(&hero, empty, trend))
	{
		bbMap[ALL_BLACK ^ active] |= hero;
		character->create(bbMap, AntiColor, character->changing_position ^ afterChop);
		bbMap[ALL_BLACK ^ active] &= ~hero;
	}
}

void MoveKing(Queue *character, bitboard kings, bitboard empty, bitboard *bbMap)
{
	bitboard fromMove;

	for (int i = 0; i < 32; i++)
	{
		if ((kings >> i) & 1)
		{
			fromMove = (1 << i);
			bbMap[ALL_BLACK ^ active] &= ~fromMove;
			for (int j = 0; j < 4; j++)
				MoveByKing(character, fromMove, empty | fromMove, (chtrend)j, bbMap, false);
			bbMap[ALL_BLACK ^ active] |= fromMove;
		}
	}
}

void MoveByCheckers(Queue *character, bitboard hero, bitboard empty, bitboard *bbMap)
{
	bitboard tmp;
	bool change = character->changing_position;
	for (int i = 0; i < 4; i++)
	{
		if (((active << 1) ^ i) >> 1) continue;
		tmp = hero;
		if (InertialMove(&tmp, empty, (chtrend)i))
		{
			bbMap[ALL_BLACK ^ active] |= tmp;
			if (tmp & is_new_king(character->color))
			{
				change = !change;
			}
			else
			{
				bbMap[CHECKERS_BLACK ^ active] |= tmp;
			}
			character->create(bbMap, AntiColor, change);
			bbMap[ALL_BLACK ^ active] &= ~tmp;
			if (!(tmp & is_new_king(character->color))) bbMap[CHECKERS_BLACK ^ active] &= ~tmp;
		}
	}
}

void MoveCheckers(Queue *character, bitboard checkers, bitboard empty, bitboard *bbMap)
{
	bitboard fromMove;

	for (int i = 0; i < 32; i++)
	{
		if ((checkers >> i) & 1)
		{
			fromMove = (1 << i);
			bbMap[ALL_BLACK ^ active] &= ~fromMove;
			bbMap[CHECKERS_BLACK ^ active] &= ~fromMove;
			MoveByCheckers(character, fromMove, empty | fromMove, bbMap);
			bbMap[ALL_BLACK ^ active] |= fromMove;
			bbMap[CHECKERS_BLACK ^ active] |= fromMove;
		}
	}
}