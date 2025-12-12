#include <gtest/gtest.h>

#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/moves/bishop_move_gen.hpp>

/**
 * @test BishopMoveGenerator::north_east_ray() with no blockers
 * @brief Verifies that north_east_ray returns all squares in the NE diagonal when unobstructed.
 */
TEST(BishopMoveGeneratorTest, NorthEastRayNoBlockers) {
  Bitboard occupied;
  Square from = Squares::A1;  // Bottom-left corner

  Bitboard result = BishopMoveGenerator::north_east_ray(from, occupied);

  // Should include: B2, C3, D4, E5, F6, G7, H8
  EXPECT_TRUE(result.test(Square::B2));
  EXPECT_TRUE(result.test(Square::C3));
  EXPECT_TRUE(result.test(Square::D4));
  EXPECT_TRUE(result.test(Square::E5));
  EXPECT_TRUE(result.test(Square::F6));
  EXPECT_TRUE(result.test(Square::G7));
  EXPECT_TRUE(result.test(Square::H8));
  EXPECT_EQ(result.count(), 7);
}

/**
 * @test BishopMoveGenerator::north_east_ray() with a blocker
 * @brief Verifies that north_east_ray stops at the first blocker (inclusive).
 */
TEST(BishopMoveGeneratorTest, NorthEastRayWithBlocker) {
  Bitboard occupied;
  occupied.set(Square::D4);  // Blocker at D4
  Square from = Squares::A1;

  Bitboard result = BishopMoveGenerator::north_east_ray(from, occupied);

  // Should include: B2, C3, D4 (blocker), but not E5, F6, G7, H8
  EXPECT_TRUE(result.test(Square::B2));
  EXPECT_TRUE(result.test(Square::C3));
  EXPECT_TRUE(result.test(Square::D4));  // Blocker is included
  EXPECT_FALSE(result.test(Square::E5));
  EXPECT_FALSE(result.test(Square::F6));
  EXPECT_FALSE(result.test(Square::G7));
  EXPECT_FALSE(result.test(Square::H8));
  EXPECT_EQ(result.count(), 3);
}

/**
 * @test BishopMoveGenerator::north_east_ray() with immediate blocker
 * @brief Verifies that north_east_ray handles a blocker on the first square.
 */
TEST(BishopMoveGeneratorTest, NorthEastRayWithImmediateBlocker) {
  Bitboard occupied;
  occupied.set(Square::B2);  // Immediate blocker
  Square from = Squares::A1;

  Bitboard result = BishopMoveGenerator::north_east_ray(from, occupied);

  // Should only include B2
  EXPECT_TRUE(result.test(Square::B2));
  EXPECT_FALSE(result.test(Square::C3));
  EXPECT_EQ(result.count(), 1);
}

/**
 * @test BishopMoveGenerator::north_east_ray() from center of board
 * @brief Verifies north_east_ray works correctly from a central square.
 */
TEST(BishopMoveGeneratorTest, NorthEastRayFromCenter) {
  Bitboard occupied;
  Square from = Squares::D4;

  Bitboard result = BishopMoveGenerator::north_east_ray(from, occupied);

  // Should include: E5, F6, G7, H8
  EXPECT_TRUE(result.test(Square::E5));
  EXPECT_TRUE(result.test(Square::F6));
  EXPECT_TRUE(result.test(Square::G7));
  EXPECT_TRUE(result.test(Square::H8));
  EXPECT_EQ(result.count(), 4);
}

/**
 * @test BishopMoveGenerator::north_east_ray() with multiple blockers
 * @brief Verifies that each ray function only stops at the FIRST blocker, not subsequent ones.
 */
TEST(BishopMoveGeneratorTest, NorthEastRayStopAtFirstBlockerOnly) {
  Bitboard occupied;
  occupied.set(Square::C3);  // First blocker
  occupied.set(Square::E5);  // Second blocker (should be ignored)
  Square from = Squares::A1;

  Bitboard result = BishopMoveGenerator::north_east_ray(from, occupied);

  // Should include: B2, C3 (first blocker), but not D4, E5, F6, G7, H8
  EXPECT_TRUE(result.test(Square::B2));
  EXPECT_TRUE(result.test(Square::C3));
  EXPECT_FALSE(result.test(Square::D4));
  EXPECT_FALSE(result.test(Square::E5));
  EXPECT_EQ(result.count(), 2);
}

/**
 * @test BishopMoveGenerator::north_east_ray() from edge squares
 * @brief Verifies that ray functions return empty bitboards when starting from edge squares
 *        where no moves are possible in that direction.
 */
TEST(BishopMoveGeneratorTest, NorthEastRayFromEdgeSquares) {
  Bitboard occupied;

  // NE from H8 (top-right corner) - no squares available
  Bitboard ne_result = BishopMoveGenerator::north_east_ray(Squares::H8, occupied);
  EXPECT_EQ(ne_result.count(), 0);
}
