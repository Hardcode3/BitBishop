#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/square.hpp>

/**
 * @test Count operator for bits set to one
 * @brief Confirms count works as intended on an empty bitboard.
 */
TEST(BitboardTest, CountEmptyBitboardIsZero) {
  Bitboard bb;

  EXPECT_EQ(bb.count(), 0);
}

/**
 * @test Count operator for bits set to one
 * @brief Confirms count works as intended on a bitboard populated with one element.
 */
TEST(BitboardTest, CountSingleBitIsOne) {
  Bitboard bb;

  bb.set(Square::C5);

  EXPECT_EQ(bb.count(), 1);
}

/**
 * @test Bitboard::count() with multiple bits set
 * @brief Verifies that count() returns the correct number of set bits (3) on a bitboard with multiple elements.
 */
TEST(BitboardTest, CountMultipleBits) {
  Bitboard bb;
  bb.set(Square::C5);
  bb.set(Square::H2);
  bb.set(Square::A1);

  EXPECT_EQ(bb.count(), 3);
}
