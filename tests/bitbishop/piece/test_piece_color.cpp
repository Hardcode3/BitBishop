#include <gtest/gtest.h>

#include <bitbishop/piece.hpp>
#include <tuple>

/**
 * @test Verifies all White pieces return correct color.
 */
TEST(PieceTest, WhitePiecesHaveWhiteColor) {
  EXPECT_EQ(Pieces::WHITE_PAWN.color(), Color::WHITE);
  EXPECT_EQ(Pieces::WHITE_KNIGHT.color(), Color::WHITE);
  EXPECT_EQ(Pieces::WHITE_BISHOP.color(), Color::WHITE);
  EXPECT_EQ(Pieces::WHITE_ROOK.color(), Color::WHITE);
  EXPECT_EQ(Pieces::WHITE_QUEEN.color(), Color::WHITE);
  EXPECT_EQ(Pieces::WHITE_KING.color(), Color::WHITE);
}

/**
 * @test Verifies all Black pieces return correct color.
 */
TEST(PieceTest, BlackPiecesHaveBlackColor) {
  EXPECT_EQ(Pieces::BLACK_PAWN.color(), Color::BLACK);
  EXPECT_EQ(Pieces::BLACK_KNIGHT.color(), Color::BLACK);
  EXPECT_EQ(Pieces::BLACK_BISHOP.color(), Color::BLACK);
  EXPECT_EQ(Pieces::BLACK_ROOK.color(), Color::BLACK);
  EXPECT_EQ(Pieces::BLACK_QUEEN.color(), Color::BLACK);
  EXPECT_EQ(Pieces::BLACK_KING.color(), Color::BLACK);
}

/**
 * @test Verifies is_white() returns true only for White pieces.
 */
TEST(PieceTest, IsWhiteReturnsTrueForWhitePieces) {
  EXPECT_TRUE(Pieces::WHITE_PAWN.is_white());
  EXPECT_TRUE(Pieces::WHITE_KNIGHT.is_white());
  EXPECT_TRUE(Pieces::WHITE_BISHOP.is_white());
  EXPECT_TRUE(Pieces::WHITE_ROOK.is_white());
  EXPECT_TRUE(Pieces::WHITE_QUEEN.is_white());
  EXPECT_TRUE(Pieces::WHITE_KING.is_white());

  EXPECT_FALSE(Pieces::BLACK_PAWN.is_white());
  EXPECT_FALSE(Pieces::BLACK_KNIGHT.is_white());
  EXPECT_FALSE(Pieces::BLACK_BISHOP.is_white());
  EXPECT_FALSE(Pieces::BLACK_ROOK.is_white());
  EXPECT_FALSE(Pieces::BLACK_QUEEN.is_white());
  EXPECT_FALSE(Pieces::BLACK_KING.is_white());
}

/**
 * @test Verifies is_black() returns true only for Black pieces.
 */
TEST(PieceTest, IsBlackReturnsTrueForBlackPieces) {
  EXPECT_TRUE(Pieces::BLACK_PAWN.is_black());
  EXPECT_TRUE(Pieces::BLACK_KNIGHT.is_black());
  EXPECT_TRUE(Pieces::BLACK_BISHOP.is_black());
  EXPECT_TRUE(Pieces::BLACK_ROOK.is_black());
  EXPECT_TRUE(Pieces::BLACK_QUEEN.is_black());
  EXPECT_TRUE(Pieces::BLACK_KING.is_black());

  EXPECT_FALSE(Pieces::WHITE_PAWN.is_black());
  EXPECT_FALSE(Pieces::WHITE_KNIGHT.is_black());
  EXPECT_FALSE(Pieces::WHITE_BISHOP.is_black());
  EXPECT_FALSE(Pieces::WHITE_ROOK.is_black());
  EXPECT_FALSE(Pieces::WHITE_QUEEN.is_black());
  EXPECT_FALSE(Pieces::WHITE_KING.is_black());
}

TEST(PieceTest, ColorFromWhitePawnsChars) {
  EXPECT_EQ(Piece::color_from_char('P'), Color::WHITE);
  EXPECT_EQ(Piece::color_from_char('N'), Color::WHITE);
  EXPECT_EQ(Piece::color_from_char('B'), Color::WHITE);
  EXPECT_EQ(Piece::color_from_char('R'), Color::WHITE);
  EXPECT_EQ(Piece::color_from_char('Q'), Color::WHITE);

  // Should also work with other uppercase letters by construction
  EXPECT_EQ(Piece::color_from_char('Z'), Color::WHITE);
  EXPECT_EQ(Piece::color_from_char('A'), Color::WHITE);
  EXPECT_EQ(Piece::color_from_char('K'), Color::WHITE);
}

TEST(PieceTest, ColorFromBlackPawnsChars) {
  EXPECT_EQ(Piece::color_from_char('p'), Color::BLACK);
  EXPECT_EQ(Piece::color_from_char('n'), Color::BLACK);
  EXPECT_EQ(Piece::color_from_char('b'), Color::BLACK);
  EXPECT_EQ(Piece::color_from_char('r'), Color::BLACK);
  EXPECT_EQ(Piece::color_from_char('q'), Color::BLACK);

  // Should also work with other lowecase letters by construction
  EXPECT_EQ(Piece::color_from_char('z'), Color::BLACK);
  EXPECT_EQ(Piece::color_from_char('a'), Color::BLACK);
  EXPECT_EQ(Piece::color_from_char('k'), Color::BLACK);
}

TEST(PieceTest, ColorFromImvalidCharsThrows) {
  EXPECT_THROW(std::ignore = Piece::color_from_char('?'), std::invalid_argument);
  EXPECT_THROW(std::ignore = Piece::color_from_char('@'), std::invalid_argument);
  EXPECT_THROW(std::ignore = Piece::color_from_char('.'), std::invalid_argument);
}
