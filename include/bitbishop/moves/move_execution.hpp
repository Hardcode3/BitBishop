#pragma once

#include <array>
#include <bitbishop/moves/move_effect.hpp>

/**
 * @brief Aggregates the individual effects of a single move.
 *
 * A chess move may consist of several low-level effects
 * (piece placements/removals, board state updates).
 * MoveExecution stores these in order, allowing the move
 * to be applied and fully reverted.
 */
struct MoveExecution {
  static constexpr int MAX_EFFECTS = 6;  ///< Maximum number of effects per move

  std::array<MoveEffect, MAX_EFFECTS> effects;  ///< Ordered list of effects
  int count = 0;                                ///< Number of effects currently stored

  /**
   * @brief Adds a new effect to the execution.
   * @param effect The effect to append
   * @warning Exceeding MAX_EFFECTS is undefined behaviour
   */
  void add(const MoveEffect& effect);

  /**
   * @brief Applies all effects in order.
   * @param board Board to modify
   */
  void apply(Board& board) const;

  /**
   * @brief Reverts all effects in reverse order.
   * @param board Board to restore
   */
  void revert(Board& board) const;
};
