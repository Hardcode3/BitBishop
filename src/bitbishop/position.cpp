#include <bitbishop/move_builder.hpp>
#include <bitbishop/position.hpp>

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
