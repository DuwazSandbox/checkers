#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <stdint.h>
typedef uint32_t bitboard;

enum Map_of_bitboard
{
	ALL_WHITE = 0,
	ALL_BLACK = 1,
	CHECKERS_WHITE = 2,
	CHECKERS_BLACK = 3
};

#define possible_left_up	0x7EF4F6DE
#define possible_right_up	0x7DFDF5DD
#define possible_left_down	0xFEFAEEBE
#define possible_right_down	0xFDF9EDBC

#define SCL7(a) (((a) >> 25) | ((a) << 7))
#define SCR7(a) (((a) << 25) | ((a) >> 7))
#define SCL14(a) (((a) >> 18) | ((a) << 14))
#define SCR14(a) (((a) << 18) | ((a) >> 14))

#define is_new_king(color) ((color == CHBLACK) ? 0x80020820 : 0x00041041)

int num_in_bitboard(int i, int j);

#endif