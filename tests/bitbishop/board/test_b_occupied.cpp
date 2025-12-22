#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>

/**
 * @test BoardTest.Occupied
 * @brief Validates that occupied() returns all populated squares.
 */
TEST(BoardTest, OccupiedBoardSquares) {
  Board board;  // default opening position

  Bitboard populated = board.occupied();

  // Occupied squares for the default starting position correspond to ranks 1, 2, 7, 8
  for (int sq_index = Square::A1; sq_index <= Square::H2; sq_index++) {
    EXPECT_TRUE(populated.test(Square(sq_index)));
  }
  for (int sq_index = Square::A7; sq_index <= Square::H8; sq_index++) {
    EXPECT_TRUE(populated.test(Square(sq_index)));
  }
}