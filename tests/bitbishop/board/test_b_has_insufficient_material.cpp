#include <gtest/gtest.h>

#include <bitbishop/board.hpp>

TEST(BoardTest, TwoKingsIsInsufficientMaterial) {
  Board board("8/8/8/8/4k3/8/8/2K5 w KQkq - 0 1");
  EXPECT_TRUE(board.has_insufficient_material());
}

TEST(BoardTest, KN_vs_k_IsInsufficientMaterial) {
  Board board("8/8/8/8/4k3/2N5/8/2K5 b KQkq - 0 1");
  EXPECT_TRUE(board.has_insufficient_material());
}

TEST(BoardTest, KB_vs_k_IsInsufficientMaterial) {
  Board board("8/8/8/1B6/4k3/8/8/2K5 b KQkq - 0 1");
  EXPECT_TRUE(board.has_insufficient_material());
}

TEST(BoardTest, K_vs_kn_IsInsufficientMaterial) {
  Board board("8/8/6n1/8/4k3/8/8/2K5 w KQkq - 0 1");
  EXPECT_TRUE(board.has_insufficient_material());
}

TEST(BoardTest, K_vs_kb_IsInsufficientMaterial) {
  Board board("8/2b5/8/8/4k3/8/8/2K5 w KQkq - 0 1");
  EXPECT_TRUE(board.has_insufficient_material());
}

TEST(BoardTest, KB_vs_kb_SameColorIsInsufficientMaterial) {
  Board board_bishops_on_black_square("8/2b5/8/8/4k3/8/5B2/2K5 b KQkq - 0 1");
  EXPECT_TRUE(board_bishops_on_black_square.has_insufficient_material());

  Board board_bishops_on_white_square("8/3b4/8/8/4k3/8/4B3/2K5 b KQkq - 0 1");
  EXPECT_TRUE(board_bishops_on_white_square.has_insufficient_material());
}

TEST(BoardTest, KN_vs_kn_IsInsufficientMaterial) {
  Board board("8/2N5/4n3/8/4k3/8/8/2K5 w KQkq - 0 1");
  EXPECT_TRUE(board.has_insufficient_material());
}

TEST(BoardTest, StartingPosIsNotInsufficientMaterial) {
  Board board = Board::StartingPosition();
  EXPECT_FALSE(board.has_insufficient_material());
}
