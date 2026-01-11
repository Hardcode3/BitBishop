#pragma once

#include <bitbishop/board.hpp>
#include <bitbishop/moves/move_execution.hpp>
#include <vector>

class Position {
 private:
  Board board;
  std::vector<MoveExecution> move_execution_history;

 public:
  Position() = delete;
  Position(Board&) = delete;
  explicit Position(Board&& initial) : board(std::move(initial)) { ; }

  void apply_move(const Move& move);
  void revert_move();

  [[nodiscard]] const Board& get_board() const { return board; }

  [[nodiscard]] bool can_unmake() const { return !move_execution_history.empty(); }
};
