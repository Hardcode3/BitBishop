#include <gtest/gtest.h>

#include <bitbishop/piece.hpp>

/**
 * @test Verifies equality operator for same pieces.
 */
TEST(PieceTest, EqualityOperatorForSamePieces) {
  EXPECT_EQ(Pieces::WHITE_PAWN, Pieces::WHITE_PAWN);
  EXPECT_EQ(Pieces::BLACK_KNIGHT, Pieces::BLACK_KNIGHT);
}

/**
 * @test Verifies equality operator for different pieces.
 */
TEST(PieceTest, EqualityOperatorForDifferentPieces) {
  EXPECT_NE(Pieces::WHITE_PAWN, Pieces::WHITE_KNIGHT);
  EXPECT_NE(Pieces::WHITE_PAWN, Pieces::BLACK_PAWN);
}

/**
 * @test Verifies inequality operator.
 */
TEST(PieceTest, InequalityOperator) {
  EXPECT_TRUE(Pieces::WHITE_PAWN != Pieces::WHITE_KNIGHT);
  EXPECT_TRUE(Pieces::WHITE_PAWN != Pieces::BLACK_PAWN);
  EXPECT_FALSE(Pieces::WHITE_PAWN != Pieces::WHITE_PAWN);
}

/**
 * @test Verifies pieces of same type but different colors are not equal.
 */
TEST(PieceTest, SameTypeDifferentColorsNotEqual) {
  Piece white_pawn(Piece::PAWN, Color::WHITE);
  Piece black_pawn(Piece::PAWN, Color::BLACK);

  EXPECT_EQ(white_pawn.type(), black_pawn.type());
  EXPECT_NE(white_pawn.color(), black_pawn.color());
  EXPECT_NE(white_pawn, black_pawn);
}
