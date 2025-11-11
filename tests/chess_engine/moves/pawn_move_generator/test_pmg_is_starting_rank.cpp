#include <gtest/gtest.h>

#include <chess_engine/moves/pawn_move_gen.hpp>

/**
 * @test Verifies is_starting_rank returns true for White pawns on rank 2 (A2-H2).
 */
TEST(PawnMoveGeneratorTest, IsStartingRankFromStartingRankForWhite) {
  for (int sq = Square::A2; sq <= Square::H2; sq++) {
    ASSERT_TRUE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::WHITE))
        << "for square " << Square(sq).to_string();
  }
}

/**
 * @test Verifies is_starting_rank returns true for Black pawns on rank 7 (A7-H7).
 */
TEST(PawnMoveGeneratorTest, IsStartingRankFromStartingRankForBlack) {
  for (int sq = Square::A7; sq <= Square::H7; sq++) {
    ASSERT_TRUE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::BLACK))
        << "for square " << Square(sq).to_string();
  }
}

/**
 * @test Verifies is_starting_rank returns false for White pawns on all non-starting ranks.
 */
TEST(PawnMoveGeneratorTest, IsStartingRankFromNotSartingRankForWhite) {
  // Rank 1 is not a starting rank
  for (int sq = Square::A1; sq <= Square::H1; sq++) {
    ASSERT_FALSE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::WHITE))
        << "for square " << Square(sq).to_string();
  }
    // Ranks 3-8 are not starting ranks
    for (int sq = Square::A3; sq <= Square::H8; sq++) {
      ASSERT_FALSE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::WHITE))
          << "for square " << Square(sq).to_string();
    }
}

/**
 * @test Verifies is_starting_rank returns false for Black pawns on all non-starting ranks.
 */
TEST(PawnMoveGeneratorTest, IsStartingRankFromNotSartingRankForBlack) {
  // Ranks 1-6 are not starting ranks
  for (int sq = Square::A1; sq <= Square::H6; sq++) {
    ASSERT_FALSE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::BLACK))
        << "for square " << Square(sq).to_string();
  }
    // Rank 8 is not a starting rank
    for (int sq = Square::A8; sq <= Square::H8; sq++) {
      ASSERT_FALSE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::BLACK))
          << "for square " << Square(sq).to_string();
    }
}