#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/square.hpp>

/**
 * @test Bitboard::pop_msb() on an empty bitboard
 * @brief Ensures pop_msb() returns std::nullopt when called on an empty bitboard.
 */
TEST(BitboardTest, PopMsbOnEmptyReturnsNullopt) {
  Bitboard bb;

  EXPECT_FALSE(bb.pop_msb().has_value());
}

/**
 * @test Bitboard::pop_msb() returns and clears the highest set bit
 * @brief Confirms pop_msb() returns the index of the highest set bit and clears it from the bitboard.
 */
TEST(BitboardTest, PopMsbReturnsAndClearsHighestBit) {
  Bitboard bb;
  bb.set(Square::A1);  // 0
  bb.set(Square::H2);  // 15
  bb.set(Square::C5);  // 34

  EXPECT_TRUE(bb.test(Square::C5));
  auto first = bb.pop_msb();
  ASSERT_TRUE(first.has_value());
  EXPECT_EQ(first->value(), Square::C5);
  EXPECT_FALSE(bb.test(Square::C5));

  // Check that the most significant bit has now changed to H2
  auto second = bb.msb();
  ASSERT_TRUE(second.has_value());
  EXPECT_EQ(second->value(), Square::H2);
}

/**
 * @test Bitboard::pop_msb() decreases the bit count
 * @brief Verifies that pop_msb() reduces the bit count by one.
 */
TEST(BitboardTest, PopMsbDecreasesCount) {
  Bitboard bb;
  bb.set(Square::A1);  // 0
  bb.set(Square::C5);  // 34

  int before = bb.count();
  bb.pop_msb();
  EXPECT_EQ(bb.count(), before - 1);
}

/**
 * @test Bitboard::pop_msb() eventually empties the bitboard
 * @brief Ensures that repeated calls to pop_msb() on a bitboard with two set bits
 *        will return each bit in MSB order and eventually return std::nullopt
 *        when the bitboard is empty.
 */
TEST(BitboardTest, PopMsbEventuallyEmptiesBoard) {
  Bitboard bb;
  bb.set(Square::A1);  // 0
  bb.set(Square::H2);  // 15

  auto first = bb.pop_msb();
  auto second = bb.pop_msb();
  auto third = bb.pop_msb();

  EXPECT_TRUE(first.has_value());
  EXPECT_TRUE(second.has_value());
  EXPECT_FALSE(third.has_value());
}

/**
 * @test Bitboard::pop_msb() and pop_lsb() work in opposite order
 * @brief Verifies that pop_msb() returns bits from highest to lowest,
 *        while pop_lsb() returns bits from lowest to highest.
 */
TEST(BitboardTest, PopMsbAndPopLsbWorkInOppositeOrder) {
  Bitboard bb1;
  bb1.set(Square::A1);  // 0
  bb1.set(Square::D4);  // 27
  bb1.set(Square::H8);  // 63

  Bitboard bb2 = bb1;

  // pop_lsb should return: A1 (0), D4 (27), H8 (63)
  auto lsb1 = bb1.pop_lsb();
  auto lsb2 = bb1.pop_lsb();
  auto lsb3 = bb1.pop_lsb();

  ASSERT_TRUE(lsb1.has_value());
  ASSERT_TRUE(lsb2.has_value());
  ASSERT_TRUE(lsb3.has_value());
  EXPECT_EQ(lsb1->value(), Square::A1);
  EXPECT_EQ(lsb2->value(), Square::D4);
  EXPECT_EQ(lsb3->value(), Square::H8);

  // pop_msb should return: H8 (63), D4 (27), A1 (0)
  auto msb1 = bb2.pop_msb();
  auto msb2 = bb2.pop_msb();
  auto msb3 = bb2.pop_msb();

  ASSERT_TRUE(msb1.has_value());
  ASSERT_TRUE(msb2.has_value());
  ASSERT_TRUE(msb3.has_value());
  EXPECT_EQ(msb1->value(), Square::H8);
  EXPECT_EQ(msb2->value(), Square::D4);
  EXPECT_EQ(msb3->value(), Square::A1);
}
