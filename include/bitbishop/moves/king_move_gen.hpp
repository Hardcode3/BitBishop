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
 * King can move to each available adjacent square.
 */
namespace KingMoveGenerator {
/**
 * @brief Generates all pseudo-legal king moves for a given side.
 *
 * Pseudo-legal means the move is valid according to piece movement rules,
 * but may leave the king in check. These moves must be validated separately.
 *
 * @param board Current board state
 * @param side Color of the side to generate moves for
 * @return Vector of all pseudo-legal king moves
 */
void generate_pseudo_legal_moves(std::vector<Move>& moves, const Board& board, Color side);

/**
 * @brief Generates all legal king moves for a given side.
 *
 * Legal moves are pseudo-legal moves that don't leave the king in check.
 *
 * @param board Current board state
 * @param side Color of the side to generate moves for
 * @return Vector of all legal king moves
 */
void generate_legal_moves(std::vector<Move>& moves, const Board& board, Color side);

/**
 * @brief Adds king_castling moves to the move list.
 *
 * Ensurue castling conditions are met before adding castling moves to the move list.
 *
 * https://www.chess.com/article/view/kingside-castling-or-queenside-castling
 */
void add_king_castling(std::vector<Move>& moves, Square from, Color side, const Board& board);

/**
 * @brief Checks if the king and rook can castle kingside.
 *
 * Kingside castling is available when:
 *
 * - Neither the kingside rook nor the king have previously moved
 * - There are no pieces between the king and the kingside rook
 * - The king is not currently in check
 *
 * @param board Board The board state in which king and rook state must be checked
 * @param side Color Color of the side to check
 * @return true if Board's state allows for kingside castling, false otherwise
 *
 */
bool can_castle_kingside(const Board& board, Color side);

/**
 * @brief Checks if the king and rook can castle queenside.
 *
 * queenside castling is available when:
 *
 * - Neither the queenside rook nor the king have previously moved
 * - There are no pieces between the king and the queenside rook
 * - The king is not currently in check
 *
 * @param board Board The board state in which king and rook state must be checked
 * @param side Color Color of the side to check
 * @return true if Board's state allows for queenside castling, false otherwise
 *
 */
bool can_castle_queenside(const Board& board, Color side);
};  // namespace KingMoveGenerator
