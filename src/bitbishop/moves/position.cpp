#include <algorithm>
#include <bitbishop/attacks/checkers.hpp>
#include <bitbishop/moves/move_builder.hpp>
#include <bitbishop/moves/position.hpp>
#include <cassert>

void Position::apply_move(const Move& move) {
  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();
  move_execution_history.push_back(exec);
  exec.apply(board);
  zobrist_hashes_history.push_back(board.get_zobrist_hash());
}

void Position::revert_move() {
  if (can_unmake()) {
    assert(!zobrist_hashes_history.empty());
    assert(board.get_zobrist_hash() == zobrist_hashes_history.back());

    MoveExecution last_exec = move_execution_history.back();
    last_exec.revert(board);
    move_execution_history.pop_back();
    zobrist_hashes_history.pop_back();

    assert(!zobrist_hashes_history.empty());
    assert(board.get_zobrist_hash() == zobrist_hashes_history.back());
  }
}

[[nodiscard]] int Position::repetition_count() const noexcept {
  assert(!zobrist_hashes_history.empty());

  const Zobrist::Key key = zobrist_hashes_history.back();

  int count = 1;

  const int halfmove_clock = board.get_state().m_halfmove_clock;

  // If a pawn moved 2 turns ago, it is mathematically impossible for the current position to have occurred 4 turns ago
  // (a pawn move is irreversible).
  // Therefore, we only need to look back as far as the halfmove_clock allows.
  // We also ensure we don't look back further than the actual history size (to avoid index out-of-bounds).
  const int max_back_plies = std::min<int>(halfmove_clock, static_cast<int>(zobrist_hashes_history.size()) - 1);

  // An irreversible move just happened or the game just started.
  if (max_back_plies < 2) {
    return count;
  }

  const int stop_index = static_cast<int>(zobrist_hashes_history.size()) - 1 - max_back_plies;

  // For a position to be a "repetition" in chess, it’s not enough for the pieces to be on the same squares.
  // The same player must also be on move.
  // If it is White's turn now (Index N), it must have been White's turn in the repeating position:
  // - Index N: Current (White) -> size() - 1 is the current turn (cannot be the same position).
  // - Index N-1: Previous (Black) -> size() - 2 was the opponent's turn (impossible to be the same position).
  // - Index N-2: Previous (White) -> size() - 3 was the last time it was our turn.
  for (int i = static_cast<int>(zobrist_hashes_history.size()) - 3; i >= stop_index; i -= 2) {
    if (zobrist_hashes_history[static_cast<std::size_t>(i)] == key) {
      ++count;
      if (count >= Const::FIVEFOLD_REPETITION_COUNT) {
        break;
      }
    }
  }

  return count;
}

[[nodiscard]] bool Position::is_in_check() const {
  Color us = board.get_side_to_move();
  Color them = ColorUtil::opposite(us);
  Square king_square = board.king_square(us).value();
  const Bitboard checkers = compute_checkers(board, king_square, them);
  return checkers.any();
}
