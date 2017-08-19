#ifndef LOCALMOVEMENT_HPP
#define LOCALMOVEMENT_HPP

void ChopByCheckers(Queue *character, bitboard hero, bitboard enemy, bitboard empty, bitboard *bbMap, bitboard choped);
void ChopChecker(Queue *character, bitboard checkers, bitboard enemy, bitboard empty, bitboard *bbMap);
bitboard FindChopKing(bitboard hero, bitboard enemy, bitboard empty, chtrend trend, bitboard* toChop);
bool InertialMove(bitboard* hero, bitboard empty, chtrend trend);
void ChopByKing(Queue *character, bitboard hero, bitboard enemy, bitboard empty, bitboard *bbMap, bitboard choped, chtrend trend);
void ChopKing(Queue *character, bitboard kings, bitboard enemy, bitboard empty, bitboard *bbMap);
void MoveByKing(Queue *character, bitboard hero, bitboard empty, chtrend trend, bitboard *bbMap, bool afterChop);
void MoveKing(Queue *character, bitboard kings, bitboard empty, bitboard *bbMap);
void MoveByCheckers(Queue *character, bitboard hero, bitboard empty, bitboard *bbMap);
void MoveCheckers(Queue *character, bitboard checkers, bitboard empty, bitboard *bbMap);

#endif