#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/square.hpp>

/**
 * @test Bitboard::pop_lsb() on an empty bitboard
 * @brief Ensures pop_lsb() returns std::nullopt when called on an empty bitboard.
 */
TEST(BitboardTest, PopLsbOnEmptyReturnsNullopt) {
  Bitboard bb;

  EXPECT_FALSE(bb.pop_lsb().has_value());
}

/**
 * @test Bitboard::pop_lsb() returns and clears the lowest set bit
 * @brief Confirms pop_lsb() returns the index of the lowest set bit and clears it from the bitboard.
 */
TEST(BitboardTest, PopLsbReturnsAndClearsLowestBit) {
  Bitboard bb;
  bb.set(Square::C5);  // 34
  bb.set(Square::H2);  // 15
  bb.set(Square::A1);  // 0

  EXPECT_TRUE(bb.test(Square::A1));
  auto first = bb.pop_lsb();
  ASSERT_TRUE(first.has_value());
  EXPECT_EQ(first->value(), Square::A1);
  EXPECT_FALSE(bb.test(Square::A1));

  // Check that the least significant bit has now changed to H2
  auto second = bb.lsb();
  ASSERT_TRUE(second.has_value());
  EXPECT_EQ(second->value(), Square::H2);
}

/**
 * @test Bitboard::pop_lsb() decreases the bit count
 * @brief Verifies that pop_lsb() reduces the bit count by one.
 */
TEST(BitboardTest, PopLsbDecreasesCount) {
  Bitboard bb;
  bb.set(Square::A1);  // 0
  bb.set(Square::C5);  // 34

  int before = bb.count();
  bb.pop_lsb();
  EXPECT_EQ(bb.count(), before - 1);
}

/**
 * @test Bitboard::pop_lsb() eventually empties the bitboard
 * @brief Ensures that repeated calls to pop_lsb() on a bitboard with two set bits
 *        will return each bit in LSB order and eventually return std::nullopt
 *        when the bitboard is empty.
 */
TEST(BitboardTest, PopLsbEventuallyEmptiesBoard) {
  Bitboard bb;
  bb.set(Square::H2);  // 15
  bb.set(Square::A1);  // 0

  auto first = bb.pop_lsb();
  auto second = bb.pop_lsb();
  auto third = bb.pop_lsb();

  EXPECT_TRUE(first.has_value());
  EXPECT_TRUE(second.has_value());
  EXPECT_FALSE(third.has_value());
}
