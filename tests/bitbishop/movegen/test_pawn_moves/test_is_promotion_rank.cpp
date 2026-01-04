#include <gtest/gtest.h>

#include <bitbishop/movegen/pawn_moves.hpp>

/**
 * @test Verifies is_promotion_rank returns true for White pawns on rank 8 (A8-H8).
 */
TEST(PawnMoveGeneratorTest, IsPromotionRankFromPromotionRankForWhite) {
  for (int sq = Square::A8; sq <= Square::H8; sq++) {
    ASSERT_TRUE(is_promotion_rank(Square(sq), Color::WHITE)) << "for square " << Square(sq).to_string();
  }
}

/**
 * @test Verifies is_promotion_rank returns true for Black pawns on rank 1 (A1-H1).
 */
TEST(PawnMoveGeneratorTest, IsPromotionRankFromPromotionRankForBlack) {
  for (int sq = Square::A1; sq <= Square::H1; sq++) {
    ASSERT_TRUE(is_promotion_rank(Square(sq), Color::BLACK)) << "for square " << Square(sq).to_string();
  }
}

/**
 * @test Verifies is_promotion_rank returns false for White pawns on ranks 1-7.
 */
TEST(PawnMoveGeneratorTest, IsPromotionRankNotFromPromotionRankForWhite) {
  for (int sq = Square::A1; sq <= Square::H7; sq++) {
    ASSERT_FALSE(is_promotion_rank(Square(sq), Color::WHITE)) << "for square " << Square(sq).to_string();
  }
}

/**
 * @test Verifies is_promotion_rank returns false for Black pawns on ranks 2-8.
 */
TEST(PawnMoveGeneratorTest, IsPromotionRankNotFromPromotionRankForBlack) {
  for (int sq = Square::A2; sq <= Square::H8; sq++) {
    ASSERT_FALSE(is_promotion_rank(Square(sq), Color::BLACK)) << "for square " << Square(sq).to_string();
  }
}
