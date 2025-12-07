#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/square.hpp>

/**
 * @test Equality and inequality operators.
 * @brief Confirms bitboards compare equal only when they have identical bits.
 */
TEST(BitboardTest, EqualityOperators) {
  Bitboard a, b;
  a.set(Square::E2);
  b.set(Square::E2);

  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);

  b.set(Square::E4);
  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a != b);
}
