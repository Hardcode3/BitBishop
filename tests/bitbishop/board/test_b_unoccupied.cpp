#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>

/**
 * @test BoardTest.Unoccupied
 * @brief Validates that unoccupied() returns all unoccupied squares.
 */
TEST(BoardTest, UnoccupiedBoardSquares) {
  Board board;  // default opening position

  Bitboard empty = board.unoccupied();

  // Empty squares for the default starting position
  // corresponds to ranks 3, 4, 5, 6
  for (int sq_index = Square::A3; sq_index <= Square::H6; sq_index++) {
    EXPECT_TRUE(empty.test(Square(sq_index)));
  }
}
