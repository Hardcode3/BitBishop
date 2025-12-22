#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>

/**
 * @test BoardTest.GetWhitePieces
 * @brief Confirms that white_pieces() returns the right squares using the standard fen opening.
 */
TEST(BoardTest, GetWhitePieces) {
  Board board;  // default opening position

  Bitboard white_pieces = board.white_pieces();

  // In the default opening position, white pieces are on rank 1 and 2
  for (int sq_index = Square::A1; sq_index <= Square::H2; sq_index++) {
    EXPECT_TRUE(white_pieces.test(Square(sq_index)));
  }
}
