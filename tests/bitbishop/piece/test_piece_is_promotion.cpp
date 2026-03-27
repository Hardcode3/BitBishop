#include <gtest/gtest.h>

#include <bitbishop/piece.hpp>

TEST(PieceTest, IsPromotionReturnsTrueOnPromotionPieces) {
  EXPECT_TRUE(Pieces::BLACK_BISHOP.is_promotion());
  EXPECT_TRUE(Pieces::BLACK_KNIGHT.is_promotion());
  EXPECT_TRUE(Pieces::BLACK_QUEEN.is_promotion());
  EXPECT_TRUE(Pieces::BLACK_ROOK.is_promotion());

  EXPECT_TRUE(Pieces::WHITE_BISHOP.is_promotion());
  EXPECT_TRUE(Pieces::WHITE_KNIGHT.is_promotion());
  EXPECT_TRUE(Pieces::WHITE_QUEEN.is_promotion());
  EXPECT_TRUE(Pieces::WHITE_ROOK.is_promotion());
}

TEST(PieceTest, IsPromotionReturnsFalseOnNonPromotionPieces) {
  EXPECT_FALSE(Pieces::BLACK_KING.is_promotion());
  EXPECT_FALSE(Pieces::BLACK_PAWN.is_promotion());

  EXPECT_FALSE(Pieces::WHITE_KING.is_promotion());
  EXPECT_FALSE(Pieces::WHITE_PAWN.is_promotion());
}
