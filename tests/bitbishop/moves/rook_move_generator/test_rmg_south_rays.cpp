#include <gtest/gtest.h>

#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/moves/rook_move_gen.hpp>

/**
 * @test RookMoveGenerator::south_ray() with no blockers
 * @brief Verifies that south_ray returns all squares to the south when unobstructed.
 */
TEST(RookMoveGeneratorTest, SouthRayNoBlockers) {
  Bitboard occupied;
  Square from = Squares::D8;  // Top of board, file D
  Bitboard result = RookMoveGenerator::south_ray(from, occupied);

  // Should include: D7, D6, D5, D4, D3, D2, D1
  EXPECT_TRUE(result.test(Square::D7));
  EXPECT_TRUE(result.test(Square::D6));
  EXPECT_TRUE(result.test(Square::D5));
  EXPECT_TRUE(result.test(Square::D4));
  EXPECT_TRUE(result.test(Square::D3));
  EXPECT_TRUE(result.test(Square::D2));
  EXPECT_TRUE(result.test(Square::D1));
  EXPECT_EQ(result.count(), 7);
}

/**
 * @test RookMoveGenerator::south_ray() with a blocker
 * @brief Verifies that south_ray stops at the first blocker (inclusive).
 */
TEST(RookMoveGeneratorTest, SouthRayWithBlocker) {
  Bitboard occupied;
  occupied.set(Square::D4);  // Blocker at D4
  Square from = Squares::D8;
  Bitboard result = RookMoveGenerator::south_ray(from, occupied);

  // Should include: D7, D6, D5, D4 (blocker), but not D3, D2, D1
  EXPECT_TRUE(result.test(Square::D7));
  EXPECT_TRUE(result.test(Square::D6));
  EXPECT_TRUE(result.test(Square::D5));
  EXPECT_TRUE(result.test(Square::D4));  // Blocker is included
  EXPECT_FALSE(result.test(Square::D3));
  EXPECT_FALSE(result.test(Square::D2));
  EXPECT_FALSE(result.test(Square::D1));
  EXPECT_EQ(result.count(), 4);
}

/**
 * @test RookMoveGenerator::south_ray() with immediate blocker
 * @brief Verifies that south_ray handles a blocker on the first square.
 */
TEST(RookMoveGeneratorTest, SouthRayWithImmediateBlocker) {
  Bitboard occupied;
  occupied.set(Square::D7);  // Immediate blocker
  Square from = Squares::D8;
  Bitboard result = RookMoveGenerator::south_ray(from, occupied);

  // Should only include D7
  EXPECT_TRUE(result.test(Square::D7));
  EXPECT_FALSE(result.test(Square::D6));
  EXPECT_EQ(result.count(), 1);
}

/**
 * @test RookMoveGenerator::south_ray() with multiple blockers
 * @brief Verifies that south_ray stops at the FIRST blocker, not subsequent ones.
 */
TEST(RookMoveGeneratorTest, SouthRayStopAtFirstBlockerOnly) {
  Bitboard occupied;
  occupied.set(Square::D5);  // First blocker
  occupied.set(Square::D2);  // Second blocker (should be ignored)
  Square from = Squares::D8;
  Bitboard result = RookMoveGenerator::south_ray(from, occupied);

  // Should include: D7, D6, D5 (first blocker), but not D4, D3, D2, D1
  EXPECT_TRUE(result.test(Square::D7));
  EXPECT_TRUE(result.test(Square::D6));
  EXPECT_TRUE(result.test(Square::D5));
  EXPECT_FALSE(result.test(Square::D4));
  EXPECT_FALSE(result.test(Square::D3));
  EXPECT_FALSE(result.test(Square::D2));
  EXPECT_EQ(result.count(), 3);
}

/**
 * @test RookMoveGenerator::south_ray() from rank 1 edge squares
 * @brief Verifies that south_ray returns empty bitboard when starting from the south edge
 *        where no moves are possible in that direction.
 */
TEST(RookMoveGeneratorTest, SouthRayFromRank1EdgeSquares) {
  Bitboard occupied;

  // South from A1 (bottom rank, file A) - no squares available
  Bitboard south_result = RookMoveGenerator::south_ray(Squares::A1, occupied);
  EXPECT_EQ(south_result.count(), 0);

  // South from H1 (bottom rank, file H) - no squares available
  south_result = RookMoveGenerator::south_ray(Squares::H1, occupied);
  EXPECT_EQ(south_result.count(), 0);
}

/**
 * @test RookMoveGenerator::south_ray() from rank 1 with different files
 * @brief Verifies that south_ray returns empty bitboard from any rank 1 square.
 */
TEST(RookMoveGeneratorTest, SouthRayFromRank1DifferentFiles) {
  Bitboard occupied;

  // South from D1 (bottom rank, file D) - no squares available
  Bitboard south_result = RookMoveGenerator::south_ray(Squares::D1, occupied);
  EXPECT_EQ(south_result.count(), 0);

  // South from E1 (bottom rank, file E) - no squares available
  south_result = RookMoveGenerator::south_ray(Squares::E1, occupied);
  EXPECT_EQ(south_result.count(), 0);
}

/**
 * @test RookMoveGenerator::south_ray() from center board positions
 * @brief Verifies that south_ray correctly handles starting positions in the middle of the board.
 */
TEST(RookMoveGeneratorTest, SouthRayFromCenterPositions) {
  Bitboard occupied;

  // From D5 (middle of board)
  Bitboard result = RookMoveGenerator::south_ray(Squares::D5, occupied);
  EXPECT_TRUE(result.test(Square::D4));
  EXPECT_TRUE(result.test(Square::D3));
  EXPECT_TRUE(result.test(Square::D2));
  EXPECT_TRUE(result.test(Square::D1));
  EXPECT_EQ(result.count(), 4);

  // From E6 (also middle of board)
  result = RookMoveGenerator::south_ray(Squares::E6, occupied);
  EXPECT_TRUE(result.test(Square::E5));
  EXPECT_TRUE(result.test(Square::E4));
  EXPECT_TRUE(result.test(Square::E3));
  EXPECT_TRUE(result.test(Square::E2));
  EXPECT_TRUE(result.test(Square::E1));
  EXPECT_EQ(result.count(), 5);
}

/**
 * @test RookMoveGenerator::south_ray() from near-edge positions
 * @brief Verifies that south_ray correctly handles starting positions near the southern edge.
 */
TEST(RookMoveGeneratorTest, SouthRayFromNearEdgePositions) {
  Bitboard occupied;

  // From D2 (one square from edge)
  Bitboard result = RookMoveGenerator::south_ray(Squares::D2, occupied);
  EXPECT_TRUE(result.test(Square::D1));
  EXPECT_EQ(result.count(), 1);

  // From C3 (two squares from edge)
  result = RookMoveGenerator::south_ray(Squares::C3, occupied);
  EXPECT_TRUE(result.test(Square::C2));
  EXPECT_TRUE(result.test(Square::C1));
  EXPECT_EQ(result.count(), 2);
}
