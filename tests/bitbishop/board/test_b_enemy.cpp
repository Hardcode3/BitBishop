#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>

/**
 * @test BoardTest.EnemyBitboard
 * @brief Confirms that enemy() returns the opposing side’s occupied squares.
 */
TEST(BoardTest, EnemyBitboard) {
  Board board;  // default opening position

  Bitboard white_enemy = board.enemy(Color::WHITE);
  Bitboard black_enemy = board.enemy(Color::BLACK);

  // In the default opening position, white pieces are on rank 1 and 2
  for (int sq_index = Square::A1; sq_index <= Square::H2; sq_index++) {
    EXPECT_TRUE(black_enemy.test(Square(sq_index)));
  }

  // In the default opening position, black pieces are on rank 7 and 8
  for (int sq_index = Square::A7; sq_index <= Square::H8; sq_index++) {
    EXPECT_TRUE(white_enemy.test(Square(sq_index)));
  }
}
