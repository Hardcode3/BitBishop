#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/square.hpp>

/**
 * @test Bitboard::lsb() on an empty bitboard
 * @brief Ensures lsb() returns std::nullopt when called on an empty bitboard.
 */
TEST(BitboardTest, LsbOnEmptyReturnsNullopt) {
  Bitboard bb;

  EXPECT_FALSE(bb.lsb().has_value());
}

/**
 * @test Bitboard::lsb() returns the lowest set bit
 * @brief Confirms lsb() returns the index of the lowest set bit without modifying the bitboard.
 */
TEST(BitboardTest, LsbReturnsLowestBitIndex) {
  Bitboard bb;
  bb.set(Square::C5);  // 34
  bb.set(Square::H2);  // 15
  bb.set(Square::A1);  // 0

  auto result = bb.lsb();
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->value(), Square::A1);
}

/**
 * @test Bitboard::lsb() does not modify the bitboard
 * @brief Verifies that calling lsb() does not change the state of the bitboard.
 */
TEST(BitboardTest, LsbDoesNotModifyBitboard) {
  Bitboard bb;
  bb.set(Square::C5);  // 34
  bb.set(Square::H2);  // 15
  bb.set(Square::A1);  // 0

  auto before = bb.count();
  auto _ = bb.lsb();
  EXPECT_EQ(bb.count(), before);
}
