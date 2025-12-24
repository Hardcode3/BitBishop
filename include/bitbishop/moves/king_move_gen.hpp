#pragma once
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/lookups/king_attacks.hpp>
#include <bitbishop/move.hpp>
#include <utility>
#include <vector>

/**
 * @brief Generates king moves.
 *
 * The king can move one square in any direction (horizontally, vertically, or diagonally).
 * This namespace provides functions for generating both pseudo-legal and legal king moves,
 * including castling moves.
 */
namespace KingMoveGenerator {

/**
 * @brief Generates all pseudo-legal king moves for a given side.
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
 * @brief Generates all legal king moves for a given side.
 *
 * Legal moves are pseudo-legal moves that have been validated to ensure
 * the king is not left in check after the move.
 *
 * @param moves Vector to append generated moves to
 * @param board Current board state
 * @param side Color of the side to generate moves for
 */
void generate_legal_moves(std::vector<Move>& moves, const Board& board, Color side);

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
void add_king_castling(std::vector<Move>& moves, Square from, Color side, const Board& board);

};  // namespace KingMoveGenerator
