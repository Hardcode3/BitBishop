#pragma once

#include <bitbishop/bitboard.hpp>
#include <bitbishop/square.hpp>

/**
 * @brief Computes bishop attacks along the northeast diagonal.
 *
 * Returns all squares reachable by a bishop moving northeast from the given
 * square, stopping at the first occupied square (inclusive).
 *
 * This function is occupancy-dependent and intended for runtime move generation.
 *
 * @param from     The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard of attacked squares along the northeast diagonal
 */
Bitboard bishop_north_east_attacks(Square from, const Bitboard& occupied);

/**
 * @brief Computes bishop attacks along the northwest diagonal.
 *
 * Returns all squares reachable by a bishop moving northwest from the given
 * square, stopping at the first occupied square (inclusive).
 *
 * @param from     The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard of attacked squares along the northwest diagonal
 */
Bitboard bishop_north_west_attacks(Square from, const Bitboard& occupied);

/**
 * @brief Computes bishop attacks along the southeast diagonal.
 *
 * Returns all squares reachable by a bishop moving southeast from the given
 * square, stopping at the first occupied square (inclusive).
 *
 * @param from     The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard of attacked squares along the southeast diagonal
 */
Bitboard bishop_south_east_attacks(Square from, const Bitboard& occupied);

/**
 * @brief Computes bishop attacks along the southwest diagonal.
 *
 * Returns all squares reachable by a bishop moving southwest from the given
 * square, stopping at the first occupied square (inclusive).
 *
 * @param from     The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard of attacked squares along the southwest diagonal
 */
Bitboard bishop_south_west_attacks(Square from, const Bitboard& occupied);

/**
 * @brief Computes all bishop attacks from a square, given board occupancy.
 *
 * Combines the four diagonal attack directions (NE, NW, SE, SW) and returns
 * the full set of squares attacked by a bishop on the given square.
 *
 * This function performs no legality filtering (pins, check, own pieces)
 * and is intended as a low-level attack generator for higher-level move logic.
 *
 * @param from     The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard of all squares attacked by the bishop
 */
Bitboard bishop_attacks(Square from, const Bitboard& occupied);
