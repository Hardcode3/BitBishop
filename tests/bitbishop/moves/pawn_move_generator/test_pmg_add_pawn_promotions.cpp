#include <gtest/gtest.h>

#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/moves/pawn_move_gen.hpp>

/**
 * @brief Test fixture for pawn promotion move generation.
 *
 * Provides a clean move vector for each test case.
 */
class PawnPromotionTest : public ::testing::Test {
 protected:
  std::vector<Move> moves;

  void SetUp() override { moves.clear(); }

  void TearDown() override { moves.clear(); }
};

/**
 * @test Verifies White promotion generates exactly 4 moves (Q, R, B, N).
 */
TEST_F(PawnPromotionTest, WhitePromotionAdds4Moves) {
  Square from = Squares::E7;
  Square to = Squares::E8;

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, false);

  EXPECT_EQ(moves.size(), 4);
}

/**
 * @test Verifies White promotion includes all four piece types.
 */
TEST_F(PawnPromotionTest, WhitePromotionContainsAllPieces) {
  Square from = Squares::E7;
  Square to = Squares::E8;

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, false);

  ASSERT_EQ(moves.size(), 4);

  EXPECT_EQ(count_promotions_to(moves, Pieces::WHITE_QUEEN), 1);
  EXPECT_EQ(count_promotions_to(moves, Pieces::WHITE_ROOK), 1);
  EXPECT_EQ(count_promotions_to(moves, Pieces::WHITE_BISHOP), 1);
  EXPECT_EQ(count_promotions_to(moves, Pieces::WHITE_KNIGHT), 1);
}

/**
 * @test Verifies White promotion generates correct move objects for each piece type.
 */
TEST_F(PawnPromotionTest, WhitePromotionContainsSpecificMoves) {
  Square from = Squares::E7;
  Square to = Squares::E8;

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, false);

  Move queen_promo = {from, to, Pieces::WHITE_QUEEN, false, false, false};
  Move rook_promo = {from, to, Pieces::WHITE_ROOK, false, false, false};
  Move bishop_promo = {from, to, Pieces::WHITE_BISHOP, false, false, false};
  Move knight_promo = {from, to, Pieces::WHITE_KNIGHT, false, false, false};

  EXPECT_TRUE(contains_move(moves, queen_promo));
  EXPECT_TRUE(contains_move(moves, rook_promo));
  EXPECT_TRUE(contains_move(moves, bishop_promo));
  EXPECT_TRUE(contains_move(moves, knight_promo));
}

/**
 * @test Verifies all White promotion moves have correct source and destination squares.
 */
TEST_F(PawnPromotionTest, WhitePromotionHasCorrectSquares) {
  Square from = Squares::A7;
  Square to = Squares::A8;

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, false);

  ASSERT_EQ(moves.size(), 4);

  for (const auto& move : moves) {
    EXPECT_EQ(move.from, from);
    EXPECT_EQ(move.to, to);
  }
}

/**
 * @test Verifies White non-capture promotions have correct flags.
 */
TEST_F(PawnPromotionTest, WhitePromotionNonCaptureFlags) {
  Square from = Squares::E7;
  Square to = Squares::E8;

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, false);

  ASSERT_EQ(moves.size(), 4);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_capture);
    EXPECT_FALSE(move.is_en_passant);
    EXPECT_FALSE(move.is_castling);
    EXPECT_TRUE(move.promotion.has_value());
  }
}

/**
 * @test Verifies White capture promotions have is_capture flag set.
 */
TEST_F(PawnPromotionTest, WhitePromotionCaptureFlags) {
  Square from = Squares::E7;
  Square to = Squares::F8;

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, true);

  ASSERT_EQ(moves.size(), 4);

  for (const auto& move : moves) {
    EXPECT_TRUE(move.is_capture);
    EXPECT_FALSE(move.is_en_passant);
    EXPECT_FALSE(move.is_castling);
    EXPECT_TRUE(move.promotion.has_value());
  }
}

/**
 * @test Verifies White capture promotions generate correct move objects.
 */
TEST_F(PawnPromotionTest, WhitePromotionCaptureContainsSpecificMoves) {
  Square from = Squares::E7;
  Square to = Squares::F8;

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, true);

  Move queen_promo = {from, to, Pieces::WHITE_QUEEN, true, false, false};
  Move rook_promo = {from, to, Pieces::WHITE_ROOK, true, false, false};
  Move bishop_promo = {from, to, Pieces::WHITE_BISHOP, true, false, false};
  Move knight_promo = {from, to, Pieces::WHITE_KNIGHT, true, false, false};

  EXPECT_TRUE(contains_move(moves, queen_promo));
  EXPECT_TRUE(contains_move(moves, rook_promo));
  EXPECT_TRUE(contains_move(moves, bishop_promo));
  EXPECT_TRUE(contains_move(moves, knight_promo));
}

/**
 * @test Verifies Black promotion generates exactly 4 moves (q, r, b, n).
 */
TEST_F(PawnPromotionTest, BlackPromotionAdds4Moves) {
  Square from = Squares::A2;
  Square to = Squares::A1;

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::BLACK, false);

  EXPECT_EQ(moves.size(), 4);
}

/**
 * @test Verifies Black promotion includes all four piece types.
 */
TEST_F(PawnPromotionTest, BlackPromotionContainsAllPieces) {
  Square from = Squares::A2;
  Square to = Squares::A1;

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::BLACK, false);

  ASSERT_EQ(moves.size(), 4);

  EXPECT_EQ(count_promotions_to(moves, Pieces::BLACK_QUEEN), 1);
  EXPECT_EQ(count_promotions_to(moves, Pieces::BLACK_ROOK), 1);
  EXPECT_EQ(count_promotions_to(moves, Pieces::BLACK_BISHOP), 1);
  EXPECT_EQ(count_promotions_to(moves, Pieces::BLACK_KNIGHT), 1);
}

/**
 * @test Verifies Black promotion generates correct move objects for each piece type.
 */
TEST_F(PawnPromotionTest, BlackPromotionContainsSpecificMoves) {
  Square from = Squares::E2;
  Square to = Squares::E1;

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::BLACK, false);

  Move queen_promo = {from, to, Pieces::BLACK_QUEEN, false, false, false};
  Move rook_promo = {from, to, Pieces::BLACK_ROOK, false, false, false};
  Move bishop_promo = {from, to, Pieces::BLACK_BISHOP, false, false, false};
  Move knight_promo = {from, to, Pieces::BLACK_KNIGHT, false, false, false};

  EXPECT_TRUE(contains_move(moves, queen_promo));
  EXPECT_TRUE(contains_move(moves, rook_promo));
  EXPECT_TRUE(contains_move(moves, bishop_promo));
  EXPECT_TRUE(contains_move(moves, knight_promo));
}

/**
 * @test Verifies all Black promotion moves have correct source and destination squares.
 */
TEST_F(PawnPromotionTest, BlackPromotionHasCorrectSquares) {
  Square from = Squares::H2;
  Square to = Squares::H1;

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::BLACK, false);

  ASSERT_EQ(moves.size(), 4);

  for (const auto& move : moves) {
    EXPECT_EQ(move.from, from);
    EXPECT_EQ(move.to, to);
  }
}

/**
 * @test Verifies Black non-capture promotions have correct flags.
 */
TEST_F(PawnPromotionTest, BlackPromotionNonCaptureFlags) {
  Square from = Squares::E2;
  Square to = Squares::E1;

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::BLACK, false);

  ASSERT_EQ(moves.size(), 4);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_capture);
    EXPECT_FALSE(move.is_en_passant);
    EXPECT_FALSE(move.is_castling);
    EXPECT_TRUE(move.promotion.has_value());
  }
}

/**
 * @test Verifies Black capture promotions have is_capture flag set.
 */
TEST_F(PawnPromotionTest, BlackPromotionCaptureFlags) {
  Square from = Squares::E2;
  Square to = Squares::D1;

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::BLACK, true);

  ASSERT_EQ(moves.size(), 4);

  for (const auto& move : moves) {
    EXPECT_TRUE(move.is_capture);
    EXPECT_FALSE(move.is_en_passant);
    EXPECT_FALSE(move.is_castling);
    EXPECT_TRUE(move.promotion.has_value());
  }
}

/**
 * @test Verifies Black capture promotions generate correct move objects.
 */
TEST_F(PawnPromotionTest, BlackPromotionCaptureContainsSpecificMoves) {
  Square from = Squares::E2;
  Square to = Squares::D1;

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::BLACK, true);

  Move queen_promo = {from, to, Pieces::BLACK_QUEEN, true, false, false};
  Move rook_promo = {from, to, Pieces::BLACK_ROOK, true, false, false};
  Move bishop_promo = {from, to, Pieces::BLACK_BISHOP, true, false, false};
  Move knight_promo = {from, to, Pieces::BLACK_KNIGHT, true, false, false};

  EXPECT_TRUE(contains_move(moves, queen_promo));
  EXPECT_TRUE(contains_move(moves, rook_promo));
  EXPECT_TRUE(contains_move(moves, bishop_promo));
  EXPECT_TRUE(contains_move(moves, knight_promo));
}

/**
 * @test Verifies promotions are appended to existing moves without clearing the vector.
 */
TEST_F(PawnPromotionTest, PromotionAppendsToExistingMoves) {
  moves.push_back({Squares::E2, Squares::E4, std::nullopt, false, false, false});
  moves.push_back({Squares::D2, Squares::D4, std::nullopt, false, false, false});

  size_t initial_size = moves.size();

  PawnMoveGenerator::add_pawn_promotions(moves, Squares::E7, Squares::E8, Color::WHITE, false);

  EXPECT_EQ(moves.size(), initial_size + 4);
}

/**
 * @test Verifies multiple promotion calls accumulate correctly (3 × 4 = 12 moves).
 */
TEST_F(PawnPromotionTest, MultiplePromotionCalls) {
  PawnMoveGenerator::add_pawn_promotions(moves, Squares::E7, Squares::E8, Color::WHITE, false);
  PawnMoveGenerator::add_pawn_promotions(moves, Squares::F7, Squares::G8, Color::WHITE, true);
  PawnMoveGenerator::add_pawn_promotions(moves, Squares::E2, Squares::E1, Color::BLACK, false);

  EXPECT_EQ(moves.size(), 12);
}

/**
 * @test Verifies promotions work correctly for various square combinations.
 */
TEST_F(PawnPromotionTest, PromotionWithDifferentSquareCombinations) {
  std::vector<std::pair<Square, Square>> white_test_cases = {
      {Squares::A7, Squares::A8}, {Squares::B7, Squares::B8}, {Squares::H7, Squares::H8}, {Squares::D7, Squares::E8}  // capture
  };

  for (const auto& [from, to] : white_test_cases) {
    std::vector<Move> temp_moves;
    PawnMoveGenerator::add_pawn_promotions(temp_moves, from, to, Color::WHITE, false);
    EXPECT_EQ(temp_moves.size(), 4);
  }

  std::vector<std::pair<Square, Square>> black_test_cases = {
      {Squares::A2, Squares::A1}, {Squares::E2, Squares::E1}, {Squares::H2, Squares::H1}, {Squares::F2, Squares::E1}  // capture
  };

  for (const auto& [from, to] : black_test_cases) {
    std::vector<Move> temp_moves;
    PawnMoveGenerator::add_pawn_promotions(temp_moves, from, to, Color::BLACK, false);
    EXPECT_EQ(temp_moves.size(), 4);
  }
}

/**
 * @test Verifies all four promotion moves share the same source square.
 */
TEST_F(PawnPromotionTest, AllPromotionMovesHaveSameFromSquare) {
  Square from = Squares::E7;
  Square to = Squares::E8;

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, false);

  for (const auto& move : moves) {
    EXPECT_EQ(move.from, from);
  }
}

/**
 * @test Verifies all four promotion moves share the same destination square.
 */
TEST_F(PawnPromotionTest, AllPromotionMovesHaveSameToSquare) {
  Square from = Squares::E7;
  Square to = Squares::E8;

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, false);

  for (const auto& move : moves) {
    EXPECT_EQ(move.to, to);
  }
}

/**
 * @test Verifies all four promotion moves share the same capture flag value.
 */
TEST_F(PawnPromotionTest, AllPromotionMovesHaveSameCaptureFlag) {
  Square from = Squares::E7;
  Square to = Squares::E8;

  PawnMoveGenerator::add_pawn_promotions(moves, from, to, Color::WHITE, false);
  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_capture);
  }

    moves.clear();

    Square cap_from = Squares::E7;
    Square cap_to = Squares::F8;
    PawnMoveGenerator::add_pawn_promotions(moves, cap_from, cap_to, Color::WHITE, true);
    for (const auto& move : moves) {
      EXPECT_TRUE(move.is_capture);
    }
}

/**
 * @test Verifies promotion moves never have en passant flag set.
 */
TEST_F(PawnPromotionTest, NoPromotionMovesAreEnPassant) {
  PawnMoveGenerator::add_pawn_promotions(moves, Squares::E7, Squares::E8, Color::WHITE, false);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_en_passant);
  }
}

/**
 * @test Verifies promotion moves never have castling flag set.
 */
TEST_F(PawnPromotionTest, NoPromotionMovesAreCastles) {
  PawnMoveGenerator::add_pawn_promotions(moves, Squares::E7, Squares::E8, Color::WHITE, false);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_castling);
  }
}

/**
 * @test Verifies all promotion moves have a promotion piece specified.
 */
TEST_F(PawnPromotionTest, AllPromotionMovesHavePromotionPiece) {
  PawnMoveGenerator::add_pawn_promotions(moves, Squares::E7, Squares::E8, Color::WHITE, false);

  for (const auto& move : moves) {
    EXPECT_TRUE(move.promotion.has_value()) << "All promotion moves must have a promotion piece";
  }
}
