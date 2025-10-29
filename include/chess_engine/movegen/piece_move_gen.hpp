#pragma once

#include <chess_engine/board.hpp>
#include <chess_engine/color.hpp>
#include <chess_engine/move.hpp>
#include <vector>

/**
 * @class PieceMoveGenerator
 * @brief Abstract base class for generating moves for a specific chess piece type.
 *
 * This class defines the interface for generating both pseudo-legal and legal moves
 * for a given piece type and color. Derived classes must implement the move generation logic.
 */
class PieceMoveGenerator {
 public:
  /**
   * @brief Virtual destructor for safe polymorphic deletion.
   */
  virtual ~PieceMoveGenerator() = default;

  /**
   * @brief Generates all pseudo-legal moves for this piece type and color.
   *
   * Pseudo-legal moves are moves that do not consider checks or the game's current state.
   * Derived classes must implement this method to provide piece-specific logic.
   *
   * @param board The current state of the chess board.
   * @param side The color of the pieces to generate moves for.
   * @return std::vector<Move> A list of pseudo-legal moves.
   */
  virtual std::vector<Move> generate_pseudo_legal_moves(const Board& board, Color side) const = 0;

  /**
   * @brief Generates all legal moves for this piece type and color (filtered for checks).
   *
   * Legal moves are pseudo-legal moves that do not leave the king in check.
   * Derived classes must implement this method to filter out illegal moves.
   *
   * @param board The current state of the chess board.
   * @param side The color of the pieces to generate moves for.
   * @return std::vector<Move> A list of legal moves.
   */
  virtual std::vector<Move> generate_legal_moves(const Board& board, Color side) const = 0;
};
