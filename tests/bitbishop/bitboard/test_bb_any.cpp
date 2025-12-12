#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/square.hpp>

/**
 * @test any() method
 * @brief Confirms any() returns false for an empty bitboard.
 */
TEST(BitboardTest, AnyReturnsFalseWhenEmpty) {
  Bitboard bb;
  EXPECT_FALSE(bb.any());
}

/**
 * @test any() method
 * @brief Confirms any() returns true when a single bit is set.
 */
TEST(BitboardTest, AnyReturnsTrueWhenSingleBit) {
  Bitboard bb;
  bb.set(Square::E4);
  EXPECT_TRUE(bb.any());
}

/**
 * @test any() method
 * @brief Confirms any() returns true when multiple bits are set.
 */
TEST(BitboardTest, AnyReturnsTrueWhenMultipleBits) {
  Bitboard bb;
  bb.set(Square::A1);
  bb.set(Square::H8);
  bb.set(Square::D4);
  EXPECT_TRUE(bb.any());
}

/**
 * @test any() method
 * @brief Confirms any() returns false after clearing all bits.
 */
TEST(BitboardTest, AnyReturnsFalseAfterClear) {
  Bitboard bb;
  bb.set(Square::E4);
  bb.set(Square::D5);
  bb.clear(Square::E4);
  bb.clear(Square::D5);
  EXPECT_FALSE(bb.any());
}

/**
 * @test any() method
 * @brief Confirms any() is equivalent to bool operator.
 */
TEST(BitboardTest, AnyEquivalentToBoolOperator) {
  Bitboard bb_empty;
  EXPECT_EQ(bb_empty.any(), static_cast<bool>(bb_empty));

  Bitboard bb_set;
  bb_set.set(Square::C3);
  EXPECT_EQ(bb_set.any(), static_cast<bool>(bb_set));
}

/**
 * @test any() method
 * @brief Confirms any() is equivalent to count() > 0.
 */
TEST(BitboardTest, AnyEquivalentToCountGtZero) {
  Bitboard bb_empty;
  EXPECT_EQ(bb_empty.count() > 0, static_cast<bool>(bb_empty));

  Bitboard bb_set;
  bb_set.set(Square::C3);
  EXPECT_EQ(bb_set.count() > 0, static_cast<bool>(bb_set));
}
