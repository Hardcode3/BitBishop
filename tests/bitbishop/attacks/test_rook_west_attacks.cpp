#include <gtest/gtest.h>

#include <bitbishop/attacks/rook_attacks.hpp>
#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>

/**
 * @test rook_west_attacks() with no blockers
 * @brief Verifies that rook_west_attacks returns all squares to the west when unobstructed.
 */
TEST(RookMoveGeneratorTest, WestRayNoBlockers) {
  Bitboard occupied;
  Square from = Squares::H4;  // Right side of board, rank 4
  Bitboard result = rook_west_attacks(from, occupied);

  // Should include: G4, F4, E4, D4, C4, B4, A4
  EXPECT_TRUE(result.test(Square::G4));
  EXPECT_TRUE(result.test(Square::F4));
  EXPECT_TRUE(result.test(Square::E4));
  EXPECT_TRUE(result.test(Square::D4));
  EXPECT_TRUE(result.test(Square::C4));
  EXPECT_TRUE(result.test(Square::B4));
  EXPECT_TRUE(result.test(Square::A4));
  EXPECT_EQ(result.count(), 7);
}

/**
 * @test rook_west_attacks() with a blocker
 * @brief Verifies that rook_west_attacks stops at the first blocker (inclusive).
 */
TEST(RookMoveGeneratorTest, WestRayWithBlocker) {
  Bitboard occupied;
  occupied.set(Square::D4);  // Blocker at D4
  Square from = Squares::H4;
  Bitboard result = rook_west_attacks(from, occupied);

  // Should include: G4, F4, E4, D4 (blocker), but not C4, B4, A4
  EXPECT_TRUE(result.test(Square::G4));
  EXPECT_TRUE(result.test(Square::F4));
  EXPECT_TRUE(result.test(Square::E4));
  EXPECT_TRUE(result.test(Square::D4));  // Blocker is included
  EXPECT_FALSE(result.test(Square::C4));
  EXPECT_FALSE(result.test(Square::B4));
  EXPECT_FALSE(result.test(Square::A4));
  EXPECT_EQ(result.count(), 4);
}

/**
 * @test rook_west_attacks() with immediate blocker
 * @brief Verifies that rook_west_attacks handles a blocker on the first square.
 */
TEST(RookMoveGeneratorTest, WestRayWithImmediateBlocker) {
  Bitboard occupied;
  occupied.set(Square::G4);  // Immediate blocker
  Square from = Squares::H4;
  Bitboard result = rook_west_attacks(from, occupied);

  // Should only include G4
  EXPECT_TRUE(result.test(Square::G4));
  EXPECT_FALSE(result.test(Square::F4));
  EXPECT_EQ(result.count(), 1);
}

/**
 * @test rook_west_attacks() with multiple blockers
 * @brief Verifies that rook_west_attacks stops at the FIRST blocker, not subsequent ones.
 */
TEST(RookMoveGeneratorTest, WestRayStopAtFirstBlockerOnly) {
  Bitboard occupied;
  occupied.set(Square::E4);  // First blocker
  occupied.set(Square::B4);  // Second blocker (should be ignored)
  Square from = Squares::H4;
  Bitboard result = rook_west_attacks(from, occupied);

  // Should include: G4, F4, E4 (first blocker), but not D4, C4, B4, A4
  EXPECT_TRUE(result.test(Square::G4));
  EXPECT_TRUE(result.test(Square::F4));
  EXPECT_TRUE(result.test(Square::E4));
  EXPECT_FALSE(result.test(Square::D4));
  EXPECT_FALSE(result.test(Square::C4));
  EXPECT_FALSE(result.test(Square::B4));
  EXPECT_EQ(result.count(), 3);
}

/**
 * @test rook_west_attacks() from A-file edge squares
 * @brief Verifies that rook_west_attacks returns empty bitboard when starting from the west edge
 *        where no moves are possible in that direction.
 */
TEST(RookMoveGeneratorTest, WestRayFromAFileEdgeSquares) {
  Bitboard occupied;

  // West from A1 (leftmost column, rank 1) - no squares available
  Bitboard west_result = rook_west_attacks(Squares::A1, occupied);
  EXPECT_EQ(west_result.count(), 0);

  // West from A8 (leftmost column, rank 8) - no squares available
  west_result = rook_west_attacks(Squares::A8, occupied);
  EXPECT_EQ(west_result.count(), 0);
}

/**
 * @test rook_west_attacks() from A-file with different ranks
 * @brief Verifies that rook_west_attacks returns empty bitboard from any A-file square.
 */
TEST(RookMoveGeneratorTest, WestRayFromAFileDifferentRanks) {
  Bitboard occupied;

  // West from A4 (leftmost column, rank 4) - no squares available
  Bitboard west_result = rook_west_attacks(Squares::A4, occupied);
  EXPECT_EQ(west_result.count(), 0);

  // West from A5 (leftmost column, rank 5) - no squares available
  west_result = rook_west_attacks(Squares::A5, occupied);
  EXPECT_EQ(west_result.count(), 0);
}

/**
 * @test rook_west_attacks() from center board positions
 * @brief Verifies that rook_west_attacks correctly handles starting positions in the middle of the board.
 */
TEST(RookMoveGeneratorTest, WestRayFromCenterPositions) {
  Bitboard occupied;

  // From D4 (middle of board)
  Bitboard result = rook_west_attacks(Squares::D4, occupied);
  EXPECT_TRUE(result.test(Square::C4));
  EXPECT_TRUE(result.test(Square::B4));
  EXPECT_TRUE(result.test(Square::A4));
  EXPECT_EQ(result.count(), 3);

  // From E5 (also middle of board)
  result = rook_west_attacks(Squares::E5, occupied);
  EXPECT_TRUE(result.test(Square::D5));
  EXPECT_TRUE(result.test(Square::C5));
  EXPECT_TRUE(result.test(Square::B5));
  EXPECT_TRUE(result.test(Square::A5));
  EXPECT_EQ(result.count(), 4);
}

/**
 * @test rook_west_attacks() from near-edge positions
 * @brief Verifies that rook_west_attacks correctly handles starting positions near the western edge.
 */
TEST(RookMoveGeneratorTest, WestRayFromNearEdgePositions) {
  Bitboard occupied;

  // From B1 (one square from edge)
  Bitboard result = rook_west_attacks(Squares::B1, occupied);
  EXPECT_TRUE(result.test(Square::A1));
  EXPECT_EQ(result.count(), 1);

  // From C3 (two squares from edge)
  result = rook_west_attacks(Squares::C3, occupied);
  EXPECT_TRUE(result.test(Square::B3));
  EXPECT_TRUE(result.test(Square::A3));
  EXPECT_EQ(result.count(), 2);
}
