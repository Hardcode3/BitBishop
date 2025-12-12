#include <gtest/gtest.h>

#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/moves/bishop_move_gen.hpp>

/**
 * @test BishopMoveGenerator::north_west_ray() with no blockers
 * @brief Verifies that north_west_ray returns all squares in the NW diagonal when unobstructed.
 */
TEST(BishopMoveGeneratorTest, NorthWestRayNoBlockers) {
  Bitboard occupied;
  Square from = Squares::H1;  // Bottom-right corner

  Bitboard result = BishopMoveGenerator::north_west_ray(from, occupied);

  // Should include: G2, F3, E4, D5, C6, B7, A8
  EXPECT_TRUE(result.test(Square::G2));
  EXPECT_TRUE(result.test(Square::F3));
  EXPECT_TRUE(result.test(Square::E4));
  EXPECT_TRUE(result.test(Square::D5));
  EXPECT_TRUE(result.test(Square::C6));
  EXPECT_TRUE(result.test(Square::B7));
  EXPECT_TRUE(result.test(Square::A8));
  EXPECT_EQ(result.count(), 7);
}

/**
 * @test BishopMoveGenerator::north_west_ray() with a blocker
 * @brief Verifies that north_west_ray stops at the first blocker (inclusive).
 */
TEST(BishopMoveGeneratorTest, NorthWestRayWithBlocker) {
  Bitboard occupied;
  occupied.set(Square::E4);  // Blocker at E4
  Square from = Squares::H1;

  Bitboard result = BishopMoveGenerator::north_west_ray(from, occupied);

  // Should include: G2, F3, E4 (blocker), but not D5, C6, B7, A8
  EXPECT_TRUE(result.test(Square::G2));
  EXPECT_TRUE(result.test(Square::F3));
  EXPECT_TRUE(result.test(Square::E4));  // Blocker is included
  EXPECT_FALSE(result.test(Square::D5));
  EXPECT_FALSE(result.test(Square::C6));
  EXPECT_FALSE(result.test(Square::B7));
  EXPECT_FALSE(result.test(Square::A8));
  EXPECT_EQ(result.count(), 3);
}

/**
 * @test BishopMoveGenerator::north_west_ray() with immediate blocker
 * @brief Verifies that north_west_ray handles a blocker on the first square.
 */
TEST(BishopMoveGeneratorTest, NorthWestRayWithImmediateBlocker) {
  Bitboard occupied;
  occupied.set(Square::G2);  // Immediate blocker
  Square from = Squares::H1;

  Bitboard result = BishopMoveGenerator::north_west_ray(from, occupied);

  // Should only include G2
  EXPECT_TRUE(result.test(Square::G2));
  EXPECT_FALSE(result.test(Square::F3));
  EXPECT_EQ(result.count(), 1);
}

/**
 * @test BishopMoveGenerator::north_west_ray() with multiple blockers
 * @brief Verifies that each ray function only stops at the FIRST blocker, not subsequent ones.
 */
TEST(BishopMoveGeneratorTest, NorthWestRayStopAtFirstBlockerOnly) {
  Bitboard occupied;
  occupied.set(Square::F3);  // First blocker
  occupied.set(Square::D5);  // Second blocker (should be ignored)
  Square from = Squares::H1;

  Bitboard result = BishopMoveGenerator::north_west_ray(from, occupied);

  // Should include: G2, F3 (first blocker), but not E4, D5, C6, B7, A8
  EXPECT_TRUE(result.test(Square::G2));
  EXPECT_TRUE(result.test(Square::F3));
  EXPECT_FALSE(result.test(Square::E4));
  EXPECT_FALSE(result.test(Square::D5));
  EXPECT_EQ(result.count(), 2);
}

/**
 * @test BishopMoveGenerator::north_west_ray() from edge squares
 * @brief Verifies that ray functions return empty bitboards when starting from edge squares
 *        where no moves are possible in that direction.
 */
TEST(BishopMoveGeneratorTest, NorthWestRayFromEdgeSquares) {
  Bitboard occupied;

  // NW from A8 (top-left corner) - no squares available
  Bitboard nw_result = BishopMoveGenerator::north_west_ray(Squares::A8, occupied);
  EXPECT_EQ(nw_result.count(), 0);
}
