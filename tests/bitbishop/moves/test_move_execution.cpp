#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/moves/move_effect.hpp>
#include <bitbishop/moves/move_execution.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

TEST(MoveExecutionTest, ApplyPlaceEffect) {
  Board board;
  MoveExecution exec;

  exec.add(MoveEffect::place(E4, WHITE_PAWN));
  exec.apply(board);

  auto piece = board.get_piece(E4);
  ASSERT_TRUE(piece.has_value());
  EXPECT_EQ(piece.value(), WHITE_PAWN);

  exec.revert(board);
  EXPECT_FALSE(board.get_piece(E4).has_value());
}

TEST(MoveExecutionTest, ApplyRemoveEffect) {
  Board board;
  MoveExecution exec;

  // Place a piece manually (outside MoveExecution)
  board.set_piece(E4, WHITE_PAWN);

  exec.add(MoveEffect::remove(E4, WHITE_PAWN));
  exec.apply(board);

  // Board should now have empty square
  EXPECT_FALSE(board.get_piece(E4).has_value());

  exec.revert(board);

  // Board should restore pawn
  auto piece = board.get_piece(E4);
  ASSERT_TRUE(piece.has_value());
  EXPECT_EQ(piece.value(), WHITE_PAWN);
}

TEST(MoveExecutionTest, RevertHappensInReverseOrder) {
  Board board;
  MoveExecution exec;

  exec.add(MoveEffect::place(A1, WHITE_ROOK));
  exec.add(MoveEffect::place(B1, WHITE_KNIGHT));
  exec.add(MoveEffect::place(C1, WHITE_BISHOP));

  exec.apply(board);

  EXPECT_TRUE(board.get_piece(A1));
  EXPECT_TRUE(board.get_piece(B1));
  EXPECT_TRUE(board.get_piece(C1));

  exec.revert(board);

  EXPECT_FALSE(board.get_piece(A1));
  EXPECT_FALSE(board.get_piece(B1));
  EXPECT_FALSE(board.get_piece(C1));
}

TEST(MoveExecutionTest, ApplyAndRevertBoardState) {
  Board board;
  MoveExecution exec;

  BoardState prev = board.get_state();
  BoardState next = prev;
  next.m_is_white_turn = !prev.m_is_white_turn;

  exec.add(MoveEffect::state_change(prev, next));
  exec.apply(board);

  EXPECT_EQ(board.get_state(), next);

  exec.revert(board);

  EXPECT_EQ(board.get_state(), prev);
}

TEST(MoveExecutionTest, ApplyAndRevertZeroEffects) {
  Board board;
  MoveExecution exec;

  Board copy = board;

  exec.apply(board);
  exec.revert(board);

  EXPECT_EQ(board, copy);
}

TEST(MoveExecutionTest, DoubleApplyDoubleRevertRestoresBoard) {
  Board board;
  Board original = board;

  MoveExecution exec;
  exec.add(MoveEffect::place(C3, WHITE_BISHOP));

  exec.apply(board);
  exec.apply(board);

  ASSERT_TRUE(board.get_piece(C3).has_value());

  exec.revert(board);
  exec.revert(board);

  EXPECT_EQ(board, original);
}
