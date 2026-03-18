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

  /** History of Zobrist hashes for threefold and fivefold repetition rules. */
  std::vector<Zobrist::Key> zobrist_hashes_history;

 public:
  Position() = delete;  ///< Default construction not allowed
  Position(Board& board) : board(board) { zobrist_hashes_history.push_back(board.get_zobrist_hash()); }

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

  /**
   * @brief Returns the number of occurrences of the current position in history.
   *
   * Only positions since the last pawn move or capture are considered (using the board's half-move clock),
   * since positions cannot repeat across irreversible moves.
   *
   * @return Occurrence count (includes the current position)
   */
  [[nodiscard]] int repetition_count() const noexcept;

  /**
   * @brief Returns true if the current position occurred 3 or more times.
   */
  [[nodiscard]] bool is_threefold_repetition() const noexcept {
    return repetition_count() >= Const::THREEFOLD_REPETITION_COUNT;
  }

  /**
   * @brief Returns true if the current position occurred 5 or more times.
   */
  [[nodiscard]] bool is_fivefold_repetition() const noexcept {
    return repetition_count() >= Const::FIVEFOLD_REPETITION_COUNT;
  }

  /**
   * @brief Tells if a position is in a check situation.
   * @return true if the king is in check, false otherwise
   * @note if there is no king on the board, returns false
   */
  [[nodiscard]] bool is_in_check() const;
};
