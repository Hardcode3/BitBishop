#pragma once

#include <bitbishop/board.hpp>
#include <bitbishop/moves/move_execution.hpp>
#include <vector>

/**
 * @brief Represents a game position, including board state and move history.
 *
 * A Position owns a Board instance and provides the ability to apply and revert moves.
 * It is primarily responsible for maintaining a consistent board state across move
 * generation, execution, and undo operations.
 *
 * Key responsibilities:
 * - Track the current board state
 * - Execute moves and record them for later rollback
 * - Revert moves safely via the stored execution history
 *
 * The class is intentionally non-copyable and non-default-constructible to avoid
 * ambiguous or partially-initialized board states. A fully-initialized Board must be
 * provided at construction time.
 */
class Position {
 private:
  /**
   * @brief The current board state of the position.
   *
   * This board is updated every time a move is applied or reverted. The Position
   * class provides controlled access to this board to ensure move history and
   * board state remain synchronized.
   */
  Board board;

  /**
   * @brief History of executed moves, including auxiliary information.
   *
   * Each entry contains the data necessary to revert a move accurately, including
   * captured pieces, castling rights changes, en passant information, etc.
   *
   * Moves are pushed when applied and popped when reverted.
   */
  std::vector<MoveExecution> move_execution_history;

 public:
  Position() = delete;        ///< Must be initialized with a Board
  Position(Board&) = delete;  ///< Prevent accidental copy from lvalue Board

  /**
   * @brief Constructs a Position from an initial board state.
   *
   * Although this is declared as a move constructor, Board is trivially copyable,
   * so the actual internal assignment is effectively a copy. This constructor
   * ensures that the Position starts with a valid, fully defined board.
   *
   * @param initial The initial board state (rvalue reference)
   */
  explicit Position(Board&& initial) : board(initial) {
    ;  // board is trivially copyable, no move, just a copy behind the scenes
  }

  /**
   * @brief Applies a move to the current position.
   *
   * This method:
   * - Computes the full execution details of the move
   * - Updates the board state accordingly
   * - Stores the MoveExecution record so the move can be reverted later
   *
   * @param move The move to apply
   */
  void apply_move(const Move& move);

  /**
   * @brief Reverts the last applied move.
   *
   * Pops the most recent entry from the move execution history and restores the
   * board to its previous state. Calling this function when no moves have been
   * applied have no effect..
   */
  void revert_move();

  /**
   * @brief Returns a const reference to the current board.
   *
   * Provides read-only access to the board; callers must not attempt to mutate
   * the board directly to avoid desynchronizing board state and move history.
   *
   * @return Const reference to the current Board
   */
  [[nodiscard]] const Board& get_board() const { return board; }

  /**
   * @brief Checks whether a previously applied move can be reverted.
   *
   * @return true if at least one move has been applied, false otherwise
   */
  [[nodiscard]] bool can_unmake() const { return !move_execution_history.empty(); }
};
