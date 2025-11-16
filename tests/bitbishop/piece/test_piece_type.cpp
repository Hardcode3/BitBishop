#include <gtest/gtest.h>

#include <bitbishop/piece.hpp>

/**
 * @test Verifies same piece type for both colors.
 */
TEST(PieceTest, SameTypeForBothColors) {
  EXPECT_EQ(Pieces::WHITE_PAWN.type(), Pieces::BLACK_PAWN.type());
  EXPECT_EQ(Pieces::WHITE_KNIGHT.type(), Pieces::BLACK_KNIGHT.type());
  EXPECT_EQ(Pieces::WHITE_BISHOP.type(), Pieces::BLACK_BISHOP.type());
  EXPECT_EQ(Pieces::WHITE_ROOK.type(), Pieces::BLACK_ROOK.type());
  EXPECT_EQ(Pieces::WHITE_QUEEN.type(), Pieces::BLACK_QUEEN.type());
  EXPECT_EQ(Pieces::WHITE_KING.type(), Pieces::BLACK_KING.type());
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

  EXPECT_NE(Piece::KNIGHT, Piece::BISHOP);
  EXPECT_NE(Piece::KNIGHT, Piece::ROOK);
  EXPECT_NE(Piece::KNIGHT, Piece::QUEEN);
  EXPECT_NE(Piece::KNIGHT, Piece::KING);
}
