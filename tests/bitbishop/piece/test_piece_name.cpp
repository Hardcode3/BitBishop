#include <gtest/gtest.h>

#include <bitbishop/piece.hpp>

using namespace Pieces;

TEST(PieceTest, NameFromTypeHasCorrectName) {
  struct TestCase {
    Piece::Type type;
    const char* expected;
  };
  TestCase cases[] = {{Piece::PAWN, "pawn"}, {Piece::KNIGHT, "knight"}, {Piece::BISHOP, "bishop"},
                      {Piece::ROOK, "rook"}, {Piece::QUEEN, "queen"},   {Piece::KING, "king"}};

  for (const auto& [type, expected] : cases) {
    EXPECT_STREQ(Piece::name(type), expected);
  }
}

TEST(PieceTest, NameFromTypeHasUnknownType) {
  Piece::Type invalid_type = static_cast<Piece::Type>(155);
  EXPECT_STREQ(Piece::name(invalid_type), "unknown");
}
