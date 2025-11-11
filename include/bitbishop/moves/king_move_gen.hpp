#pragma once
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/lookups/king.hpp>
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

/**
 * @brief Checks if kingside castling is legal.
 *
 * Kingside castling (also called short castling or O-O) is legal when:
 * - Neither the king nor the kingside rook have previously moved
 * - All squares between the king and the kingside rook are empty
 * - The king is not currently in check
 * - The king does not pass through or land on a square that is under attack
 *
 * @param board Current board state
 * @param side Color of the side to check
 * @return true if kingside castling is legal, false otherwise
 */
bool can_castle_kingside(const Board& board, Color side);

/**
 * @brief Checks if queenside castling is legal.
 *
 * Queenside castling (also called long castling or O-O-O) is legal when:
 * - Neither the king nor the queenside rook have previously moved
 * - All squares between the king and the queenside rook are empty
 * - The king is not currently in check
 * - The king does not pass through or land on a square that is under attack
 *
 * @param board Current board state
 * @param side Color of the side to check
 * @return true if queenside castling is legal, false otherwise
 */
bool can_castle_queenside(const Board& board, Color side);

};  // namespace KingMoveGenerator
