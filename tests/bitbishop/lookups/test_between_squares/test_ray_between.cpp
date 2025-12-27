#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/lookups/between_squares.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Lookups;

/**
 * @test Empty ray for identical squares.
 * @brief Confirms ray_between() returns empty bitboard when from == to.
 */
TEST(RayBetweenTest, RayBetweenIdenticalSquares) {
  EXPECT_EQ(ray_between(E4, E4), Bitboard::Zeros());
  EXPECT_EQ(ray_between(A1, A1), Bitboard::Zeros());
  EXPECT_EQ(ray_between(H8, H8), Bitboard::Zeros());
}

/**
 * @test Empty ray for non-aligned squares.
 * @brief Confirms ray_between() returns empty bitboard for unaligned squares.
 */
TEST(RayBetweenTest, RayBetweenNonAligned) {
  EXPECT_EQ(ray_between(A1, B3), Bitboard::Zeros());
  EXPECT_EQ(ray_between(E4, F6), Bitboard::Zeros());
  EXPECT_EQ(ray_between(C2, E3), Bitboard::Zeros());
}

/**
 * @test Ray between adjacent squares on same rank.
 * @brief Confirms ray_between() returns empty bitboard for adjacent squares.
 */
TEST(RayBetweenTest, RayBetweenAdjacentRank) {
  EXPECT_EQ(ray_between(D4, E4), Bitboard::Zeros());
  EXPECT_EQ(ray_between(A1, B1), Bitboard::Zeros());
  EXPECT_EQ(ray_between(G8, H8), Bitboard::Zeros());
}

/**
 * @test Ray between adjacent squares on same file.
 * @brief Confirms ray_between() returns empty bitboard for adjacent squares.
 */
TEST(RayBetweenTest, RayBetweenAdjacentFile) {
  EXPECT_EQ(ray_between(D4, D5), Bitboard::Zeros());
  EXPECT_EQ(ray_between(A1, A2), Bitboard::Zeros());
  EXPECT_EQ(ray_between(H7, H8), Bitboard::Zeros());
}

/**
 * @test Ray between squares on same rank.
 * @brief Confirms ray_between() contains all intermediate squares horizontally.
 */
TEST(RayBetweenTest, RayBetweenSameRank) {
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
TEST(RayBetweenTest, RayBetweenSameFile) {
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
TEST(RayBetweenTest, RayBetweenNESWDiagonal) {
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
TEST(RayBetweenTest, RayBetweenNWSEDiagonal) {
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
TEST(RayBetweenTest, RayBetweenSymmetry) {
  EXPECT_EQ(ray_between(A1, H1), ray_between(H1, A1));
  EXPECT_EQ(ray_between(D1, D8), ray_between(D8, D1));
  EXPECT_EQ(ray_between(A1, H8), ray_between(H8, A1));
  EXPECT_EQ(ray_between(A8, H1), ray_between(H1, A8));
}

/**
 * @test Full board corner-to-corner rays.
 * @brief Confirms longest rays span the entire board correctly.
 */
TEST(RayBetweenTest, RayBetweenFullBoard) {
  EXPECT_EQ(ray_between(A1, H1).count(), 6);
  EXPECT_EQ(ray_between(A1, A8).count(), 6);
  EXPECT_EQ(ray_between(A1, H8).count(), 6);
  EXPECT_EQ(ray_between(A8, H1).count(), 6);
}
