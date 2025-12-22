#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>

/**
 * @test BoardTest.FriendlyBitboard
 * @brief Confirms that friendly() returns the current side’s occupied squares.
 */
TEST(BoardTest, FriendlyBitboard) {
  Board board;  // default opening position

  Bitboard white_friends = board.friendly(Color::WHITE);
  Bitboard black_friends = board.friendly(Color::BLACK);

  // In the default opening position, white pieces are on rank 1 and 2
  for (int sq_index = Square::A1; sq_index <= Square::H2; sq_index++) {
    EXPECT_TRUE(white_friends.test(Square(sq_index)));
  }

  // In the default opening position, black pieces are on rank 7 and 8
  for (int sq_index = Square::A7; sq_index <= Square::H8; sq_index++) {
    EXPECT_TRUE(black_friends.test(Square(sq_index)));
  }
}