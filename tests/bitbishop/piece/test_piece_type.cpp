#include <gtest/gtest.h>

#include <bitbishop/piece.hpp>

/**
 * @test Verifies same piece type for both colors.
 */
TEST(PieceTest, SameTypeForBothColors) {
  EXPECT_EQ(Piece('P').type(), Piece('p').type());
  EXPECT_EQ(Piece('N').type(), Piece('n').type());
  EXPECT_EQ(Piece('B').type(), Piece('b').type());
  EXPECT_EQ(Piece('R').type(), Piece('r').type());
  EXPECT_EQ(Piece('Q').type(), Piece('q').type());
  EXPECT_EQ(Piece('K').type(), Piece('k').type());
}

/**
 * @test Verifies all piece types are distinct.
 */
TEST(PieceTest, AllPieceTypesAreDistinct) {
  EXPECT_NE(Piece::PAWN, Piece::KNIGHT);
  EXPECT_NE(Piece::PAWN, Piece::BISHOP);
  EXPECT_NE(Piece::PAWN, Piece::ROOK);
  EXPECT_NE(Piece::PAWN, Piece::QUEEN);
  EXPECT_NE(Piece::PAWN, Piece::KING);
  EXPECT_NE(Piece::PAWN, Piece::NONE);

  EXPECT_NE(Piece::KNIGHT, Piece::BISHOP);
  EXPECT_NE(Piece::KNIGHT, Piece::ROOK);
  EXPECT_NE(Piece::KNIGHT, Piece::QUEEN);
  EXPECT_NE(Piece::KNIGHT, Piece::KING);
  EXPECT_NE(Piece::KNIGHT, Piece::NONE);
}
