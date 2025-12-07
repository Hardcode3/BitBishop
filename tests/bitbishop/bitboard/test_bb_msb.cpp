#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/square.hpp>

/**
 * @test Bitboard::msb() on an empty bitboard
 * @brief Ensures msb() returns std::nullopt when called on an empty bitboard.
 */
TEST(BitboardTest, MsbOnEmptyReturnsNullopt) {
  Bitboard bb;

  EXPECT_FALSE(bb.msb().has_value());
}

/**
 * @test Bitboard::msb() returns the highest set bit without modifying the bitboard
 * @brief Confirms msb() returns the highest set bit but does not clear it.
 */
TEST(BitboardTest, MsbReturnsHighestBitWithoutModifying) {
  Bitboard bb;
  bb.set(Square::A1);  // 0
  bb.set(Square::H2);  // 15
  bb.set(Square::C5);  // 34

  auto highest = bb.msb();
  ASSERT_TRUE(highest.has_value());
  EXPECT_EQ(highest->value(), Square::C5);

  // Verify the bit is still set
  EXPECT_TRUE(bb.test(Square::C5));
  EXPECT_EQ(bb.count(), 3);

  // Calling msb() again should return the same result
  auto again = bb.msb();
  ASSERT_TRUE(again.has_value());
  EXPECT_EQ(again->value(), Square::C5);
}

/**
 * @test Bitboard::msb() on a single bit
 * @brief Verifies msb() correctly identifies the only set bit.
 */
TEST(BitboardTest, MsbOnSingleBit) {
  Bitboard bb;
  bb.set(Square::E4);  // 28

  auto result = bb.msb();
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->value(), Square::E4);
  EXPECT_EQ(bb.count(), 1);
}

/**
 * @test Bitboard::msb() and lsb() on the same bitboard
 * @brief Ensures msb() and lsb() return different values when multiple bits are set.
 */
TEST(BitboardTest, MsbAndLsbReturnDifferentBits) {
  Bitboard bb;
  bb.set(Square::A1);  // 0
  bb.set(Square::H8);  // 63

  auto lowest = bb.lsb();
  auto highest = bb.msb();

  ASSERT_TRUE(lowest.has_value());
  ASSERT_TRUE(highest.has_value());
  EXPECT_EQ(lowest->value(), Square::A1);
  EXPECT_EQ(highest->value(), Square::H8);
  EXPECT_NE(lowest->value(), highest->value());
}

/**
 * @test Bitboard::msb() and lsb() return the same value on a single bit
 * @brief When only one bit is set, msb() and lsb() should return the same square.
 */
TEST(BitboardTest, MsbAndLsbReturnSameOnSingleBit) {
  Bitboard bb;
  bb.set(Square::D4);  // 27

  auto lowest = bb.lsb();
  auto highest = bb.msb();

  ASSERT_TRUE(lowest.has_value());
  ASSERT_TRUE(highest.has_value());
  EXPECT_EQ(lowest->value(), highest->value());
  EXPECT_EQ(lowest->value(), Square::D4);
}
