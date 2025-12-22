#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>

/**
 * @test BoardTest.GetBlackPieces
 * @brief Confirms that black_pieces() returns the right squares using the standard fen opening.
 */
TEST(BoardTest, GetBlackPieces) {
  Board board;  // default opening position

  Bitboard black_pieces = board.black_pieces();

  // In the default opening position, black pieces are on rank 7 and 8
  for (int sq_index = Square::A7; sq_index <= Square::H8; sq_index++) {
    EXPECT_TRUE(black_pieces.test(Square(sq_index)));
  }
}
