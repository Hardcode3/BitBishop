#pragma once

#include <bitbishop/bitboard.hpp>
#include <bitbishop/square.hpp>

/**
 * @brief Computes all queen attacks from a square, given board occupancy.
 *
 * Returns the union of rook-like (horizontal and vertical) and bishop-like
 * (diagonal) attacks reachable by a queen from the given square, stopping
 * at the first blocker in each direction (inclusive).
 *
 * This function is occupancy-dependent and performs no legality filtering:
 * it does not account for piece color, pins, check, or king safety.
 *
 * Intended as a low-level attack generator for:
 *  - legal move generation
 *  - check detection
 *  - pin detection
 *  - attack maps
 *
 * @param from     The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard of all squares attacked by the queen
 */
Bitboard queen_attacks(Square from, const Bitboard& occupied);
