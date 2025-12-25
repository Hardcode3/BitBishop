
#include <gtest/gtest.h>

#include <bitbishop/attacks/bishop_attacks.hpp>
#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/moves/bishop_move_gen.hpp>

/**
 * @test bishop_south_east_attacks() with no blockers
 * @brief Verifies that bishop_south_east_attacks returns all squares in the SE diagonal when unobstructed.
 */
TEST(BishopMoveGeneratorTest, SouthEastRayNoBlockers) {
  Bitboard occupied;
  Square from = Squares::A8;  // Top-left corner

  Bitboard result = bishop_south_east_attacks(from, occupied);

  // Should include: B7, C6, D5, E4, F3, G2, H1
  EXPECT_TRUE(result.test(Square::B7));
  EXPECT_TRUE(result.test(Square::C6));
  EXPECT_TRUE(result.test(Square::D5));
  EXPECT_TRUE(result.test(Square::E4));
  EXPECT_TRUE(result.test(Square::F3));
  EXPECT_TRUE(result.test(Square::G2));
  EXPECT_TRUE(result.test(Square::H1));
  EXPECT_EQ(result.count(), 7);
}

/**
 * @test bishop_south_east_attacks() with a blocker
 * @brief Verifies that bishop_south_east_attacks stops at the first blocker (inclusive).
 */
TEST(BishopMoveGeneratorTest, SouthEastRayWithBlocker) {
  Bitboard occupied;
  occupied.set(Square::D5);  // Blocker at D5
  Square from = Squares::A8;

  Bitboard result = bishop_south_east_attacks(from, occupied);

  // Should include: B7, C6, D5 (blocker), but not E4, F3, G2, H1
  EXPECT_TRUE(result.test(Square::B7));
  EXPECT_TRUE(result.test(Square::C6));
  EXPECT_TRUE(result.test(Square::D5));  // Blocker is included
  EXPECT_FALSE(result.test(Square::E4));
  EXPECT_FALSE(result.test(Square::F3));
  EXPECT_FALSE(result.test(Square::G2));
  EXPECT_FALSE(result.test(Square::H1));
  EXPECT_EQ(result.count(), 3);
}

/**
 * @test bishop_south_east_attacks() with immediate blocker
 * @brief Verifies that bishop_south_east_attacks handles a blocker on the first square.
 */
TEST(BishopMoveGeneratorTest, SouthEastRayWithImmediateBlocker) {
  Bitboard occupied;
  occupied.set(Square::B7);  // Immediate blocker
  Square from = Squares::A8;

  Bitboard result = bishop_south_east_attacks(from, occupied);

  // Should only include B7
  EXPECT_TRUE(result.test(Square::B7));
  EXPECT_FALSE(result.test(Square::C6));
  EXPECT_EQ(result.count(), 1);
}

/**
 * @test bishop_south_east_attacks() with multiple blockers
 * @brief Verifies that each ray function only stops at the FIRST blocker, not subsequent ones.
 */
TEST(BishopMoveGeneratorTest, SouthEastRayStopAtFirstBlockerOnly) {
  Bitboard occupied;
  occupied.set(Square::C6);  // First blocker
  occupied.set(Square::E4);  // Second blocker (should be ignored)
  Square from = Squares::A8;

  Bitboard result = bishop_south_east_attacks(from, occupied);

  // Should include: B7, C6 (first blocker), but not D5, E4, F3, G2, H1
  EXPECT_TRUE(result.test(Square::B7));
  EXPECT_TRUE(result.test(Square::C6));
  EXPECT_FALSE(result.test(Square::D5));
  EXPECT_FALSE(result.test(Square::E4));
  EXPECT_EQ(result.count(), 2);
}

/**
 * @test bishop_south_east_attacks() from edge squares
 * @brief Verifies that ray functions return empty bitboards when starting from edge squares
 *        where no moves are possible in that direction.
 */
TEST(BishopMoveGeneratorTest, SouthEastRayFromEdgeSquares) {
  Bitboard occupied;

  // SE from H1 (bottom-right corner) - no squares available
  Bitboard se_result = bishop_south_east_attacks(Squares::H1, occupied);
  EXPECT_EQ(se_result.count(), 0);
}
