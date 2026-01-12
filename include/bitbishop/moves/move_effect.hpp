#pragma once

#include <bitbishop/board.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>

/**
 * @brief Represents a single low-level board modification.
 *
 * A chess move may consist of multiple effects: placing a piece, removing a
 * piece, or updating board state (e.g., castling rights, en passant). Each
 * MoveEffect describes exactly one such change and can be applied or reverted.
 */
struct MoveEffect {
  /**
   * @brief Type of board modification.
   */
  enum class Type : uint8_t { Place, Remove, BoardState };

  Type type;                         ///< Effect category
  Square square = Squares::A1;       ///< Target square (for Place/Remove)
  Piece piece = Pieces::WHITE_KING;  ///< Piece involved (for Place/Remove)
  BoardState prev_state;             ///< State before change (for BoardState effect)
  BoardState next_state;             ///< State after change (for BoardState effect)

  /**
   * @brief Creates a piece placement effect.
   * @param sq   Destination square
   * @param piece Piece to place
   */
  static MoveEffect place(Square sq, Piece piece);

  /**
   * @brief Creates a piece removal effect.
   * @param sq   Square to clear
   * @param piece Piece being removed
   */
  static MoveEffect remove(Square sq, Piece piece);

  /**
   * @brief Creates a board state update effect.
   * @param prev The previous state
   * @param next The new state
   */
  static MoveEffect state_change(const BoardState& prev, const BoardState& next);

  /**
   * @brief Applies the effect to the board.
   * @param board Board to modify
   */
  void apply(Board& board) const;

  /**
   * @brief Reverts the effect on the board.
   * @param board Board to restore
   */
  void revert(Board& board) const;
};
