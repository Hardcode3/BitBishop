#include <gtest/gtest.h>

#include <chess_engine/movegen/pawn_move_gen.hpp>

TEST(PawnMoveGeneratorTest, IsStartingRankFromStartingRankForWhite) {
  for (int sq = Square::A2; sq <= Square::H2; sq++) {
    ASSERT_TRUE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::WHITE))
        << "for square " << Square(sq).to_string();
  }
}

TEST(PawnMoveGeneratorTest, IsStartingRankFromStartingRankForBlack) {
  for (int sq = Square::A7; sq <= Square::H7; sq++) {
    ASSERT_TRUE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::BLACK))
        << "for square " << Square(sq).to_string();
  }
}

TEST(PawnMoveGeneratorTest, IsStartingRankFromNotSartingRankForWhite) {
  // first rank is not a white pawn starting rank
  for (int sq = Square::A1; sq <= Square::H1; sq++) {
    ASSERT_FALSE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::WHITE))
        << "for square " << Square(sq).to_string();
  }
  // ranks 3 to 8 are not white pawn starting ranks
  for (int sq = Square::A3; sq <= Square::H8; sq++) {
    ASSERT_FALSE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::WHITE))
        << "for square " << Square(sq).to_string();
  }
}

TEST(PawnMoveGeneratorTest, IsStartingRankFromNotSartingRankForBlack) {
  // ranks 1 to 5 are not black pawn starting ranks
  for (int sq = Square::A1; sq <= Square::H6; sq++) {
    ASSERT_FALSE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::BLACK))
        << "for square " << Square(sq).to_string();
  }
  // rank 8 is not a black pawn starting rank
  for (int sq = Square::A8; sq <= Square::H8; sq++) {
    ASSERT_FALSE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::BLACK))
        << "for square " << Square(sq).to_string();
  }
}
