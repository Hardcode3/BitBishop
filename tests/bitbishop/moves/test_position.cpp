#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/moves/position.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

TEST(PositionTest, ApplyMoveUpdatesBoard) {
  Board board = Board::Empty();
  board.set_piece(E2, WHITE_PAWN);

  Position pos(board);

  Move move = Move::make(E2, E4, false);

  pos.apply_move(move);

  // The pawn should now be on E4
  EXPECT_EQ(pos.get_board().get_piece(E4), WHITE_PAWN);

  // The original square should be empty
  EXPECT_FALSE(pos.get_board().get_piece(E2).has_value());
}

TEST(PositionTest, RevertMoveRestoresBoard) {
  Board board = Board::Empty();
  board.set_piece(E2, WHITE_PAWN);

  Position pos(board);

  Move move = Move::make(E2, E4, false);

  pos.apply_move(move);

  // Apply move changed board
  EXPECT_EQ(pos.get_board().get_piece(E4), WHITE_PAWN);
  EXPECT_FALSE(pos.get_board().get_piece(E2).has_value());

  pos.revert_move();

  // Revert should restore original board
  EXPECT_EQ(pos.get_board().get_piece(E2), WHITE_PAWN);
  EXPECT_FALSE(pos.get_board().get_piece(E4).has_value());
}

TEST(PositionTest, CanUnmakeReflectsMoveHistory) {
  Board board = Board::Empty();
  board.set_piece(E2, WHITE_PAWN);

  Position pos(board);

  // No moves yet
  EXPECT_FALSE(pos.can_unmake());

  Move move = Move::make(E2, E4, false);
  pos.apply_move(move);

  // After one move, can_unmake should be true
  EXPECT_TRUE(pos.can_unmake());

  pos.revert_move();

  // After revert, history is empty
  EXPECT_FALSE(pos.can_unmake());
}
