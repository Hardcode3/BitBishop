#include <gtest/gtest.h>

#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/moves/rook_move_gen.hpp>

/**
 * @test RookMoveGenerator::east_ray() with no blockers
 * @brief Verifies that east_ray returns all squares to the east when unobstructed.
 */
TEST(RookMoveGeneratorTest, EastRayNoBlockers) {
  Bitboard occupied;
  Square from = Squares::A4;  // Left side of board, rank 4
  Bitboard result = RookMoveGenerator::east_ray(from, occupied);

  // Should include: B4, C4, D4, E4, F4, G4, H4
  EXPECT_TRUE(result.test(Square::B4));
  EXPECT_TRUE(result.test(Square::C4));
  EXPECT_TRUE(result.test(Square::D4));
  EXPECT_TRUE(result.test(Square::E4));
  EXPECT_TRUE(result.test(Square::F4));
  EXPECT_TRUE(result.test(Square::G4));
  EXPECT_TRUE(result.test(Square::H4));
  EXPECT_EQ(result.count(), 7);
}

/**
 * @test RookMoveGenerator::east_ray() with a blocker
 * @brief Verifies that east_ray stops at the first blocker (inclusive).
 */
TEST(RookMoveGeneratorTest, EastRayWithBlocker) {
  Bitboard occupied;
  occupied.set(Square::E4);  // Blocker at E4
  Square from = Squares::A4;
  Bitboard result = RookMoveGenerator::east_ray(from, occupied);

  // Should include: B4, C4, D4, E4 (blocker), but not F4, G4, H4
  EXPECT_TRUE(result.test(Square::B4));
  EXPECT_TRUE(result.test(Square::C4));
  EXPECT_TRUE(result.test(Square::D4));
  EXPECT_TRUE(result.test(Square::E4));  // Blocker is included
  EXPECT_FALSE(result.test(Square::F4));
  EXPECT_FALSE(result.test(Square::G4));
  EXPECT_FALSE(result.test(Square::H4));
  EXPECT_EQ(result.count(), 4);
}

/**
 * @test RookMoveGenerator::east_ray() with immediate blocker
 * @brief Verifies that east_ray handles a blocker on the first square.
 */
TEST(RookMoveGeneratorTest, EastRayWithImmediateBlocker) {
  Bitboard occupied;
  occupied.set(Square::B4);  // Immediate blocker
  Square from = Squares::A4;
  Bitboard result = RookMoveGenerator::east_ray(from, occupied);

  // Should only include B4
  EXPECT_TRUE(result.test(Square::B4));
  EXPECT_FALSE(result.test(Square::C4));
  EXPECT_EQ(result.count(), 1);
}

/**
 * @test RookMoveGenerator::east_ray() with multiple blockers
 * @brief Verifies that east_ray stops at the FIRST blocker, not subsequent ones.
 */
TEST(RookMoveGeneratorTest, EastRayStopAtFirstBlockerOnly) {
  Bitboard occupied;
  occupied.set(Square::D4);  // First blocker
  occupied.set(Square::G4);  // Second blocker (should be ignored)
  Square from = Squares::A4;
  Bitboard result = RookMoveGenerator::east_ray(from, occupied);

  // Should include: B4, C4, D4 (first blocker), but not E4, F4, G4, H4
  EXPECT_TRUE(result.test(Square::B4));
  EXPECT_TRUE(result.test(Square::C4));
  EXPECT_TRUE(result.test(Square::D4));
  EXPECT_FALSE(result.test(Square::E4));
  EXPECT_FALSE(result.test(Square::F4));
  EXPECT_FALSE(result.test(Square::G4));
  EXPECT_EQ(result.count(), 3);
}

/**
 * @test RookMoveGenerator::east_ray() from H-file edge squares
 * @brief Verifies that east_ray returns empty bitboard when starting from the east edge
 *        where no moves are possible in that direction.
 */
TEST(RookMoveGeneratorTest, EastRayFromHFileEdgeSquares) {
  Bitboard occupied;

  // East from H1 (rightmost column, rank 1) - no squares available
  Bitboard east_result = RookMoveGenerator::east_ray(Squares::H1, occupied);
  EXPECT_EQ(east_result.count(), 0);

  // East from H8 (rightmost column, rank 8) - no squares available
  east_result = RookMoveGenerator::east_ray(Squares::H8, occupied);
  EXPECT_EQ(east_result.count(), 0);
}

/**
 * @test RookMoveGenerator::east_ray() from H-file with different ranks
 * @brief Verifies that east_ray returns empty bitboard from any H-file square.
 */
TEST(RookMoveGeneratorTest, EastRayFromHFileDifferentRanks) {
  Bitboard occupied;

  // East from H4 (rightmost column, rank 4) - no squares available
  Bitboard east_result = RookMoveGenerator::east_ray(Squares::H4, occupied);
  EXPECT_EQ(east_result.count(), 0);

  // East from H5 (rightmost column, rank 5) - no squares available
  east_result = RookMoveGenerator::east_ray(Squares::H5, occupied);
  EXPECT_EQ(east_result.count(), 0);
}

/**
 * @test RookMoveGenerator::east_ray() from center board positions
 * @brief Verifies that east_ray correctly handles starting positions in the middle of the board.
 */
TEST(RookMoveGeneratorTest, EastRayFromCenterPositions) {
  Bitboard occupied;

  // From D4 (middle of board)
  Bitboard result = RookMoveGenerator::east_ray(Squares::D4, occupied);
  EXPECT_TRUE(result.test(Square::E4));
  EXPECT_TRUE(result.test(Square::F4));
  EXPECT_TRUE(result.test(Square::G4));
  EXPECT_TRUE(result.test(Square::H4));
  EXPECT_EQ(result.count(), 4);

  // From C5 (also middle of board)
  result = RookMoveGenerator::east_ray(Squares::C5, occupied);
  EXPECT_TRUE(result.test(Square::D5));
  EXPECT_TRUE(result.test(Square::E5));
  EXPECT_TRUE(result.test(Square::F5));
  EXPECT_TRUE(result.test(Square::G5));
  EXPECT_TRUE(result.test(Square::H5));
  EXPECT_EQ(result.count(), 5);
}

/**
 * @test RookMoveGenerator::east_ray() from near-edge positions
 * @brief Verifies that east_ray correctly handles starting positions near the eastern edge.
 */
TEST(RookMoveGeneratorTest, EastRayFromNearEdgePositions) {
  Bitboard occupied;

  // From G1 (one square from edge)
  Bitboard result = RookMoveGenerator::east_ray(Squares::G1, occupied);
  EXPECT_TRUE(result.test(Square::H1));
  EXPECT_EQ(result.count(), 1);

  // From F3 (two squares from edge)
  result = RookMoveGenerator::east_ray(Squares::F3, occupied);
  EXPECT_TRUE(result.test(Square::G3));
  EXPECT_TRUE(result.test(Square::H3));
  EXPECT_EQ(result.count(), 2);
}