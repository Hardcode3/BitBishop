#include <gtest/gtest.h>

#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/moves/rook_move_gen.hpp>

/**
 * @brief Test fixture for rook pseudo-legal move generation.
 */
class RookPseudoLegalMovesTest : public ::testing::Test {
 protected:
  std::vector<Move> moves;

  void SetUp() override {}

  void TearDown() override {}
};

/**
 * @test Verifies White rooks have 0 moves from starting position (blocked by own pieces).
 */
TEST_F(RookPseudoLegalMovesTest, StartingPositionWhiteHas0Moves) {
  Board board;

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies Black rooks have 0 moves from starting position (blocked by own pieces).
 */
TEST_F(RookPseudoLegalMovesTest, StartingPositionBlackHas0Moves) {
  Board board;

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies White rooks have no captures from starting position.
 */
TEST_F(RookPseudoLegalMovesTest, StartingPositionWhiteHasNoCaptures) {
  Board board;

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies Black rooks have no captures from starting position.
 */
TEST_F(RookPseudoLegalMovesTest, StartingPositionBlackHasNoCaptures) {
  Board board;

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies White rook on empty board generates 14 moves (7 in each direction).
 */
TEST_F(RookPseudoLegalMovesTest, WhiteRookCenterEmptyBoardHas14Moves) {
  Board board("8/8/8/8/3R4/8/8/8 w - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 14);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies Black rook on empty board generates 14 moves (7 in each direction).
 */
TEST_F(RookPseudoLegalMovesTest, BlackRookCenterEmptyBoardHas14Moves) {
  Board board("8/8/8/8/3r4/8/8/8 b - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 14);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies White rook in corner has 14 moves on empty board.
 */
TEST_F(RookPseudoLegalMovesTest, WhiteRookInCornerHas14Moves) {
  Board board("8/8/8/8/8/8/8/R7 w - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 14);
  // Should have 7 moves north and 7 moves east
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies Black rook in corner has 14 moves on empty board.
 */
TEST_F(RookPseudoLegalMovesTest, BlackRookInCornerHas14Moves) {
  Board board("r7/8/8/8/8/8/8/8 b - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 14);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies White rook can capture enemy pieces in all four directions.
 */
TEST_F(RookPseudoLegalMovesTest, WhiteRookCanCaptureEnemyPieces) {
  Board board("8/8/8/3p4/2pRp3/3p4/8/8 w - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 4);
  EXPECT_EQ(count_captures(moves), 4);
}

/**
 * @test Verifies Black rook can capture enemy pieces in all four directions.
 */
TEST_F(RookPseudoLegalMovesTest, BlackRookCanCaptureEnemyPieces) {
  Board board("8/8/8/3P4/2PrP3/3P4/8/8 b - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 4);
  EXPECT_EQ(count_captures(moves), 4);
}

/**
 * @test Verifies White rook cannot capture own pieces.
 */
TEST_F(RookPseudoLegalMovesTest, WhiteRookCannotCaptureOwnPieces) {
  Board board("8/8/8/3P4/2PRP3/3P4/8/8 w - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies Black rook cannot capture own pieces.
 */
TEST_F(RookPseudoLegalMovesTest, BlackRookCannotCaptureOwnPieces) {
  Board board("8/8/8/3p4/2prp3/3p4/8/8 b - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies White rook with mix of blocked and available squares.
 */
TEST_F(RookPseudoLegalMovesTest, WhiteRookMixedOccupancy) {
  Board board("8/8/8/3p4/2PRp3/3P4/8/8 w - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_captures(moves), 2);
  EXPECT_EQ(moves.size(), 2);
}

/**
 * @test Verifies Black rook with mix of blocked and available squares.
 */
TEST_F(RookPseudoLegalMovesTest, BlackRookMixedOccupancy) {
  Board board("8/8/8/3P4/2prP3/3p4/8/8 b - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_captures(moves), 2);
  EXPECT_EQ(moves.size(), 2);
}

/**
 * @test Verifies White rook moves along rank are generated correctly.
 */
TEST_F(RookPseudoLegalMovesTest, WhiteRookMovesAlongRank) {
  Board board("8/8/8/8/R7/8/8/8 w - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 14);

  EXPECT_TRUE(contains_move(moves, {Squares::A4, Squares::B4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A4, Squares::H4, std::nullopt, false, false, false}));
}

/**
 * @test Verifies Black rook moves along rank are generated correctly.
 */
TEST_F(RookPseudoLegalMovesTest, BlackRookMovesAlongRank) {
  Board board("8/8/8/8/r7/8/8/8 b - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 14);
  EXPECT_TRUE(contains_move(moves, {Squares::A4, Squares::B4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A4, Squares::H4, std::nullopt, false, false, false}));
}

/**
 * @test Verifies White rook moves along file are generated correctly.
 */
TEST_F(RookPseudoLegalMovesTest, WhiteRookMovesAlongFile) {
  Board board("8/8/8/8/8/8/8/R7 w - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 14);

  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::A2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::A8, std::nullopt, false, false, false}));
}

/**
 * @test Verifies Black rook moves along file are generated correctly.
 */
TEST_F(RookPseudoLegalMovesTest, BlackRookMovesAlongFile) {
  Board board("r7/8/8/8/8/8/8/8 b - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 14);
  EXPECT_TRUE(contains_move(moves, {Squares::A8, Squares::A7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A8, Squares::A1, std::nullopt, false, false, false}));
}

/**
 * @test Verifies White rook stops at first blocker in each direction.
 */
TEST_F(RookPseudoLegalMovesTest, WhiteRookStopsAtBlocker) {
  Board board("3p4/3p4/3p4/8/pp1R1ppp/8/3p4/3p4 w - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_captures(moves), 4);

  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D6, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D2, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B4, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F4, std::nullopt, true, false, false}));
}

/**
 * @test Verifies Black rook stops at first blocker in each direction.
 */
TEST_F(RookPseudoLegalMovesTest, BlackRookStopsAtBlocker) {
  Board board("8/8/3P4/8/PP1r1PPP/8/3P4/8 b - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_captures(moves), 4);

  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D6, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D2, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B4, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F4, std::nullopt, true, false, false}));
}

/**
 * @test Verifies multiple White rooks generate moves independently.
 */
TEST_F(RookPseudoLegalMovesTest, MultipleWhiteRooksGenerateMoves) {
  Board board("8/8/8/8/R6R/8/8/8 w - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  // Each rook has 13 moves, total 26
  EXPECT_EQ(moves.size(), 26);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies multiple Black rooks generate moves independently.
 */
TEST_F(RookPseudoLegalMovesTest, MultipleBlackRooksGenerateMoves) {
  Board board("8/8/8/8/r6r/8/8/8 b - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  // Each rook has 13 moves, total 26
  EXPECT_EQ(moves.size(), 26);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies White kingside castling is available from starting position.
 */
TEST_F(RookPseudoLegalMovesTest, WhiteBothCastlingAvailable) {
  Board board("8/8/8/8/8/8/8/R3K2R w KQ - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_rook_kingside_castling(moves, Color::WHITE), 1);
  EXPECT_EQ(count_rook_queenside_castling(moves, Color::WHITE), 1);
}

/**
 * @test Verifies Black kingside castling is available from starting position.
 */
TEST_F(RookPseudoLegalMovesTest, BlackBothCastlingAvailable) {
  Board board("r3k2r/8/8/8/8/8/8/8 b kq - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_rook_kingside_castling(moves, Color::BLACK), 1);
  EXPECT_EQ(count_rook_queenside_castling(moves, Color::BLACK), 1);
}

/**
 * @test Verifies White castling unavailable when castling rights are lost.
 */
TEST_F(RookPseudoLegalMovesTest, WhiteCastlingUnavailableWithoutRights) {
  Board board("8/8/8/8/8/8/8/R3K2R w - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_rook_kingside_castling(moves, Color::WHITE), 0);
  EXPECT_EQ(count_rook_queenside_castling(moves, Color::WHITE), 0);
}

/**
 * @test Verifies Black castling unavailable when castling rights are lost.
 */
TEST_F(RookPseudoLegalMovesTest, BlackCastlingUnavailableWithoutRights) {
  Board board("r3k2r/8/8/8/8/8/8/8 b - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_rook_kingside_castling(moves, Color::BLACK), 0);
  EXPECT_EQ(count_rook_queenside_castling(moves, Color::BLACK), 0);
}

/**
 * @test Verifies White can only castle kingside when queenside rights are lost.
 */
TEST_F(RookPseudoLegalMovesTest, WhiteOnlyKingsideCastlingAvailable) {
  Board board("8/8/8/8/8/8/8/R3K2R w K - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_rook_kingside_castling(moves, Color::WHITE), 1);
  EXPECT_EQ(count_rook_queenside_castling(moves, Color::WHITE), 0);
}

/**
 * @test Verifies White can only castle queenside when kingside rights are lost.
 */
TEST_F(RookPseudoLegalMovesTest, WhiteOnlyQueensideCastlingAvailable) {
  Board board("8/8/8/8/8/8/8/R3K2R w Q - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_rook_kingside_castling(moves, Color::WHITE), 0);
  EXPECT_EQ(count_rook_queenside_castling(moves, Color::WHITE), 1);
}

/**
 * @test Verifies Black can only castle kingside when queenside rights are lost.
 */
TEST_F(RookPseudoLegalMovesTest, BlackOnlyKingsideCastlingAvailable) {
  Board board("r3k2r/8/8/8/8/8/8/8 b k - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_rook_kingside_castling(moves, Color::BLACK), 1);
  EXPECT_EQ(count_rook_queenside_castling(moves, Color::BLACK), 0);
}

/**
 * @test Verifies Black can only castle queenside when kingside rights are lost.
 */
TEST_F(RookPseudoLegalMovesTest, BlackOnlyQueensideCastlingAvailable) {
  Board board("r3k2r/8/8/8/8/8/8/8 b q - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_rook_kingside_castling(moves, Color::BLACK), 0);
  EXPECT_EQ(count_rook_queenside_castling(moves, Color::BLACK), 1);
}

/**
 * @test Verifies White castling blocked when pieces obstruct the path.
 */
TEST_F(RookPseudoLegalMovesTest, WhiteCastlingBlockedByPieces) {
  Board board("8/8/8/8/8/8/8/R1B1KN1R w KQ - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_rook_kingside_castling(moves, Color::WHITE), 0);
  EXPECT_EQ(count_rook_queenside_castling(moves, Color::WHITE), 0);
}

/**
 * @test Verifies Black castling blocked when pieces obstruct the path.
 */
TEST_F(RookPseudoLegalMovesTest, BlackCastlingBlockedByPieces) {
  Board board("r1b1kn1r/8/8/8/8/8/8/8 b kq - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_rook_kingside_castling(moves, Color::BLACK), 0);
  EXPECT_EQ(count_rook_queenside_castling(moves, Color::BLACK), 0);
}

/**
 * @test Verifies all rook moves have no promotion flag.
 */
TEST_F(RookPseudoLegalMovesTest, AllMovesHaveNoPromotionFlag) {
  Board board("8/8/8/8/3R4/8/8/8 w - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_promotions(moves), 0);
}

/**
 * @test Verifies all rook moves have no en passant flag.
 */
TEST_F(RookPseudoLegalMovesTest, AllMovesHaveNoEnPassantFlag) {
  Board board("8/8/8/8/3R4/8/8/8 w - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_en_passant) << "Rook moves should never be en passant";
  }
}

/**
 * @test Verifies only castling moves have castling flag set.
 */
TEST_F(RookPseudoLegalMovesTest, OnlyCastlingMovesHaveCastlingFlag) {
  Board board("8/8/8/8/8/8/8/R3K2R w KQ - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  for (const auto& move : moves) {
    if (move.is_castling) {
      EXPECT_TRUE(move.is_castling);
    } else {
      EXPECT_FALSE(move.is_castling);
    }
  }
}

/**
 * @test Verifies complex position with captures, regular moves, and castling.
 */
TEST_F(RookPseudoLegalMovesTest, ComplexPositionMixedMoves) {
  Board board("8/8/3p4/8/ppppRppp/8/4P3/8 b - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 7);
  EXPECT_EQ(count_captures(moves), 2);
  EXPECT_EQ(count_quiet_moves(moves), 5);
}

/**
 * @test Verifies rook with long-range captures.
 */
TEST_F(RookPseudoLegalMovesTest, RookLongRangeCapture) {
  Board board("8/8/8/8/R6p/8/8/8 w - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  // Should be able to capture on H4
  EXPECT_TRUE(contains_move(moves, {Squares::A4, Squares::H4, std::nullopt, true, false, false}));
}

/**
 * @test Verifies no moves generated when no rooks present.
 */
TEST_F(RookPseudoLegalMovesTest, NoMovesWhenNoRooks) {
  Board board("8/8/8/8/8/8/8/4K3 w - - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies castling moves have correct properties for White kingside.
 */
TEST_F(RookPseudoLegalMovesTest, WhiteKingsideCastlingCorrectProperties) {
  Board board("8/8/8/8/8/8/8/R3K2R w KQ - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  bool found_kingside = false;
  for (const auto& move : moves) {
    if (move.is_castling && move.from == Squares::H1 && move.to == Squares::F1) {
      EXPECT_FALSE(move.is_capture);
      EXPECT_FALSE(move.is_en_passant);
      EXPECT_FALSE(move.promotion.has_value());
      found_kingside = true;
    }
  }
  EXPECT_TRUE(found_kingside) << "White kingside castling move should be present";
}

/**
 * @test Verifies castling moves have correct properties for White queenside.
 */
TEST_F(RookPseudoLegalMovesTest, WhiteQueensideCastlingCorrectProperties) {
  Board board("8/8/8/8/8/8/8/R3K2R w KQ - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  bool found_queenside = false;
  for (const auto& move : moves) {
    if (move.is_castling && move.from == Squares::A1 && move.to == Squares::D1) {
      EXPECT_FALSE(move.is_capture);
      EXPECT_FALSE(move.is_en_passant);
      EXPECT_FALSE(move.promotion.has_value());
      found_queenside = true;
    }
  }
  EXPECT_TRUE(found_queenside) << "White queenside castling move should be present";
}

/**
 * @test Verifies castling moves have correct properties for Black kingside.
 */
TEST_F(RookPseudoLegalMovesTest, BlackKingsideCastlingCorrectProperties) {
  Board board("r3k2r/8/8/8/8/8/8/8 b kq - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  bool found_kingside = false;
  for (const auto& move : moves) {
    if (move.is_castling && move.from == Squares::H8 && move.to == Squares::F8) {
      EXPECT_FALSE(move.is_capture);
      EXPECT_FALSE(move.is_en_passant);
      EXPECT_FALSE(move.promotion.has_value());
      found_kingside = true;
    }
  }
  EXPECT_TRUE(found_kingside) << "Black kingside castling move should be present";
}

/**
 * @test Verifies castling moves have correct properties for Black queenside.
 */
TEST_F(RookPseudoLegalMovesTest, BlackQueensideCastlingCorrectProperties) {
  Board board("r3k2r/8/8/8/8/8/8/8 b kq - 0 1");

  RookMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  bool found_queenside = false;
  for (const auto& move : moves) {
    if (move.is_castling && move.from == Squares::A8 && move.to == Squares::D8) {
      EXPECT_FALSE(move.is_capture);
      EXPECT_FALSE(move.is_en_passant);
      EXPECT_FALSE(move.promotion.has_value());
      found_queenside = true;
    }
  }
  EXPECT_TRUE(found_queenside) << "Black queenside castling move should be present";
}
