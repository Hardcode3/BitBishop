#include <gtest/gtest.h>

#include <bitbishop/piece.hpp>

/**
 * @test Verifies equality operator for same pieces.
 */
TEST(PieceTest, EqualityOperatorForSamePieces) {
  EXPECT_EQ(Piece('P'), Piece('P'));
  EXPECT_EQ(Piece('n'), Piece('n'));
  EXPECT_EQ(Piece('.'), Piece('.'));
}

/**
 * @test Verifies equality operator for different pieces.
 */
TEST(PieceTest, EqualityOperatorForDifferentPieces) {
  EXPECT_NE(Piece('P'), Piece('N'));
  EXPECT_NE(Piece('P'), Piece('p'));
  EXPECT_NE(Piece('K'), Piece('.'));
}

/**
 * @test Verifies inequality operator.
 */
TEST(PieceTest, InequalityOperator) {
  EXPECT_TRUE(Piece('P') != Piece('N'));
  EXPECT_TRUE(Piece('P') != Piece('p'));
  EXPECT_FALSE(Piece('P') != Piece('P'));
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
