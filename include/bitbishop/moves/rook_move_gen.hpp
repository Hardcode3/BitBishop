#pragma once
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/move.hpp>
#include <vector>

/**
 * @brief Generates rook moves.
 *
 * Rooks can move horizontally and vertically in the four directions (N, S, E, W) until a friendly
 * or enemy piece is encountered.
 * This namespace provides functions for generating both pseudo-legal and legal rook moves.
 */
namespace RookMoveGenerator {

/**
 * @brief Generates all pseudo-legal rook moves for a given side.
 *
 * Pseudo-legal moves follow piece movement rules but may leave the king in check.
 * These moves must be validated separately to ensure legality.
 *
 * @param moves Vector to append generated moves to
 * @param board Current board state
 * @param side Color of the side to generate moves for
 */
void generate_pseudo_legal_moves(std::vector<Move>& moves, const Board& board, Color side);

/**
 * @brief Adds castling moves to the move list if conditions are met.
 *
 * Validates castling legality and adds both kingside and queenside castling
 * moves when available. Castling requires that the king and rook haven't moved,
 * squares between them are empty, and the king doesn't move through check.
 *
 * @param moves Vector to append castling moves to
 * @param from Square the king is currently on
 * @param side Color of the side attempting to castle
 * @param board Current board state
 *
 * @see https://www.chess.com/article/view/how-to-castle-in-chess
 */
void add_rook_castling(std::vector<Move>& moves, Square from, Color side, const Board& board);

/**
 * @brief Computes the north ray from a square, stopping at the first blocker.
 *
 * @param from The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard containing all squares occupied by friendly or enemy pieces
 */
Bitboard north_ray(Square from, const Bitboard& occupied);

/**
 * @brief Computes the south ray from a square, stopping at the first blocker.
 *
 * @param from The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard containing all squares occupied by friendly or enemy pieces
 */
Bitboard south_ray(Square from, const Bitboard& occupied);

/**
 * @brief Computes the east ray from a square, stopping at the first blocker.
 *
 * @param from The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard containing all squares occupied by friendly or enemy pieces
 */
Bitboard east_ray(Square from, const Bitboard& occupied);

/**
 * @brief Computes the west ray from a square, stopping at the first blocker.
 *
 * @param from The starting square
 * @param occupied Bitboard of all occupied squares on the board
 * @return Bitboard containing all squares occupied by friendly or enemy pieces
 */
Bitboard west_ray(Square from, const Bitboard& occupied);

};  // namespace RookMoveGenerator