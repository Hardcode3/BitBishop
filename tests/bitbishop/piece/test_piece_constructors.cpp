#include <gtest/gtest.h>

#include <bitbishop/piece.hpp>

/**
 * @test Verifies default constructor creates NONE piece.
 */
TEST(PieceTest, DefaultConstructorCreatesNonePiece) {
  Piece p;

  EXPECT_TRUE(p.is_none());
  EXPECT_EQ(p.type(), Piece::NONE);
  EXPECT_EQ(p.to_char(), '.');
}

/**
 * @test Verifies construction from Type and Color.
 */
TEST(PieceTest, ConstructFromTypeAndColor) {
  Piece white_pawn(Piece::PAWN, Color::WHITE);
  Piece black_knight(Piece::KNIGHT, Color::BLACK);

  EXPECT_EQ(white_pawn.type(), Piece::PAWN);
  EXPECT_TRUE(white_pawn.is_white());

  EXPECT_EQ(black_knight.type(), Piece::KNIGHT);
  EXPECT_TRUE(black_knight.is_black());
}

/**
 * @test Verifies White pawn constructed from 'P'.
 */
TEST(PieceTest, ConstructWhitePawnFromChar) {
  Piece p('P');

  EXPECT_EQ(p.type(), Piece::PAWN);
  EXPECT_TRUE(p.is_white());
  EXPECT_FALSE(p.is_black());
  EXPECT_FALSE(p.is_none());
  EXPECT_EQ(p.to_char(), 'P');
}

/**
 * @test Verifies White knight constructed from 'N'.
 */
TEST(PieceTest, ConstructWhiteKnightFromChar) {
  Piece p('N');

  EXPECT_EQ(p.type(), Piece::KNIGHT);
  EXPECT_TRUE(p.is_white());
  EXPECT_EQ(p.to_char(), 'N');
}

/**
 * @test Verifies White bishop constructed from 'B'.
 */
TEST(PieceTest, ConstructWhiteBishopFromChar) {
  Piece p('B');

  EXPECT_EQ(p.type(), Piece::BISHOP);
  EXPECT_TRUE(p.is_white());
  EXPECT_EQ(p.to_char(), 'B');
}

/**
 * @test Verifies White rook constructed from 'R'.
 */
TEST(PieceTest, ConstructWhiteRookFromChar) {
  Piece p('R');

  EXPECT_EQ(p.type(), Piece::ROOK);
  EXPECT_TRUE(p.is_white());
  EXPECT_EQ(p.to_char(), 'R');
}

/**
 * @test Verifies White queen constructed from 'Q'.
 */
TEST(PieceTest, ConstructWhiteQueenFromChar) {
  Piece p('Q');

  EXPECT_EQ(p.type(), Piece::QUEEN);
  EXPECT_TRUE(p.is_white());
  EXPECT_EQ(p.to_char(), 'Q');
}

/**
 * @test Verifies White king constructed from 'K'.
 */
TEST(PieceTest, ConstructWhiteKingFromChar) {
  Piece p('K');

  EXPECT_EQ(p.type(), Piece::KING);
  EXPECT_TRUE(p.is_white());
  EXPECT_EQ(p.to_char(), 'K');
}

/**
 * @test Verifies Black pawn constructed from 'p'.
 */
TEST(PieceTest, ConstructBlackPawnFromChar) {
  Piece p('p');

  EXPECT_EQ(p.type(), Piece::PAWN);
  EXPECT_TRUE(p.is_black());
  EXPECT_FALSE(p.is_white());
  EXPECT_FALSE(p.is_none());
  EXPECT_EQ(p.to_char(), 'p');
}

/**
 * @test Verifies Black knight constructed from 'n'.
 */
TEST(PieceTest, ConstructBlackKnightFromChar) {
  Piece p('n');

  EXPECT_EQ(p.type(), Piece::KNIGHT);
  EXPECT_TRUE(p.is_black());
  EXPECT_EQ(p.to_char(), 'n');
}

/**
 * @test Verifies Black bishop constructed from 'b'.
 */
TEST(PieceTest, ConstructBlackBishopFromChar) {
  Piece p('b');

  EXPECT_EQ(p.type(), Piece::BISHOP);
  EXPECT_TRUE(p.is_black());
  EXPECT_EQ(p.to_char(), 'b');
}

/**
 * @test Verifies Black rook constructed from 'r'.
 */
TEST(PieceTest, ConstructBlackRookFromChar) {
  Piece p('r');

  EXPECT_EQ(p.type(), Piece::ROOK);
  EXPECT_TRUE(p.is_black());
  EXPECT_EQ(p.to_char(), 'r');
}

/**
 * @test Verifies Black queen constructed from 'q'.
 */
TEST(PieceTest, ConstructBlackQueenFromChar) {
  Piece p('q');

  EXPECT_EQ(p.type(), Piece::QUEEN);
  EXPECT_TRUE(p.is_black());
  EXPECT_EQ(p.to_char(), 'q');
}

/**
 * @test Verifies Black king constructed from 'k'.
 */
TEST(PieceTest, ConstructBlackKingFromChar) {
  Piece p('k');

  EXPECT_EQ(p.type(), Piece::KING);
  EXPECT_TRUE(p.is_black());
  EXPECT_EQ(p.to_char(), 'k');
}

/**
 * @test Verifies NONE piece constructed from '.'.
 */
TEST(PieceTest, ConstructNonePieceFromChar) {
  Piece p('.');

  EXPECT_EQ(p.type(), Piece::NONE);
  EXPECT_TRUE(p.is_none());
  EXPECT_TRUE(p.is_white());
  EXPECT_FALSE(p.is_black());
  EXPECT_EQ(p.to_char(), '.');
}

/**
 * @test Verifies invalid character throws exception.
 */
TEST(PieceTest, InvalidCharThrowsException) {
  EXPECT_THROW(Piece('X'), std::invalid_argument);
  EXPECT_THROW(Piece('5'), std::invalid_argument);
  EXPECT_THROW(Piece(' '), std::invalid_argument);
  EXPECT_THROW(Piece('@'), std::invalid_argument);
}

/**
 * @test Verifies constructing from Type and Color sets correct properties.
 */
TEST(PieceTest, TypeColorConstructorSetsCorrectProperties) {
  Piece white_queen(Piece::QUEEN, Color::WHITE);

  EXPECT_EQ(white_queen.type(), Piece::QUEEN);
  EXPECT_EQ(white_queen.color(), Color::WHITE);
  EXPECT_TRUE(white_queen.is_white());
  EXPECT_FALSE(white_queen.is_black());
  EXPECT_FALSE(white_queen.is_none());
}
