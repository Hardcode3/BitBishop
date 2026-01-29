#include <gtest/gtest.h>

#include <bitbishop/move.hpp>
#include <regex>

using namespace Squares;
using namespace Pieces;

static const std::regex uci_move_regex("^[a-z1-8]{4,5}$");

TEST(MoveTest, NormalMove_NoCapture) {
  Square from = E2;
  Square to = E4;

  Move m = Move::make(from, to);

  EXPECT_EQ(m.from, from);
  EXPECT_EQ(m.to, to);
  EXPECT_FALSE(m.promotion.has_value());
  EXPECT_FALSE(m.is_capture);
  EXPECT_FALSE(m.is_en_passant);
  EXPECT_FALSE(m.is_castling);
}

TEST(MoveTest, NormalMove_WithCapture) {
  Square from = D4;
  Square to = E5;

  Move m = Move::make(from, to, true);

  EXPECT_EQ(m.from, from);
  EXPECT_EQ(m.to, to);
  EXPECT_FALSE(m.promotion.has_value());
  EXPECT_TRUE(m.is_capture);
  EXPECT_FALSE(m.is_en_passant);
  EXPECT_FALSE(m.is_castling);
}

TEST(MoveTest, PromotionMove_NoCapture) {
  Square from = E7;
  Square to = E8;

  Move m = Move::make_promotion(from, to, WHITE_QUEEN);

  EXPECT_EQ(m.from, from);
  EXPECT_EQ(m.to, to);
  ASSERT_TRUE(m.promotion.has_value());
  EXPECT_EQ(m.promotion.value(), WHITE_QUEEN);
  EXPECT_FALSE(m.is_capture);
  EXPECT_FALSE(m.is_en_passant);
  EXPECT_FALSE(m.is_castling);
}

TEST(MoveTest, PromotionMove_WithCapture) {
  Square from = D7;
  Square to = E8;

  Move m = Move::make_promotion(from, to, BLACK_KNIGHT, true);

  EXPECT_EQ(m.from, from);
  EXPECT_EQ(m.to, to);
  ASSERT_TRUE(m.promotion.has_value());
  EXPECT_EQ(m.promotion.value(), BLACK_KNIGHT);
  EXPECT_TRUE(m.is_capture);
  EXPECT_FALSE(m.is_en_passant);
  EXPECT_FALSE(m.is_castling);
}

TEST(MoveTest, EnPassantMove) {
  Square from = E5;
  Square to = D6;

  Move m = Move::make_en_passant(from, to);

  EXPECT_EQ(m.from, from);
  EXPECT_EQ(m.to, to);
  EXPECT_FALSE(m.promotion.has_value());
  EXPECT_TRUE(m.is_capture);
  EXPECT_TRUE(m.is_en_passant);
  EXPECT_FALSE(m.is_castling);
}

TEST(MoveTest, CastlingMove) {
  Square from = E1;
  Square to = G1;  // kingside castling

  Move m = Move::make_castling(from, to);

  EXPECT_EQ(m.from, from);
  EXPECT_EQ(m.to, to);
  EXPECT_FALSE(m.promotion.has_value());
  EXPECT_FALSE(m.is_capture);
  EXPECT_FALSE(m.is_en_passant);
  EXPECT_TRUE(m.is_castling);
}

TEST(MoveTest, ToUciStringRegularMove) {
  Move m = Move::make(D4, E5);

  const std::string res = m.to_uci();

  EXPECT_TRUE(std::regex_match(res, uci_move_regex));
  EXPECT_EQ(res, "d4e5");
}

TEST(MoveTest, ToUciStringQueenPromotion) {
  Move m = Move::make_promotion(E7, E8, WHITE_QUEEN, false);

  const std::string res = m.to_uci();

  EXPECT_TRUE(std::regex_match(res, uci_move_regex));
  EXPECT_EQ(res, "e7e8q");
}

TEST(MoveTest, ToUciStringRookPromotion) {
  Move m = Move::make_promotion(E7, E8, WHITE_ROOK, false);

  const std::string res = m.to_uci();

  EXPECT_TRUE(std::regex_match(res, uci_move_regex));
  EXPECT_EQ(res, "e7e8r");
}

TEST(MoveTest, ToUciStringBishopPromotion) {
  Move m = Move::make_promotion(E7, E8, WHITE_BISHOP, false);

  const std::string res = m.to_uci();

  EXPECT_TRUE(std::regex_match(res, uci_move_regex));
  EXPECT_EQ(res, "e7e8b");
}

TEST(MoveTest, ToUciStringKnightPromotion) {
  Move m = Move::make_promotion(E7, E8, WHITE_KNIGHT, false);

  const std::string res = m.to_uci();

  EXPECT_TRUE(std::regex_match(res, uci_move_regex));
  EXPECT_EQ(res, "e7e8n");
}

TEST(MoveTest, ToUciStringInvalidPromotionPieceDiscardsPromotionChar) {
  // Invalid promotion, cannot promote to a pawn
  Move m = Move::make_promotion(E7, E8, WHITE_PAWN, false);

  const std::string res = m.to_uci();

  EXPECT_TRUE(std::regex_match(res, uci_move_regex));
  EXPECT_EQ(res, "e7e8");
}
