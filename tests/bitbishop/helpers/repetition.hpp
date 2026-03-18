/**
 * @file repetition.hpp
 * @brief Utility helpers for constructing repetition scenarios in tests.
 */

#pragma once

#include <bitbishop/move.hpp>
#include <bitbishop/moves/position.hpp>
#include <bitbishop/square.hpp>

/**
 * @brief Reaches the same position again after 4 plies from the starting position.
 *
 * Sequence:
 *   1. Ng1-f3  ... Ng8-f6
 *   2. Nf3-g1  ... Nf6-g8
 */
inline void apply_knight_repetition_cycle(Position& pos) {
  pos.apply_move(Move::make(Squares::G1, Squares::F3));
  pos.apply_move(Move::make(Squares::G8, Squares::F6));
  pos.apply_move(Move::make(Squares::F3, Squares::G1));
  pos.apply_move(Move::make(Squares::F6, Squares::G8));
}

