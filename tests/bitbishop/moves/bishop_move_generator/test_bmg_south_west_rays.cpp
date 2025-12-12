#include <gtest/gtest.h>

#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/moves/bishop_move_gen.hpp>

/**
 * @test BishopMoveGenerator::south_west_ray() with no blockers
 * @brief Verifies that south_west_ray returns all squares in the SW diagonal when unobstructed.
 */
TEST(BishopMoveGeneratorTest, SouthWestRayNoBlockers) {
  Bitboard occupied;
  Square from = Squares::H8;  // Top-right corner

  Bitboard result = BishopMoveGenerator::south_west_ray(from, occupied);

  // Should include: G7, F6, E5, D4, C3, B2, A1
  EXPECT_TRUE(result.test(Square::G7));
  EXPECT_TRUE(result.test(Square::F6));
  EXPECT_TRUE(result.test(Square::E5));
  EXPECT_TRUE(result.test(Square::D4));
  EXPECT_TRUE(result.test(Square::C3));
  EXPECT_TRUE(result.test(Square::B2));
  EXPECT_TRUE(result.test(Square::A1));
  EXPECT_EQ(result.count(), 7);
}

/**
 * @test BishopMoveGenerator::south_west_ray() with a blocker
 * @brief Verifies that south_west_ray stops at the first blocker (inclusive).
 */
TEST(BishopMoveGeneratorTest, SouthWestRayWithBlocker) {
  Bitboard occupied;
  occupied.set(Square::E5);  // Blocker at E5
  Square from = Squares::H8;

  Bitboard result = BishopMoveGenerator::south_west_ray(from, occupied);

  // Should include: G7, F6, E5 (blocker), but not D4, C3, B2, A1
  EXPECT_TRUE(result.test(Square::G7));
  EXPECT_TRUE(result.test(Square::F6));
  EXPECT_TRUE(result.test(Square::E5));  // Blocker is included
  EXPECT_FALSE(result.test(Square::D4));
  EXPECT_FALSE(result.test(Square::C3));
  EXPECT_FALSE(result.test(Square::B2));
  EXPECT_FALSE(result.test(Square::A1));
  EXPECT_EQ(result.count(), 3);
}

/**
 * @test BishopMoveGenerator::south_west_ray() with immediate blocker
 * @brief Verifies that south_west_ray handles a blocker on the first square.
 */
TEST(BishopMoveGeneratorTest, SouthWestRayWithImmediateBlocker) {
  Bitboard occupied;
  occupied.set(Square::G7);  // Immediate blocker
  Square from = Squares::H8;

  Bitboard result = BishopMoveGenerator::south_west_ray(from, occupied);

  // Should only include G7
  EXPECT_TRUE(result.test(Square::G7));
  EXPECT_FALSE(result.test(Square::F6));
  EXPECT_EQ(result.count(), 1);
}

/**
 * @test BishopMoveGenerator::south_west_ray() with multiple blockers
 * @brief Verifies that each ray function only stops at the FIRST blocker, not subsequent ones.
 */
TEST(BishopMoveGeneratorTest, SouthWestRayStopAtFirstBlockerOnly) {
  Bitboard occupied;
  occupied.set(Square::C6);  // First blocker
  occupied.set(Square::F6);  // Second blocker (should be ignored)
  Square from = Squares::H8;

  Bitboard result = BishopMoveGenerator::south_west_ray(from, occupied);

  // Should include: G7, F6 (first blocker), but not E5, D4, C3, B2, A1
  EXPECT_TRUE(result.test(Square::G7));
  EXPECT_TRUE(result.test(Square::F6));
  EXPECT_FALSE(result.test(Square::E5));
  EXPECT_FALSE(result.test(Square::D4));
  EXPECT_EQ(result.count(), 2);
}

/**
 * @test BishopMoveGenerator::south_west_ray() from edge squares
 * @brief Verifies that ray functions return empty bitboards when starting from edge squares
 *        where no moves are possible in that direction.
 */
TEST(BishopMoveGeneratorTest, SouthWestRayFromEdgeSquares) {
  Bitboard occupied;

  // SW from A1 (bottom-left corner) - no squares available
  Bitboard sw_result = BishopMoveGenerator::south_west_ray(Squares::A1, occupied);
  EXPECT_EQ(sw_result.count(), 0);
}
