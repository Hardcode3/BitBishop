#pragma once

#include <bitbishop/board.hpp>
#include <bitbishop/moves/move_execution.hpp>
#include <vector>

/**
 * @brief Represents a chess position and move history.
 *
 * Tracks a Board (by reference) and allows applying/reverting moves. The Position
 * itself does not own the Board; it modifies the provided board safely using
 * MoveExecution history.
 */
class Position {
 private:
  /** Reference to the board being managed */
  Board& board;

  /** History of executed moves for rollback */
  std::vector<MoveExecution> move_execution_history;

 public:
  Position() = delete;  ///< Default construction not allowed
  Position(Board& board) : board(board) { ; }

  /**
   * @brief Applies a move to the board and records it for undo.
   * @param move Move to apply
   */
  void apply_move(const Move& move);

  /**
   * @brief Reverts the last applied move.
   * Safe to call only if can_unmake() returns true.
   */
  void revert_move();

  /**
   * @brief Returns the current board (read-only).
   */
  [[nodiscard]] const Board& get_board() const { return board; }

  /**
   * @brief Checks if a move can be reverted.
   * @return true if move history is non-empty
   */
  [[nodiscard]] bool can_unmake() const { return !move_execution_history.empty(); }
};
