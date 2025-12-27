#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/lookups/between_squares.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Lookups;

/**
 * @test BETWEEN table for same rank lookups.
 * @brief Confirms BETWEEN[from][to] contains correct horizontal rays.
 */
TEST(BetweenLookupTest, BetweenTableSameRank) {
  const Bitboard& ray = BETWEEN[A1.value()][E1.value()];
  EXPECT_TRUE(ray.test(B1));
  EXPECT_TRUE(ray.test(C1));
  EXPECT_TRUE(ray.test(D1));
  EXPECT_FALSE(ray.test(A1));
  EXPECT_FALSE(ray.test(E1));
}

/**
 * @test BETWEEN table for same file lookups.
 * @brief Confirms BETWEEN[from][to] contains correct vertical rays.
 */
TEST(BetweenLookupTest, BetweenTableSameFile) {
  const Bitboard& ray = BETWEEN[D1.value()][D5.value()];
  EXPECT_TRUE(ray.test(D2));
  EXPECT_TRUE(ray.test(D3));
  EXPECT_TRUE(ray.test(D4));
  EXPECT_FALSE(ray.test(D1));
  EXPECT_FALSE(ray.test(D5));
}

/**
 * @test BETWEEN table for diagonal lookups.
 * @brief Confirms BETWEEN[from][to] contains correct diagonal rays.
 */
TEST(BetweenLookupTest, BetweenTableDiagonal) {
  const Bitboard& ray = BETWEEN[A1.value()][D4.value()];
  EXPECT_TRUE(ray.test(B2));
  EXPECT_TRUE(ray.test(C3));
  EXPECT_EQ(ray.count(), 2);
}

/**
 * @test BETWEEN table for non-aligned squares.
 * @brief Confirms BETWEEN[from][to] is empty for unaligned squares.
 */
TEST(BetweenLookupTest, BetweenTableNonAligned) {
  EXPECT_EQ(BETWEEN[A1.value()][B3.value()], Bitboard::Zeros());
  EXPECT_EQ(BETWEEN[E4.value()][F6.value()], Bitboard::Zeros());
}

/**
 * @test BETWEEN table symmetry.
 * @brief Confirms BETWEEN[a][b] == BETWEEN[b][a].
 */
TEST(BetweenLookupTest, BetweenTableSymmetry) {
  EXPECT_EQ(BETWEEN[A1.value()][H1.value()], BETWEEN[H1.value()][A1.value()]);
  EXPECT_EQ(BETWEEN[A1.value()][H8.value()], BETWEEN[H8.value()][A1.value()]);
}

/**
 * @test BETWEEN table diagonal property.
 * @brief Confirms BETWEEN[x][x] is empty for all squares.
 */
TEST(BetweenLookupTest, BetweenTableDiagonalSameSquare) {
  for (int i = 0; i < 64; ++i) {
    EXPECT_EQ(BETWEEN[i][i], Bitboard::Zeros());
  }
}
