#pragma once
#include <chess_engine/attacks/king.hpp>
#include <chess_engine/movegen/piece_move_gen.hpp>
#include <utility>

/**
 * @brief Generates king moves.
 *
 * King can move to each available adjacent square.
 */
class KingMoveGenerator : public PieceMoveGenerator {
 public:
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
  std::vector<Move> generate_pseudo_legal_moves(const Board& board, Color side) const override;

  /**
   * @brief Generates all legal king moves for a given side.
   *
   * Legal moves are pseudo-legal moves that don't leave the king in check.
   *
   * @param board Current board state
   * @param side Color of the side to generate moves for
   * @return Vector of all legal king moves
   */
  std::vector<Move> generate_legal_moves(const Board& board, Color side) const override;
};
