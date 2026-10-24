#include <bitbishop/moves/move_execution.hpp>

namespace bitbishop {

void MoveExecution::add(const MoveEffect& effect) { effects[count++] = effect; }

void MoveExecution::apply(Board& board) const {
  for (int i = 0; i < count; ++i) {
    effects[i].apply(board);
  }
}

void MoveExecution::revert(Board& board) const {
  for (int i = count - 1; i >= 0; --i) {
    effects[i].revert(board);
  }
}

}  // namespace bitbishop