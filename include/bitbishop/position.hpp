#pragma once

#include <bitbishop/board.hpp>
#include <vector>

class Position {
 private:
  Board board;
  std::vector<MoveExecution> move_execution_history;

 public:
  Position() = delete;
  explicit Position(Board initial) : board(std::move(initial)) { ; }

  void apply_move(const Move& move);
  void revert_move();

  const Board& get_board() const { return board; }

  bool can_unmake() const { return !move_execution_history.empty(); }
};
