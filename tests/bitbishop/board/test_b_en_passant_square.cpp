#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>

/**
 * @test BoardTest.EnPassantSquare
 * @brief Ensures en_passant_square() works as intented when en passant square is set.
 */
TEST(BoardTest, EnPassantSquareIsAvailable) {
  // FEN: White just played d2-d4, en passant target on d3
  Board board("8/8/8/8/3P4/8/8/8 b - d3 0 1");

  auto sq = board.en_passant_square();
  ASSERT_TRUE(sq.has_value());
  EXPECT_EQ(*sq, Squares::D3);
}

/**
 * @test BoardTest.EnPassantSquare
 * @brief Ensures en_passant_square() works as intented when en passant square is not set.
 */
TEST(BoardTest, EnPassantSquareNotAvailable) {
  // FEN with no en passant
  Board board2("8/8/8/8/8/8/8/8 w - - 0 1");
  EXPECT_FALSE(board2.en_passant_square().has_value());
}
