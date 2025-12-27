#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/lookups/between_squares.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Lookups;

/**
 * @test Horizontal direction on same rank (step = +-1).
 * @brief Confirms direction() returns +1 for rightward and -1 for leftward movement.
 */
TEST(LookupsTest, DirectionSameRank) {
  EXPECT_EQ(direction(A1, H1), +1);
  EXPECT_EQ(direction(H1, A1), -1);
  EXPECT_EQ(direction(D4, G4), +1);
  EXPECT_EQ(direction(E5, B5), -1);
}

/**
 * @test Vertical direction on same file (step = +-8).
 * @brief Confirms direction() returns +8 for upward and -8 for downward movement.
 */
TEST(LookupsTest, DirectionSameFile) {
  EXPECT_EQ(direction(A1, A8), +8);
  EXPECT_EQ(direction(A8, A1), -8);
  EXPECT_EQ(direction(E2, E6), +8);
  EXPECT_EQ(direction(H7, H3), -8);
}

/**
 * @test NE-SW diagonal direction (step = +-9).
 * @brief Confirms direction() returns +9 for NE and -9 for SW movement.
 */
TEST(LookupsTest, DirectionNESWDiagonal) {
  EXPECT_EQ(direction(A1, H8), +9);
  EXPECT_EQ(direction(H8, A1), -9);
  EXPECT_EQ(direction(C3, E5), +9);
  EXPECT_EQ(direction(F6, D4), -9);
}

/**
 * @test NW-SE diagonal direction (step = +-7).
 * @brief Confirms direction() returns +7 for NW and -7 for SE movement.
 */
TEST(LookupsTest, DirectionNWSEDiagonal) {
  EXPECT_EQ(direction(A8, H1), -7);
  EXPECT_EQ(direction(H1, A8), +7);
  EXPECT_EQ(direction(B6, E3), -7);
  EXPECT_EQ(direction(F4, C7), +7);
}

/**
 * @test Empty ray for identical squares.
 * @brief Confirms ray_between() returns empty bitboard when from == to.
 */
TEST(LookupsTest, RayBetweenIdenticalSquares) {
  EXPECT_EQ(ray_between(E4, E4), Bitboard::Zeros());
  EXPECT_EQ(ray_between(A1, A1), Bitboard::Zeros());
  EXPECT_EQ(ray_between(H8, H8), Bitboard::Zeros());
}

/**
 * @test Empty ray for non-aligned squares.
 * @brief Confirms ray_between() returns empty bitboard for unaligned squares.
 */
TEST(LookupsTest, RayBetweenNonAligned) {
  EXPECT_EQ(ray_between(A1, B3), Bitboard::Zeros());
  EXPECT_EQ(ray_between(E4, F6), Bitboard::Zeros());
  EXPECT_EQ(ray_between(C2, E3), Bitboard::Zeros());
}

/**
 * @test Ray between adjacent squares on same rank.
 * @brief Confirms ray_between() returns empty bitboard for adjacent squares.
 */
TEST(LookupsTest, RayBetweenAdjacentRank) {
  EXPECT_EQ(ray_between(D4, E4), Bitboard::Zeros());
  EXPECT_EQ(ray_between(A1, B1), Bitboard::Zeros());
  EXPECT_EQ(ray_between(G8, H8), Bitboard::Zeros());
}

/**
 * @test Ray between adjacent squares on same file.
 * @brief Confirms ray_between() returns empty bitboard for adjacent squares.
 */
TEST(LookupsTest, RayBetweenAdjacentFile) {
  EXPECT_EQ(ray_between(D4, D5), Bitboard::Zeros());
  EXPECT_EQ(ray_between(A1, A2), Bitboard::Zeros());
  EXPECT_EQ(ray_between(H7, H8), Bitboard::Zeros());
}

/**
 * @test Ray between squares on same rank.
 * @brief Confirms ray_between() contains all intermediate squares horizontally.
 */
TEST(LookupsTest, RayBetweenSameRank) {
  Bitboard ray = ray_between(A1, E1);
  EXPECT_TRUE(ray.test(B1));
  EXPECT_TRUE(ray.test(C1));
  EXPECT_TRUE(ray.test(D1));
  EXPECT_FALSE(ray.test(A1));
  EXPECT_FALSE(ray.test(E1));

  ray = ray_between(B4, G4);
  EXPECT_EQ(ray.count(), 4);
  EXPECT_TRUE(ray.test(C4));
  EXPECT_TRUE(ray.test(D4));
  EXPECT_TRUE(ray.test(E4));
  EXPECT_TRUE(ray.test(F4));
}

/**
 * @test Ray between squares on same file.
 * @brief Confirms ray_between() contains all intermediate squares vertically.
 */
TEST(LookupsTest, RayBetweenSameFile) {
  Bitboard ray = ray_between(D1, D5);
  EXPECT_TRUE(ray.test(D2));
  EXPECT_TRUE(ray.test(D3));
  EXPECT_TRUE(ray.test(D4));
  EXPECT_FALSE(ray.test(D1));
  EXPECT_FALSE(ray.test(D5));

  ray = ray_between(A1, A8);
  EXPECT_EQ(ray.count(), 6);
  EXPECT_FALSE(ray.test(A1));
  EXPECT_FALSE(ray.test(A8));
}

/**
 * @test Ray between squares on NE-SW diagonal.
 * @brief Confirms ray_between() contains all intermediate diagonal squares.
 */
TEST(LookupsTest, RayBetweenNESWDiagonal) {
  Bitboard ray = ray_between(A1, D4);
  EXPECT_TRUE(ray.test(B2));
  EXPECT_TRUE(ray.test(C3));
  EXPECT_FALSE(ray.test(A1));
  EXPECT_FALSE(ray.test(D4));

  ray = ray_between(B2, F6);
  EXPECT_EQ(ray.count(), 3);
  EXPECT_TRUE(ray.test(C3));
  EXPECT_TRUE(ray.test(D4));
  EXPECT_TRUE(ray.test(E5));
}

/**
 * @test Ray between squares on NW-SE diagonal.
 * @brief Confirms ray_between() contains all intermediate diagonal squares.
 */
TEST(LookupsTest, RayBetweenNWSEDiagonal) {
  Bitboard ray = ray_between(A8, D5);
  EXPECT_TRUE(ray.test(B7));
  EXPECT_TRUE(ray.test(C6));
  EXPECT_FALSE(ray.test(A8));
  EXPECT_FALSE(ray.test(D5));

  ray = ray_between(B6, E3);
  EXPECT_EQ(ray.count(), 2);
  EXPECT_TRUE(ray.test(C5));
  EXPECT_TRUE(ray.test(D4));
}

/**
 * @test Ray symmetry property.
 * @brief Confirms ray_between(a, b) == ray_between(b, a).
 */
TEST(LookupsTest, RayBetweenSymmetry) {
  EXPECT_EQ(ray_between(A1, H1), ray_between(H1, A1));
  EXPECT_EQ(ray_between(D1, D8), ray_between(D8, D1));
  EXPECT_EQ(ray_between(A1, H8), ray_between(H8, A1));
  EXPECT_EQ(ray_between(A8, H1), ray_between(H1, A8));
}

/**
 * @test BETWEEN table for same rank lookups.
 * @brief Confirms BETWEEN[from][to] contains correct horizontal rays.
 */
TEST(LookupsTest, BetweenTableSameRank) {
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
TEST(LookupsTest, BetweenTableSameFile) {
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
TEST(LookupsTest, BetweenTableDiagonal) {
  const Bitboard& ray = BETWEEN[A1.value()][D4.value()];
  EXPECT_TRUE(ray.test(B2));
  EXPECT_TRUE(ray.test(C3));
  EXPECT_EQ(ray.count(), 2);
}

/**
 * @test BETWEEN table for non-aligned squares.
 * @brief Confirms BETWEEN[from][to] is empty for unaligned squares.
 */
TEST(LookupsTest, BetweenTableNonAligned) {
  EXPECT_EQ(BETWEEN[A1.value()][B3.value()], Bitboard::Zeros());
  EXPECT_EQ(BETWEEN[E4.value()][F6.value()], Bitboard::Zeros());
}

/**
 * @test BETWEEN table symmetry.
 * @brief Confirms BETWEEN[a][b] == BETWEEN[b][a].
 */
TEST(LookupsTest, BetweenTableSymmetry) {
  EXPECT_EQ(BETWEEN[A1.value()][H1.value()], BETWEEN[H1.value()][A1.value()]);
  EXPECT_EQ(BETWEEN[A1.value()][H8.value()], BETWEEN[H8.value()][A1.value()]);
}

/**
 * @test BETWEEN table diagonal property.
 * @brief Confirms BETWEEN[x][x] is empty for all squares.
 */
TEST(LookupsTest, BetweenTableDiagonalSameSquare) {
  for (int i = 0; i < 64; ++i) {
    EXPECT_EQ(BETWEEN[i][i], Bitboard::Zeros());
  }
}

/**
 * @test Full board corner-to-corner rays.
 * @brief Confirms longest rays span the entire board correctly.
 */
TEST(LookupsTest, RayBetweenFullBoard) {
  EXPECT_EQ(ray_between(A1, H1).count(), 6);
  EXPECT_EQ(ray_between(A1, A8).count(), 6);
  EXPECT_EQ(ray_between(A1, H8).count(), 6);
  EXPECT_EQ(ray_between(A8, H1).count(), 6);
}
