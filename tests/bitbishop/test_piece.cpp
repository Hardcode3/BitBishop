#include <gtest/gtest.h>

#include <bitbishop/piece.hpp>
#include <stdexcept>

TEST(PieceTest, DefaultConstructor) {
  Piece p;
  EXPECT_TRUE(p.is_none());
  EXPECT_FALSE(p.is_white());
  EXPECT_FALSE(p.is_black());
  EXPECT_EQ(p.to_char(), '.');
}

TEST(PieceTest, CharConstructorWhitePieces) {
  EXPECT_EQ(Piece('P').type(), Piece::P);
  EXPECT_TRUE(Piece('P').is_white());
  EXPECT_FALSE(Piece('P').is_black());

  EXPECT_EQ(Piece('N').type(), Piece::N);
  EXPECT_EQ(Piece('B').type(), Piece::B);
  EXPECT_EQ(Piece('R').type(), Piece::R);
  EXPECT_EQ(Piece('Q').type(), Piece::Q);
  EXPECT_EQ(Piece('K').type(), Piece::K);
}

TEST(PieceTest, CharConstructorBlackPieces) {
  EXPECT_EQ(Piece('p').type(), Piece::p);
  EXPECT_FALSE(Piece('p').is_white());
  EXPECT_TRUE(Piece('p').is_black());

  EXPECT_EQ(Piece('n').type(), Piece::n);
  EXPECT_EQ(Piece('b').type(), Piece::b);
  EXPECT_EQ(Piece('r').type(), Piece::r);
  EXPECT_EQ(Piece('q').type(), Piece::q);
  EXPECT_EQ(Piece('k').type(), Piece::k);
}

TEST(PieceTest, CharConstructorNoPiece) {
  Piece empty('.');
  EXPECT_TRUE(empty.is_none());
  EXPECT_EQ(empty.to_char(), '.');
}

TEST(PieceTest, ToChar) {
  EXPECT_EQ(Piece('P').to_char(), 'P');
  EXPECT_EQ(Piece('n').to_char(), 'n');
  EXPECT_EQ(Piece('.').to_char(), '.');
}

TEST(PieceTest, EqualityOperators) {
  Piece p1('P');
  Piece p2('P');
  Piece p3('N');

  EXPECT_TRUE(p1 == p2);
  EXPECT_FALSE(p1 != p2);

  EXPECT_TRUE(p1 != p3);
  EXPECT_FALSE(p1 == p3);
}

TEST(PieceTest, InvalidCharThrows) {
  EXPECT_THROW(Piece('x'), std::invalid_argument);
  EXPECT_THROW(Piece('1'), std::invalid_argument);
}