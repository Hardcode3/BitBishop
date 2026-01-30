#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/square.hpp>

/**
 * @test Set and test a square.
 * @brief After setting E2, it should be occupied, and E4 should remain empty.
 */
TEST(BitboardTest, SetAndTestSquareFromSquareValue) {
  Bitboard bb;
  bb.set(Square::E2);
  EXPECT_TRUE(bb.test(Square::E2));
  EXPECT_FALSE(bb.test(Square::E4));
}

/**
 * @test Set and test a square.
 * @brief After setting E2, it should be occupied, and E4 should remain empty.
 */
TEST(BitboardTest, SetAndTestSquareFromSquare) {
  Bitboard bb;

  Square e2(Square::E2);
  Square e4(Square::E4);

  bb.set(e2);
  EXPECT_TRUE(bb.test(e2));
  EXPECT_FALSE(bb.test(e4));
}

/**
 * @test Clear a square.
 * @brief After clearing E2, the bitboard should show it as empty.
 */
TEST(BitboardTest, ClearSquareFromSquareValue) {
  Bitboard bb;
  bb.set(Square::E2);
  EXPECT_TRUE(bb.test(Square::E2));
  bb.clear(Square::E2);
  EXPECT_FALSE(bb.test(Square::E2));
}

/**
 * @test Clear a square.
 * @brief After clearing E2, the bitboard should show it as empty.
 */
TEST(BitboardTest, ClearSquareFromSquare) {
  Bitboard bb;

  Square e2(Square::E2);

  bb.set(e2);
  EXPECT_TRUE(bb.test(e2));
  bb.clear(e2);
  EXPECT_FALSE(bb.test(e2));
}

TEST(BitboardTest, ClearSquareFromSquareUint8t) {
  Bitboard bb;

  Square e2(Square::E2);
  const std::uint8_t e2_uint8_t = static_cast<std::uint8_t>(e2.value());

  bb.set(e2_uint8_t);
  EXPECT_TRUE(bb.test(e2_uint8_t));
  bb.clear(e2_uint8_t);
  EXPECT_FALSE(bb.test(e2_uint8_t));
}

/**
 * @test Reset the bitboard.
 * @brief Ensures all bits are cleared and value() = 0.
 */
TEST(BitboardTest, ResetClearsAllSquares) {
  Bitboard bb;
  bb.set(Square::A1);
  bb.set(Square::H8);
  bb.reset();
  for (int sq = 0; sq < 64; ++sq) {
    Square square(sq);
    EXPECT_FALSE(bb.test(square));
  }
  EXPECT_EQ(bb.value(), 0ULL);
}
