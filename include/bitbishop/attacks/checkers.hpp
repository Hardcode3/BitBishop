#pragma once
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/config.hpp>
#include <bitbishop/square.hpp>

/**
 * @brief Computes the set of enemy pieces currently giving check to a king.
 *
 * Returns a bitboard containing the squares of all pieces of color @p attacker
 * that are directly attacking the king on @p king_sq in the current position.
 *
 * The result is occupancy-aware and exact:
 * - Sliding attacks respect blockers
 * - Only real pieces are included
 * - The bit count corresponds to single / double check
 *
 * @param board     Current board position
 * @param king_sq   Square of the king being attacked
 * @param attacker  Color of the attacking side
 * @return Bitboard of checking piece squares
 */
Bitboard compute_checkers(const Board& board, Square king_sq, Color attacker);
