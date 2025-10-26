#include <gtest/gtest.h>

#include <chess_engine/movegen/pawn_move_gen.hpp>

TEST(PawnMoveGeneratorTest, IsPromotionRankFromPromotionRankForWhite) {
  for (int sq = Square::A8; sq <= Square::H8; sq++) {
    ASSERT_TRUE(PawnMoveGenerator::is_promotion_rank(Square(sq), Color::WHITE))
        << "for square " << Square(sq).to_string();
  }
}

TEST(PawnMoveGeneratorTest, IsPromotionRankFromPromotionRankForBlack) {
  for (int sq = Square::A1; sq <= Square::H1; sq++) {
    ASSERT_TRUE(PawnMoveGenerator::is_promotion_rank(Square(sq), Color::BLACK))
        << "for square " << Square(sq).to_string();
  }
}

TEST(PawnMoveGeneratorTest, IsPromotionRankNotFromPromotionRankForWhite) {
  for (int sq = Square::A1; sq <= Square::H7; sq++) {
    ASSERT_FALSE(PawnMoveGenerator::is_promotion_rank(Square(sq), Color::WHITE))
        << "for square " << Square(sq).to_string();
  }
}

TEST(PawnMoveGeneratorTest, IsPromotionRankNotFromPromotionRankForBlack) {
  for (int sq = Square::A2; sq <= Square::H8; sq++) {
    ASSERT_FALSE(PawnMoveGenerator::is_promotion_rank(Square(sq), Color::BLACK))
        << "for square " << Square(sq).to_string();
  }
}
