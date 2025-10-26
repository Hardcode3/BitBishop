#include <gtest/gtest.h>

#include <chess_engine/helpers/moves.hpp>
#include <chess_engine/movegen/pawn_move_gen.hpp>

class PawnPromotionTest : public ::testing::Test {
 protected:
  std::vector<Move> moves;

  void SetUp() override { moves.clear(); }

  void TearDown() override { moves.clear(); }
};

TEST_F(PawnPromotionTest, WhitePromotionAdds4Moves) {
  Square from = Square(Square::E7);
  Square to = Square(Square::E8);

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, false);

  EXPECT_EQ(moves.size(), 4);
}

TEST_F(PawnPromotionTest, WhitePromotionContainsAllPieces) {
  Square from = Square(Square::E7);
  Square to = Square(Square::E8);

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, false);

  ASSERT_EQ(moves.size(), 4);

  EXPECT_EQ(count_promotions_to(moves, Piece('Q')), 1);
  EXPECT_EQ(count_promotions_to(moves, Piece('R')), 1);
  EXPECT_EQ(count_promotions_to(moves, Piece('B')), 1);
  EXPECT_EQ(count_promotions_to(moves, Piece('N')), 1);
}

TEST_F(PawnPromotionTest, WhitePromotionContainsSpecificMoves) {
  Square from = Square(Square::E7);
  Square to = Square(Square::E8);

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, false);

  Move queen_promo = {from, to, Piece('Q'), false, false, false};
  Move rook_promo = {from, to, Piece('R'), false, false, false};
  Move bishop_promo = {from, to, Piece('B'), false, false, false};
  Move knight_promo = {from, to, Piece('N'), false, false, false};

  EXPECT_TRUE(contains_move(moves, queen_promo));
  EXPECT_TRUE(contains_move(moves, rook_promo));
  EXPECT_TRUE(contains_move(moves, bishop_promo));
  EXPECT_TRUE(contains_move(moves, knight_promo));
}

TEST_F(PawnPromotionTest, WhitePromotionHasCorrectSquares) {
  Square from = Square(Square::A7);
  Square to = Square(Square::A8);

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, false);

  ASSERT_EQ(moves.size(), 4);

  for (const auto& move : moves) {
    EXPECT_EQ(move.from, from);
    EXPECT_EQ(move.to, to);
  }
}

TEST_F(PawnPromotionTest, WhitePromotionNonCaptureFlags) {
  Square from = Square(Square::E7);
  Square to = Square(Square::E8);

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, false);

  ASSERT_EQ(moves.size(), 4);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_capture);
    EXPECT_FALSE(move.is_en_passant);
    EXPECT_FALSE(move.is_castling);
    EXPECT_TRUE(move.promotion.has_value());
  }
}

TEST_F(PawnPromotionTest, WhitePromotionCaptureFlags) {
  Square from = Square(Square::E7);
  Square to = Square(Square::F8);

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, true);

  ASSERT_EQ(moves.size(), 4);

  for (const auto& move : moves) {
    EXPECT_TRUE(move.is_capture);
    EXPECT_FALSE(move.is_en_passant);
    EXPECT_FALSE(move.is_castling);
    EXPECT_TRUE(move.promotion.has_value());
  }
}

TEST_F(PawnPromotionTest, WhitePromotionCaptureContainsSpecificMoves) {
  Square from = Square(Square::E7);
  Square to = Square(Square::F8);

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, true);

  Move queen_promo = {from, to, Piece('Q'), true, false, false};
  Move rook_promo = {from, to, Piece('R'), true, false, false};
  Move bishop_promo = {from, to, Piece('B'), true, false, false};
  Move knight_promo = {from, to, Piece('N'), true, false, false};

  EXPECT_TRUE(contains_move(moves, queen_promo));
  EXPECT_TRUE(contains_move(moves, rook_promo));
  EXPECT_TRUE(contains_move(moves, bishop_promo));
  EXPECT_TRUE(contains_move(moves, knight_promo));
}

TEST_F(PawnPromotionTest, BlackPromotionAdds4Moves) {
  Square from = Square(Square::A2);
  Square to = Square(Square::A1);

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::BLACK, false);

  EXPECT_EQ(moves.size(), 4);
}

TEST_F(PawnPromotionTest, BlackPromotionContainsAllPieces) {
  Square from = Square(Square::A2);
  Square to = Square(Square::A1);

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::BLACK, false);

  ASSERT_EQ(moves.size(), 4);

  EXPECT_EQ(count_promotions_to(moves, Piece('q')), 1);
  EXPECT_EQ(count_promotions_to(moves, Piece('r')), 1);
  EXPECT_EQ(count_promotions_to(moves, Piece('b')), 1);
  EXPECT_EQ(count_promotions_to(moves, Piece('n')), 1);
}

TEST_F(PawnPromotionTest, BlackPromotionContainsSpecificMoves) {
  Square from = Square(Square::E2);
  Square to = Square(Square::E1);

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::BLACK, false);

  Move queen_promo = {from, to, Piece('q'), false, false, false};
  Move rook_promo = {from, to, Piece('r'), false, false, false};
  Move bishop_promo = {from, to, Piece('b'), false, false, false};
  Move knight_promo = {from, to, Piece('n'), false, false, false};

  EXPECT_TRUE(contains_move(moves, queen_promo));
  EXPECT_TRUE(contains_move(moves, rook_promo));
  EXPECT_TRUE(contains_move(moves, bishop_promo));
  EXPECT_TRUE(contains_move(moves, knight_promo));
}

TEST_F(PawnPromotionTest, BlackPromotionHasCorrectSquares) {
  Square from = Square(Square::H2);
  Square to = Square(Square::H1);

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::BLACK, false);

  ASSERT_EQ(moves.size(), 4);

  for (const auto& move : moves) {
    EXPECT_EQ(move.from, from);
    EXPECT_EQ(move.to, to);
  }
}

TEST_F(PawnPromotionTest, BlackPromotionNonCaptureFlags) {
  Square from = Square(Square::E2);
  Square to = Square(Square::E1);

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::BLACK, false);

  ASSERT_EQ(moves.size(), 4);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_capture);
    EXPECT_FALSE(move.is_en_passant);
    EXPECT_FALSE(move.is_castling);
    EXPECT_TRUE(move.promotion.has_value());
  }
}

TEST_F(PawnPromotionTest, BlackPromotionCaptureFlags) {
  Square from = Square(Square::E2);
  Square to = Square(Square::D1);

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::BLACK, true);

  ASSERT_EQ(moves.size(), 4);

  for (const auto& move : moves) {
    EXPECT_TRUE(move.is_capture);
    EXPECT_FALSE(move.is_en_passant);
    EXPECT_FALSE(move.is_castling);
    EXPECT_TRUE(move.promotion.has_value());
  }
}

TEST_F(PawnPromotionTest, BlackPromotionCaptureContainsSpecificMoves) {
  Square from = Square(Square::E2);
  Square to = Square(Square::D1);

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::BLACK, true);

  Move queen_promo = {from, to, Piece('q'), true, false, false};
  Move rook_promo = {from, to, Piece('r'), true, false, false};
  Move bishop_promo = {from, to, Piece('b'), true, false, false};
  Move knight_promo = {from, to, Piece('n'), true, false, false};

  EXPECT_TRUE(contains_move(moves, queen_promo));
  EXPECT_TRUE(contains_move(moves, rook_promo));
  EXPECT_TRUE(contains_move(moves, bishop_promo));
  EXPECT_TRUE(contains_move(moves, knight_promo));
}

TEST_F(PawnPromotionTest, PromotionAppendsToExistingMoves) {
  moves.push_back({Square(Square::E2), Square(Square::E4), std::nullopt, false, false, false});
  moves.push_back({Square(Square::D2), Square(Square::D4), std::nullopt, false, false, false});

  size_t initial_size = moves.size();

  PawnMoveGenerator::add_pawn_promotions(moves, Square(Square::E7), Square(Square::E8), Color::WHITE, false);

  EXPECT_EQ(moves.size(), initial_size + 4);
}

TEST_F(PawnPromotionTest, MultiplePromotionCalls) {
  PawnMoveGenerator::add_pawn_promotions(moves, Square(Square::E7), Square(Square::E8), Color::WHITE, false);
  PawnMoveGenerator::add_pawn_promotions(moves, Square(Square::F7), Square(Square::G8), Color::WHITE, true);
  PawnMoveGenerator::add_pawn_promotions(moves, Square(Square::E2), Square(Square::E1), Color::BLACK, false);

  // Should have 12 total moves (3 calls × 4 promotions each)
  EXPECT_EQ(moves.size(), 12);
}

TEST_F(PawnPromotionTest, PromotionWithDifferentSquareCombinations) {
  std::vector<std::pair<Square, Square>> white_test_cases = {
      {Square(Square::A7), Square(Square::A8)},
      {Square(Square::B7), Square(Square::B8)},
      {Square(Square::H7), Square(Square::H8)},
      {Square(Square::D7), Square(Square::E8)}  // capture
  };

  for (const auto& [from, to] : white_test_cases) {
    std::vector<Move> temp_moves;
    PawnMoveGenerator::add_pawn_promotions(temp_moves, from, to, Color::WHITE, false);
    EXPECT_EQ(temp_moves.size(), 4);
  }

  std::vector<std::pair<Square, Square>> black_test_cases = {
      {Square(Square::A2), Square(Square::A1)},
      {Square(Square::E2), Square(Square::E1)},
      {Square(Square::H2), Square(Square::H1)},
      {Square(Square::F2), Square(Square::E1)}  // capture
  };

  for (const auto& [from, to] : black_test_cases) {
    std::vector<Move> temp_moves;
    PawnMoveGenerator::add_pawn_promotions(temp_moves, from, to, Color::BLACK, false);
    EXPECT_EQ(temp_moves.size(), 4);
  }
}

TEST_F(PawnPromotionTest, AllPromotionMovesHaveSameFromSquare) {
  Square from = Square(Square::E7);
  Square to = Square(Square::E8);

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, false);

  for (const auto& move : moves) {
    EXPECT_EQ(move.from, from);
  }
}

TEST_F(PawnPromotionTest, AllPromotionMovesHaveSameToSquare) {
  Square from = Square(Square::E7);
  Square to = Square(Square::E8);

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, false);

  for (const auto& move : moves) {
    EXPECT_EQ(move.to, to);
  }
}

TEST_F(PawnPromotionTest, AllPromotionMovesHaveSameCaptureFlag) {
  Square from = Square(Square::E7);
  Square to = Square(Square::E8);

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, false);
  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_capture);
  }

  moves.clear();

  Square cap_from = Square(Square::E7);
  Square cap_to = Square(Square::F8);
  PawnMoveGenerator::add_pawn_promotions(moves, cap_from, cap_to, Color::WHITE, true);
  for (const auto& move : moves) {
    EXPECT_TRUE(move.is_capture);
  }
}

TEST_F(PawnPromotionTest, NoPromotionMovesAreEnPassant) {
  PawnMoveGenerator::add_pawn_promotions(moves, Square(Square::E7), Square(Square::E8), Color::WHITE, false);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_en_passant);
  }
}

TEST_F(PawnPromotionTest, NoPromotionMovesAreCastles) {
  PawnMoveGenerator::add_pawn_promotions(moves, Square(Square::E7), Square(Square::E8), Color::WHITE, false);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_castling);
  }
}

TEST_F(PawnPromotionTest, AllPromotionMovesHavePromotionPiece) {
  PawnMoveGenerator::add_pawn_promotions(moves, Square(Square::E7), Square(Square::E8), Color::WHITE, false);

  for (const auto& move : moves) {
    EXPECT_TRUE(move.promotion.has_value()) << "All promotion moves must have a promotion piece";
  }
}
