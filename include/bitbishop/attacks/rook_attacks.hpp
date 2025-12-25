#pragma once

#include <bitbishop/bitboard.hpp>
#include <bitbishop/square.hpp>

/**
 * @brief Computes rook attacks along the north direction.
 *
 * Returns all squares reachable by a rook moving north from the given square,
 * stopping at the first occupied square (inclusive).
 *
 * This function is occupancy-dependent and intended for runtime move generation.
 *
 * @param from     The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard of attacked squares along the north direction
 */
Bitboard rook_north_attacks(Square from, const Bitboard& occupied);

/**
 * @brief Computes rook attacks along the south direction.
 *
 * Returns all squares reachable by a rook moving south from the given square,
 * stopping at the first occupied square (inclusive).
 *
 * @param from     The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard of attacked squares along the south direction
 */
Bitboard rook_south_attacks(Square from, const Bitboard& occupied);

/**
 * @brief Computes rook attacks along the east direction.
 *
 * Returns all squares reachable by a rook moving east from the given square,
 * stopping at the first occupied square (inclusive).
 *
 * @param from     The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard of attacked squares along the east direction
 */
Bitboard rook_east_attacks(Square from, const Bitboard& occupied);

/**
 * @brief Computes rook attacks along the west direction.
 *
 * Returns all squares reachable by a rook moving west from the given square,
 * stopping at the first occupied square (inclusive).
 *
 * @param from     The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard of attacked squares along the west direction
 */
Bitboard rook_west_attacks(Square from, const Bitboard& occupied);

/**
 * @brief Computes all rook attacks from a square, given board occupancy.
 *
 * Combines the four orthogonal attack directions (north, south, east, west)
 * and returns the full set of squares attacked by a rook on the given square.
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
 * @return Bitboard of all squares attacked by the rook
 */
Bitboard rook_attacks(Square from, const Bitboard& occupied);
