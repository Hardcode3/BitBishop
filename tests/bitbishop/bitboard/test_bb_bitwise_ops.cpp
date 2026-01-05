#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/square.hpp>

/**
 * @test Bitwise OR operator.
 * @brief Combines two bitboards, keeping all occupied squares.
 */
TEST(BitboardTest, OrOperatorCombinesBitboards) {
  Bitboard a, b;
  a.set(Square::A1);
  b.set(Square::H8);

  Bitboard c = a | b;
  EXPECT_TRUE(c.test(Square::A1));
  EXPECT_TRUE(c.test(Square::H8));
  EXPECT_FALSE(c.test(Square::E4));
}

/**
 * @test Bitwise AND operator.
 * @brief Returns the intersection of two bitboards (only common bits set).
 */
TEST(BitboardTest, AndOperatorFindsIntersection) {
  Bitboard a, b;
  a.set(Square::E2);
  a.set(Square::E4);
  b.set(Square::E2);
  b.set(Square::D2);

  Bitboard c = a & b;
  EXPECT_TRUE(c.test(Square::E2));
  EXPECT_FALSE(c.test(Square::E4));
  EXPECT_FALSE(c.test(Square::D2));
}

/**
 * @test Exclusive bitwise (X)OR operator.
 * @brief Returns the bitboard with common bits set to zero.
 */
TEST(BitboardTest, ExlusiveOrOperatorDeletesCommonBits) {
  Bitboard a, b;
  a.set(Square::E2);
  a.set(Square::E4);
  b.set(Square::E2);
  b.set(Square::D2);

  Bitboard c = a ^ b;

  EXPECT_FALSE(c.test(Square::E2));
  EXPECT_TRUE(c.test(Square::E4));
  EXPECT_TRUE(c.test(Square::D2));
}

/**
 * @test Compound OR operator.
 * @brief Ensures |= adds squares.
 */
TEST(BitboardTest, CompoundOr) {
  Bitboard a, b;
  a.set(Square::A1);
  b.set(Square::B2);

  a |= b;
  EXPECT_TRUE(a.test(Square::A1));
  EXPECT_TRUE(a.test(Square::B2));
}

/**
 * @test Compound AND operator.
 * @brief Ensures &= keeps only intersections.
 */
TEST(BitboardTest, CompoundAnd) {
  Bitboard a, b;
  a.set(Square::A1);
  a.set(Square::B2);
  b.set(Square::A1);
  a &= b;

  EXPECT_TRUE(a.test(Square::A1));
  EXPECT_FALSE(a.test(Square::B2));
}

/**
 * @test Compound XOR.
 * @brief Ensures ^= removes common set bits.
 */
TEST(BitboardTest, CompoundXOr) {
  Bitboard a;
  a.set(Square::A1);
  a.set(Square::B3);

  Bitboard b;
  b.set(Square::A1);
  b ^= a;

  EXPECT_FALSE(b.test(Square::A1));
  EXPECT_TRUE(b.test(Square::B3));
}

/**
 * @test Bitwise NOT operator.
 * @brief Inverts all bits: occupied squares become empty and vice versa.
 */
TEST(BitboardTest, NotOperatorInvertsBits) {
  Bitboard a;
  a.set(Square::A1);
  Bitboard b = ~a;
  EXPECT_FALSE(b.test(Square::A1));
  EXPECT_TRUE(b.test(Square::B1));  // square that wasn’t set
}
