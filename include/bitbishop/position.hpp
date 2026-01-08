#pragma once

#include <move_builder.hpp>

class Position {
 private:
  Board m_board;
  std::vector<MoveExecution> m_history;

 public:
  Position() {}

  explicit Position(const Board& initial) {}

  void apply_move(const Move& move) {}
  void revert_move() {}

  const Board& get_board() const {}
  Board& board() {}

  bool can_unmake() const {}
};
