#ifndef CONVERTS_HPP
#define CONVERTS_HPP

#include "Movement.hpp"

void bbTolistbb(bitboard* elemListbbMap, bitboard* bbMap, chcolor color, bool changing_position);
void bbFromlistbb(bitboard* elemListbbMap, bitboard** bbMap, chcolor* color, bool* changing_position);

#endif /* CONVERTS_HPP */