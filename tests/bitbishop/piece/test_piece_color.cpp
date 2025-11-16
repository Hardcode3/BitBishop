#include <gtest/gtest.h>

#include <bitbishop/piece.hpp>

/**
 * @test Verifies all White pieces return correct color.
 */
TEST(PieceTest, WhitePiecesHaveWhiteColor) {
  EXPECT_EQ(Piece('P').color(), Color::WHITE);
  EXPECT_EQ(Piece('N').color(), Color::WHITE);
  EXPECT_EQ(Piece('B').color(), Color::WHITE);
  EXPECT_EQ(Piece('R').color(), Color::WHITE);
  EXPECT_EQ(Piece('Q').color(), Color::WHITE);
  EXPECT_EQ(Piece('K').color(), Color::WHITE);
}

/**
 * @test Verifies all Black pieces return correct color.
 */
TEST(PieceTest, BlackPiecesHaveBlackColor) {
  EXPECT_EQ(Piece('p').color(), Color::BLACK);
  EXPECT_EQ(Piece('n').color(), Color::BLACK);
  EXPECT_EQ(Piece('b').color(), Color::BLACK);
  EXPECT_EQ(Piece('r').color(), Color::BLACK);
  EXPECT_EQ(Piece('q').color(), Color::BLACK);
  EXPECT_EQ(Piece('k').color(), Color::BLACK);
}

/**
 * @test Verifies NONE piece has WHITE color by default.
 */
TEST(PieceTest, NonePieceHasWhiteColorByDefault) {
  Piece p('.');

  EXPECT_EQ(p.color(), Color::WHITE);
}

/**
 * @test Verifies is_white() returns true only for White pieces.
 */
TEST(PieceTest, IsWhiteReturnsTrueForWhitePieces) {
  EXPECT_TRUE(Piece('P').is_white());
  EXPECT_TRUE(Piece('N').is_white());
  EXPECT_TRUE(Piece('B').is_white());
  EXPECT_TRUE(Piece('R').is_white());
  EXPECT_TRUE(Piece('Q').is_white());
  EXPECT_TRUE(Piece('K').is_white());

  EXPECT_FALSE(Piece('p').is_white());
  EXPECT_FALSE(Piece('n').is_white());
  EXPECT_FALSE(Piece('b').is_white());
  EXPECT_FALSE(Piece('r').is_white());
  EXPECT_FALSE(Piece('q').is_white());
  EXPECT_FALSE(Piece('k').is_white());
}

/**
 * @test Verifies is_black() returns true only for Black pieces.
 */
TEST(PieceTest, IsBlackReturnsTrueForBlackPieces) {
  EXPECT_TRUE(Piece('p').is_black());
  EXPECT_TRUE(Piece('n').is_black());
  EXPECT_TRUE(Piece('b').is_black());
  EXPECT_TRUE(Piece('r').is_black());
  EXPECT_TRUE(Piece('q').is_black());
  EXPECT_TRUE(Piece('k').is_black());

  EXPECT_FALSE(Piece('P').is_black());
  EXPECT_FALSE(Piece('N').is_black());
  EXPECT_FALSE(Piece('B').is_black());
  EXPECT_FALSE(Piece('R').is_black());
  EXPECT_FALSE(Piece('Q').is_black());
  EXPECT_FALSE(Piece('K').is_black());
}
