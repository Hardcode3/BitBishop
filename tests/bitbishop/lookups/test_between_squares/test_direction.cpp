#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/lookups/between_squares.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Lookups;

/**
 * @test Seeking for the direction using identical source and destination squares should return .
 */
TEST(DirectionTest, DirectionSameSquare) {
  EXPECT_EQ(direction(E4, E4), 0);
  EXPECT_EQ(direction(A1, A1), 0);
  EXPECT_EQ(direction(H8, H8), 0);
}

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

TEST(LookupsTest, DirectionWithInvalidFromAndToSquares) {
  EXPECT_EQ(direction(A1, B6), 0);
  EXPECT_EQ(direction(F4, H8), 0);
}
