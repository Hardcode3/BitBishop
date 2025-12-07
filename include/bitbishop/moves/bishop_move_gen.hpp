#pragma once
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/move.hpp>
#include <vector>

/**
 * @brief Generates bishop moves.
 *
 * Bishop can move diagonally in the four directions (NE, NW, SE, SW) until a friendly or enemy piece
 * is encountered.
 * This namespace provides functions for generating both pseudo-legal and legal bishop moves.
 */
namespace BishopMoveGenerator {

/**
 * @brief Generates all pseudo-legal bishop moves for a given side.
 *
 * Pseudo-legal moves follow piece movement rules but may leave the king in check.
 * These moves must be validated separately to ensure legality.
 *
 * @param moves Vector to append generated moves to
 * @param board Current board state
 * @param side Color of the side to generate moves for
 */
// TODO: test
void generate_pseudo_legal_moves(std::vector<Move>& moves, const Board& board, Color side);

/**
 * @brief Computes the northeast diagonal ray from a square, stopping at the first blocker.
 *
 * @param from The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard containing all squares in the northeast direction up to and including the first blocker
 */
// TODO: test
Bitboard north_east_ray(Square from, Bitboard occupied);

/**
 * @brief Computes the northwest diagonal ray from a square, stopping at the first blocker.
 *
 * @param from The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard containing all squares in the northwest direction up to and including the first blocker
 */
// TODO: test
Bitboard north_west_ray(Square from, Bitboard occupied);

/**
 * @brief Computes the southeast diagonal ray from a square, stopping at the first blocker.
 *
 * @param from The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard containing all squares in the southeast direction up to and including the first blocker
 */
// TODO: test
Bitboard south_east_ray(Square from, Bitboard occupied);

/**
 * @brief Computes the southwest diagonal ray from a square, stopping at the first blocker.
 *
 * @param from The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard containing all squares in the southwest direction up to and including the first blocker
 */
// TODO: test
Bitboard south_west_ray(Square from, Bitboard occupied);

};  // namespace BishopMoveGenerator
