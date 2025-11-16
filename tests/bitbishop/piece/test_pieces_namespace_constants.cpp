#include <gtest/gtest.h>

#include <bitbishop/piece.hpp>

/**
 * @test Verifies White piece constants are correctly defined.
 */
TEST(PieceTest, WhitePieceConstantsAreCorrect) {
  using namespace Pieces;

  EXPECT_EQ(WHITE_PAWN.type(), Piece::PAWN);
  EXPECT_TRUE(WHITE_PAWN.is_white());

  EXPECT_EQ(WHITE_KNIGHT.type(), Piece::KNIGHT);
  EXPECT_TRUE(WHITE_KNIGHT.is_white());

  EXPECT_EQ(WHITE_BISHOP.type(), Piece::BISHOP);
  EXPECT_TRUE(WHITE_BISHOP.is_white());

  EXPECT_EQ(WHITE_ROOK.type(), Piece::ROOK);
  EXPECT_TRUE(WHITE_ROOK.is_white());

  EXPECT_EQ(WHITE_QUEEN.type(), Piece::QUEEN);
  EXPECT_TRUE(WHITE_QUEEN.is_white());

  EXPECT_EQ(WHITE_KING.type(), Piece::KING);
  EXPECT_TRUE(WHITE_KING.is_white());
}

/**
 * @test Verifies Black piece constants are correctly defined.
 */
TEST(PieceTest, BlackPieceConstantsAreCorrect) {
  using namespace Pieces;

  EXPECT_EQ(BLACK_PAWN.type(), Piece::PAWN);
  EXPECT_TRUE(BLACK_PAWN.is_black());

  EXPECT_EQ(BLACK_KNIGHT.type(), Piece::KNIGHT);
  EXPECT_TRUE(BLACK_KNIGHT.is_black());

  EXPECT_EQ(BLACK_BISHOP.type(), Piece::BISHOP);
  EXPECT_TRUE(BLACK_BISHOP.is_black());

  EXPECT_EQ(BLACK_ROOK.type(), Piece::ROOK);
  EXPECT_TRUE(BLACK_ROOK.is_black());

  EXPECT_EQ(BLACK_QUEEN.type(), Piece::QUEEN);
  EXPECT_TRUE(BLACK_QUEEN.is_black());

  EXPECT_EQ(BLACK_KING.type(), Piece::KING);
  EXPECT_TRUE(BLACK_KING.is_black());
}

/**
 * @test Verifies constants can be used in comparisons.
 */
TEST(PieceTest, ConstantsCanBeUsedInComparisons) {
  using namespace Pieces;

  Piece p('P');
  EXPECT_EQ(p, WHITE_PAWN);
  EXPECT_NE(p, BLACK_PAWN);
  EXPECT_NE(p, WHITE_KNIGHT);
}
