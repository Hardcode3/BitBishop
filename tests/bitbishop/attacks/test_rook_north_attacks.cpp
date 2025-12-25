#include <gtest/gtest.h>

#include <bitbishop/attacks/rook_attacks.hpp>
#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/moves/rook_move_gen.hpp>

/**
 * @test rook_north_attacks() with no blockers
 * @brief Verifies that rook_north_attacks returns all squares to the north when unobstructed.
 */
TEST(RookMoveGeneratorTest, NorthRayNoBlockers) {
  Bitboard occupied;
  Square from = Squares::D1;  // Bottom of board, file D
  Bitboard result = rook_north_attacks(from, occupied);

  // Should include: D2, D3, D4, D5, D6, D7, D8
  EXPECT_TRUE(result.test(Square::D2));
  EXPECT_TRUE(result.test(Square::D3));
  EXPECT_TRUE(result.test(Square::D4));
  EXPECT_TRUE(result.test(Square::D5));
  EXPECT_TRUE(result.test(Square::D6));
  EXPECT_TRUE(result.test(Square::D7));
  EXPECT_TRUE(result.test(Square::D8));
  EXPECT_EQ(result.count(), 7);
}

/**
 * @test rook_north_attacks() with a blocker
 * @brief Verifies that rook_north_attacks stops at the first blocker (inclusive).
 */
TEST(RookMoveGeneratorTest, NorthRayWithBlocker) {
  Bitboard occupied;
  occupied.set(Square::D5);  // Blocker at D5
  Square from = Squares::D1;
  Bitboard result = rook_north_attacks(from, occupied);

  // Should include: D2, D3, D4, D5 (blocker), but not D6, D7, D8
  EXPECT_TRUE(result.test(Square::D2));
  EXPECT_TRUE(result.test(Square::D3));
  EXPECT_TRUE(result.test(Square::D4));
  EXPECT_TRUE(result.test(Square::D5));  // Blocker is included
  EXPECT_FALSE(result.test(Square::D6));
  EXPECT_FALSE(result.test(Square::D7));
  EXPECT_FALSE(result.test(Square::D8));
  EXPECT_EQ(result.count(), 4);
}

/**
 * @test rook_north_attacks() with immediate blocker
 * @brief Verifies that rook_north_attacks handles a blocker on the first square.
 */
TEST(RookMoveGeneratorTest, NorthRayWithImmediateBlocker) {
  Bitboard occupied;
  occupied.set(Square::D2);  // Immediate blocker
  Square from = Squares::D1;
  Bitboard result = rook_north_attacks(from, occupied);

  // Should only include D2
  EXPECT_TRUE(result.test(Square::D2));
  EXPECT_FALSE(result.test(Square::D3));
  EXPECT_EQ(result.count(), 1);
}

/**
 * @test rook_north_attacks() with multiple blockers
 * @brief Verifies that rook_north_attacks stops at the FIRST blocker, not subsequent ones.
 */
TEST(RookMoveGeneratorTest, NorthRayStopAtFirstBlockerOnly) {
  Bitboard occupied;
  occupied.set(Square::D4);  // First blocker
  occupied.set(Square::D7);  // Second blocker (should be ignored)
  Square from = Squares::D1;
  Bitboard result = rook_north_attacks(from, occupied);

  // Should include: D2, D3, D4 (first blocker), but not D5, D6, D7, D8
  EXPECT_TRUE(result.test(Square::D2));
  EXPECT_TRUE(result.test(Square::D3));
  EXPECT_TRUE(result.test(Square::D4));
  EXPECT_FALSE(result.test(Square::D5));
  EXPECT_FALSE(result.test(Square::D6));
  EXPECT_FALSE(result.test(Square::D7));
  EXPECT_EQ(result.count(), 3);
}

/**
 * @test rook_north_attacks() from rank 8 edge squares
 * @brief Verifies that rook_north_attacks returns empty bitboard when starting from the north edge
 *        where no moves are possible in that direction.
 */
TEST(RookMoveGeneratorTest, NorthRayFromRank8EdgeSquares) {
  Bitboard occupied;

  // North from A8 (top rank, file A) - no squares available
  Bitboard north_result = rook_north_attacks(Squares::A8, occupied);
  EXPECT_EQ(north_result.count(), 0);

  // North from H8 (top rank, file H) - no squares available
  north_result = rook_north_attacks(Squares::H8, occupied);
  EXPECT_EQ(north_result.count(), 0);
}

/**
 * @test rook_north_attacks() from rank 8 with different files
 * @brief Verifies that rook_north_attacks returns empty bitboard from any rank 8 square.
 */
TEST(RookMoveGeneratorTest, NorthRayFromRank8DifferentFiles) {
  Bitboard occupied;

  // North from D8 (top rank, file D) - no squares available
  Bitboard north_result = rook_north_attacks(Squares::D8, occupied);
  EXPECT_EQ(north_result.count(), 0);

  // North from E8 (top rank, file E) - no squares available
  north_result = rook_north_attacks(Squares::E8, occupied);
  EXPECT_EQ(north_result.count(), 0);
}

/**
 * @test rook_north_attacks() from center board positions
 * @brief Verifies that rook_north_attacks correctly handles starting positions in the middle of the board.
 */
TEST(RookMoveGeneratorTest, NorthRayFromCenterPositions) {
  Bitboard occupied;

  // From D4 (middle of board)
  Bitboard result = rook_north_attacks(Squares::D4, occupied);
  EXPECT_TRUE(result.test(Square::D5));
  EXPECT_TRUE(result.test(Square::D6));
  EXPECT_TRUE(result.test(Square::D7));
  EXPECT_TRUE(result.test(Square::D8));
  EXPECT_EQ(result.count(), 4);

  // From E3 (also middle of board)
  result = rook_north_attacks(Squares::E3, occupied);
  EXPECT_TRUE(result.test(Square::E4));
  EXPECT_TRUE(result.test(Square::E5));
  EXPECT_TRUE(result.test(Square::E6));
  EXPECT_TRUE(result.test(Square::E7));
  EXPECT_TRUE(result.test(Square::E8));
  EXPECT_EQ(result.count(), 5);
}

/**
 * @test rook_north_attacks() from near-edge positions
 * @brief Verifies that rook_north_attacks correctly handles starting positions near the northern edge.
 */
TEST(RookMoveGeneratorTest, NorthRayFromNearEdgePositions) {
  Bitboard occupied;

  // From D7 (one square from edge)
  Bitboard result = rook_north_attacks(Squares::D7, occupied);
  EXPECT_TRUE(result.test(Square::D8));
  EXPECT_EQ(result.count(), 1);

  // From C6 (two squares from edge)
  result = rook_north_attacks(Squares::C6, occupied);
  EXPECT_TRUE(result.test(Square::C7));
  EXPECT_TRUE(result.test(Square::C8));
  EXPECT_EQ(result.count(), 2);
}