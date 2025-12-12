#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/square.hpp>

/**
 * @test bool operator
 * @brief Confirms boolean works as intended when no square is populated.
 */
TEST(BitboardTest, BoolOperatorFalseWhenEmpty) {
  Bitboard bb;

  EXPECT_FALSE(static_cast<bool>(bb));
}

/**
 * @test bool operator
 * @brief Confirms boolean works as intended when one square is populated.
 */
TEST(BitboardTest, BoolOperatorTrueWhenSingleBit) {
  Bitboard bb;
  bb.set(Square::C4);

  EXPECT_TRUE(static_cast<bool>(bb));
}

/**
 * @test bool operator
 * @brief Confirms boolean works as intended when multiple squares are populated.
 */
TEST(BitboardTest, BoolOperatorTrueWhenMultipleBits) {
  Bitboard bb;
  bb.set(Square::C5);
  bb.set(Square::H2);
  bb.set(Square::A1);

  EXPECT_TRUE(static_cast<bool>(bb));
}