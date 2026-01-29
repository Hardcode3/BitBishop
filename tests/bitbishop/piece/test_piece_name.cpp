#include <gtest/gtest.h>

#include <bitbishop/piece.hpp>

using namespace Pieces;

TEST(PieceTest, NameFromTypeHasCorrectName) {
  EXPECT_EQ(Piece::name(Piece::PAWN), "pawn");
  EXPECT_EQ(Piece::name(Piece::KNIGHT), "knight");
  EXPECT_EQ(Piece::name(Piece::BISHOP), "bishop");
  EXPECT_EQ(Piece::name(Piece::ROOK), "rook");
  EXPECT_EQ(Piece::name(Piece::QUEEN), "queen");
  EXPECT_EQ(Piece::name(Piece::KING), "king");
}
