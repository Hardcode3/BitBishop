#pragma once

#include <array>
#include <bitbishop/move_effect.hpp>

struct MoveExecution {
  static constexpr int MAX_EFFECTS = 6;

  std::array<MoveEffect, MAX_EFFECTS> effects;
  int count = 0;

  void add(const MoveEffect& effect);
  void apply(Board& board) const;
  void revert(Board& board) const;
};
