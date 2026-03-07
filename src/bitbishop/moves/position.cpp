#include <bitbishop/attacks/checkers.hpp>
#include <bitbishop/moves/move_builder.hpp>
#include <bitbishop/moves/position.hpp>

void Position::apply_move(const Move& move) {
  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();
  move_execution_history.push_back(exec);
  exec.apply(board);
}

void Position::revert_move() {
  if (can_unmake()) {
    MoveExecution last_exec = move_execution_history.back();
    last_exec.revert(board);
    move_execution_history.pop_back();
  }
}

[[nodiscard]] bool Position::is_in_check() const {
  Color us = board.get_side_to_move();
  Color them = ColorUtil::opposite(us);
  Square king_square = board.king_square(us).value();
  const Bitboard checkers = compute_checkers(board, king_square, them);
  return checkers.any();
}
