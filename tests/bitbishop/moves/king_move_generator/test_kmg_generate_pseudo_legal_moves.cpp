#include <gtest/gtest.h>

#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/moves/king_move_gen.hpp>

/**
 * @brief Test fixture for king pseudo-legal move generation.
 */
class KingPseudoLegalMovesTest : public ::testing::Test {
 protected:
  std::vector<Move> moves;

  void SetUp() override {}

  void TearDown() override {}
};

/**
 * @test Verifies White king has 0 moves from starting position (surrounded by own pieces).
 */
TEST_F(KingPseudoLegalMovesTest, StartingPositionWhiteHas0Moves) {
  Board board;

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies Black king has 0 moves from starting position (surrounded by own pieces).
 */
TEST_F(KingPseudoLegalMovesTest, StartingPositionBlackHas0Moves) {
  Board board;

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies White king has no captures from starting position.
 */
TEST_F(KingPseudoLegalMovesTest, StartingPositionWhiteHasNoCaptures) {
  Board board;

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies Black king has no captures from starting position.
 */
TEST_F(KingPseudoLegalMovesTest, StartingPositionBlackHasNoCaptures) {
  Board board;

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies White king on empty board generates 8 moves (all directions).
 */
TEST_F(KingPseudoLegalMovesTest, WhiteKingCenterEmptyBoardHas8Moves) {
  Board board("8/8/8/8/3K4/8/8/8 w - - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies Black king on empty board generates 8 moves (all directions).
 */
TEST_F(KingPseudoLegalMovesTest, BlackKingCenterEmptyBoardHas8Moves) {
  Board board("8/8/8/8/3k4/8/8/8 b - - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies White king in corner has only 3 moves.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteKingInCornerHas3Moves) {
  Board board("8/8/8/8/8/8/8/K7 w - - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 3);
  EXPECT_TRUE(contains_move(moves, {Square(Square::A1), Square(Square::A2), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::A1), Square(Square::B1), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::A1), Square(Square::B2), std::nullopt, false, false, false}));
}

/**
 * @test Verifies Black king in corner has only 3 moves.
 */
TEST_F(KingPseudoLegalMovesTest, BlackKingInCornerHas3Moves) {
  Board board("k7/8/8/8/8/8/8/8 b - - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 3);
  EXPECT_TRUE(contains_move(moves, {Square(Square::A8), Square(Square::A7), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::A8), Square(Square::B8), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::A8), Square(Square::B7), std::nullopt, false, false, false}));
}

/**
 * @test Verifies White king on edge has 5 moves.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteKingOnEdgeHas5Moves) {
  Board board("8/8/8/8/K7/8/8/8 w - - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 5);
}

/**
 * @test Verifies Black king on edge has 5 moves.
 */
TEST_F(KingPseudoLegalMovesTest, BlackKingOnEdgeHas5Moves) {
  Board board("8/8/8/8/k7/8/8/8 b - - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 5);
}

/**
 * @test Verifies White king can capture enemy pieces.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteKingCanCaptureEnemyPieces) {
  Board board("8/8/8/8/2ppp3/2pKp3/2ppp3/8 w - - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_captures(moves), 8);
}

/**
 * @test Verifies Black king can capture enemy pieces.
 */
TEST_F(KingPseudoLegalMovesTest, BlackKingCanCaptureEnemyPieces) {
  Board board("8/8/8/8/2PPP3/2PkP3/2PPP3/8 b - - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_captures(moves), 8);
}

/**
 * @test Verifies White king cannot capture own pieces.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteKingCannotCaptureOwnPieces) {
  Board board("8/8/8/8/2PPP3/2PKP3/2PPP3/8 w - - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies Black king cannot capture own pieces.
 */
TEST_F(KingPseudoLegalMovesTest, BlackKingCannotCaptureOwnPieces) {
  Board board("8/8/8/8/2ppp3/2pkp3/2ppp3/8 b - - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies White king with mix of blocked and available squares.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteKingMixedOccupancy) {
  Board board("8/8/8/8/2Ppp3/2pKP3/2PPp3/8 w - - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_captures(moves), 4);
  EXPECT_EQ(moves.size(), 4);
}

/**
 * @test Verifies Black king with mix of blocked and available squares.
 */
TEST_F(KingPseudoLegalMovesTest, BlackKingMixedOccupancy) {
  Board board("8/8/8/8/2pPP3/2PkP3/2ppP3/8 b - - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_captures(moves), 5);
  EXPECT_EQ(moves.size(), 5);
}

/**
 * @test Verifies White kingside castling is available from starting position.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteKingsideCastlingAvailable) {
  Board board("8/8/8/8/8/8/8/R3K2R w KQ - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::E1), Square(Square::G1), std::nullopt, false, false, true}));
}

/**
 * @test Verifies White queenside castling is available from starting position.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteQueensideCastlingAvailable) {
  Board board("8/8/8/8/8/8/8/R3K2R w KQ - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::E1), Square(Square::C1), std::nullopt, false, false, true}));
}

/**
 * @test Verifies Black kingside castling is available from starting position.
 */
TEST_F(KingPseudoLegalMovesTest, BlackKingsideCastlingAvailable) {
  Board board("r3k2r/8/8/8/8/8/8/8 b kq - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::E8), Square(Square::G8), std::nullopt, false, false, true}));
}

/**
 * @test Verifies Black queenside castling is available from starting position.
 */
TEST_F(KingPseudoLegalMovesTest, BlackQueensideCastlingAvailable) {
  Board board("r3k2r/8/8/8/8/8/8/8 b kq - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::E8), Square(Square::C8), std::nullopt, false, false, true}));
}

/**
 * @test Verifies White castling unavailable when kingside rook is missing.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteKingsideCastlingBlockedNoRook) {
  Board board("8/8/8/8/8/8/8/R3K3 w KQ - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E1), Square(Square::G1), std::nullopt, false, false, true}));
}

/**
 * @test Verifies White castling unavailable when queenside rook is missing.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteQueensideCastlingBlockedNoRook) {
  Board board("8/8/8/8/8/8/8/4K2R w KQ - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E1), Square(Square::C1), std::nullopt, false, false, true}));
}

/**
 * @test Verifies Black castling unavailable when kingside rook is missing.
 */
TEST_F(KingPseudoLegalMovesTest, BlackKingsideCastlingBlockedNoRook) {
  Board board("r3k3/8/8/8/8/8/8/8 b kq - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E8), Square(Square::G8), std::nullopt, false, false, true}));
}

/**
 * @test Verifies Black castling unavailable when queenside rook is missing.
 */
TEST_F(KingPseudoLegalMovesTest, BlackQueensideCastlingBlockedNoRook) {
  Board board("4k2r/8/8/8/8/8/8/8 b kq - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E8), Square(Square::C8), std::nullopt, false, false, true}));
}

/**
 * @test Verifies White kingside castling blocked when F1 is occupied.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteKingsideCastlingBlockedByPieceOnF1) {
  Board board("8/8/8/8/8/8/8/R3KB1R w KQ - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E1), Square(Square::G1), std::nullopt, false, false, true}));
}

/**
 * @test Verifies White kingside castling blocked when G1 is occupied.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteKingsideCastlingBlockedByPieceOnG1) {
  Board board("8/8/8/8/8/8/8/R3K1NR w KQ - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E1), Square(Square::G1), std::nullopt, false, false, true}));
}

/**
 * @test Verifies White queenside castling blocked when D1 is occupied.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteQueensideCastlingBlockedByPieceOnD1) {
  Board board("8/8/8/8/8/8/8/R2QK2R w KQ - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E1), Square(Square::C1), std::nullopt, false, false, true}));
}

/**
 * @test Verifies White queenside castling blocked when C1 is occupied.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteQueensideCastlingBlockedByPieceOnC1) {
  Board board("8/8/8/8/8/8/8/R1B1K2R w KQ - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E1), Square(Square::C1), std::nullopt, false, false, true}));
}

/**
 * @test Verifies White queenside castling blocked when B1 is occupied.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteQueensideCastlingBlockedByPieceOnB1) {
  Board board("8/8/8/8/8/8/8/RN2K2R w KQ - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E1), Square(Square::C1), std::nullopt, false, false, true}));
}

/**
 * @test Verifies Black kingside castling blocked when F8 is occupied.
 */
TEST_F(KingPseudoLegalMovesTest, BlackKingsideCastlingBlockedByPieceOnF8) {
  Board board("r3kb1r/8/8/8/8/8/8/8 b kq - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E8), Square(Square::G8), std::nullopt, false, false, true}));
}

/**
 * @test Verifies Black kingside castling blocked when G8 is occupied.
 */
TEST_F(KingPseudoLegalMovesTest, BlackKingsideCastlingBlockedByPieceOnG8) {
  Board board("r3k1nr/8/8/8/8/8/8/8 b kq - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E8), Square(Square::G8), std::nullopt, false, false, true}));
}

/**
 * @test Verifies Black queenside castling blocked when D8 is occupied.
 */
TEST_F(KingPseudoLegalMovesTest, BlackQueensideCastlingBlockedByPieceOnD8) {
  Board board("r2qk2r/8/8/8/8/8/8/8 b kq - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E8), Square(Square::C8), std::nullopt, false, false, true}));
}

/**
 * @test Verifies Black queenside castling blocked when C8 is occupied.
 */
TEST_F(KingPseudoLegalMovesTest, BlackQueensideCastlingBlockedByPieceOnC8) {
  Board board("r1b1k2r/8/8/8/8/8/8/8 b kq - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E8), Square(Square::C8), std::nullopt, false, false, true}));
}

/**
 * @test Verifies Black queenside castling blocked when B8 is occupied.
 */
TEST_F(KingPseudoLegalMovesTest, BlackQueensideCastlingBlockedByPieceOnB8) {
  Board board("rn2k2r/8/8/8/8/8/8/8 b kq - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E8), Square(Square::C8), std::nullopt, false, false, true}));
}

/**
 * @test Verifies White castling unavailable when castling rights are lost.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteCastlingUnavailableWithoutRights) {
  Board board("8/8/8/8/8/8/8/R3K2R w - - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E1), Square(Square::G1), std::nullopt, false, false, true}));
  EXPECT_FALSE(contains_move(moves, {Square(Square::E1), Square(Square::C1), std::nullopt, false, false, true}));
}

/**
 * @test Verifies Black castling unavailable when castling rights are lost.
 */
TEST_F(KingPseudoLegalMovesTest, BlackCastlingUnavailableWithoutRights) {
  Board board("r3k2r/8/8/8/8/8/8/8 b - - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E8), Square(Square::G8), std::nullopt, false, false, true}));
  EXPECT_FALSE(contains_move(moves, {Square(Square::E8), Square(Square::C8), std::nullopt, false, false, true}));
}

/**
 * @test Verifies White can only castle kingside when queenside rights are lost.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteOnlyKingsideCastlingAvailable) {
  Board board("8/8/8/8/8/8/8/R3K2R w K - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::E1), Square(Square::G1), std::nullopt, false, false, true}));
  EXPECT_FALSE(contains_move(moves, {Square(Square::E1), Square(Square::C1), std::nullopt, false, false, true}));
}

/**
 * @test Verifies White can only castle queenside when kingside rights are lost.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteOnlyQueensideCastlingAvailable) {
  Board board("8/8/8/8/8/8/8/R3K2R w Q - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E1), Square(Square::G1), std::nullopt, false, false, true}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::E1), Square(Square::C1), std::nullopt, false, false, true}));
}

/**
 * @test Verifies Black can only castle kingside when queenside rights are lost.
 */
TEST_F(KingPseudoLegalMovesTest, BlackOnlyKingsideCastlingAvailable) {
  Board board("r3k2r/8/8/8/8/8/8/8 b k - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::E8), Square(Square::G8), std::nullopt, false, false, true}));
  EXPECT_FALSE(contains_move(moves, {Square(Square::E8), Square(Square::C8), std::nullopt, false, false, true}));
}

/**
 * @test Verifies Black can only castle queenside when kingside rights are lost.
 */
TEST_F(KingPseudoLegalMovesTest, BlackOnlyQueensideCastlingAvailable) {
  Board board("r3k2r/8/8/8/8/8/8/8 b q - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_FALSE(contains_move(moves, {Square(Square::E8), Square(Square::G8), std::nullopt, false, false, true}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::E8), Square(Square::C8), std::nullopt, false, false, true}));
}

/**
 * @test Verifies White king not on starting square prevents castling.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteKingNotOnStartingSquareNoCastling) {
  Board board("8/8/8/8/8/4K3/8/R6R w KQ - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  // King on E3, so no castling moves should be generated
  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_castling) << "No castling should be possible when king is not on starting square";
  }
}

/**
 * @test Verifies Black king not on starting square prevents castling.
 */
TEST_F(KingPseudoLegalMovesTest, BlackKingNotOnStartingSquareNoCastling) {
  Board board("r6r/8/4k3/8/8/8/8/8 b kq - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  // King on E6, so no castling moves should be generated
  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_castling) << "No castling should be possible when king is not on starting square";
  }
}

/**
 * @test Verifies exception thrown when no king is present.
 */
TEST_F(KingPseudoLegalMovesTest, ThrowsExceptionWhenNoKing) {
  Board board("8/8/8/8/8/8/8/8 w - - 0 1");

  EXPECT_THROW(KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE), std::runtime_error);
}

/**
 * @test Verifies exception thrown when multiple kings are present.
 */
TEST_F(KingPseudoLegalMovesTest, ThrowsExceptionWhenMultipleKings) {
  Board board("8/8/8/8/3K4/8/8/3K4 w - - 0 1");

  EXPECT_THROW(KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE), std::runtime_error);
}

/**
 * @test Verifies all king moves have no promotion flag.
 */
TEST_F(KingPseudoLegalMovesTest, AllMovesHaveNoPromotionFlag) {
  Board board("8/8/8/8/3K4/8/8/8 w - - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.promotion.has_value()) << "King moves should never have promotion";
  }
}

/**
 * @test Verifies all king moves have no en passant flag.
 */
TEST_F(KingPseudoLegalMovesTest, AllMovesHaveNoEnPassantFlag) {
  Board board("8/8/8/8/3K4/8/8/8 w - - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_en_passant) << "King moves should never be en passant";
  }
}

/**
 * @test Verifies only castling moves have castling flag set.
 */
TEST_F(KingPseudoLegalMovesTest, OnlyCastlingMovesHaveCastlingFlag) {
  Board board("8/8/8/8/8/8/8/R3K2R w KQ - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  for (const auto& move : moves) {
    if (move.is_castling) {
      EXPECT_TRUE(move.from == Square(Square::E1));
      EXPECT_TRUE(move.to == Square(Square::G1) || move.to == Square(Square::C1));
    }
  }
}

/**
 * @test Verifies complex position with captures and regular moves.
 */
TEST_F(KingPseudoLegalMovesTest, ComplexPositionMixedMoves) {
  Board board("8/8/8/2pPp3/2pKp3/2pPp3/8/8 w - - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 6);
  EXPECT_EQ(count_captures(moves), 6);
}

/**
 * @test Verifies both castling moves available simultaneously.
 */
TEST_F(KingPseudoLegalMovesTest, BothCastlingMovesAvailable) {
  Board board("8/8/8/8/8/8/8/R3K2R w KQ - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  int castling_count = 0;
  for (const auto& move : moves) {
    if (move.is_castling) castling_count++;
  }

  EXPECT_EQ(castling_count, 2);
}

/**
 * @test Verifies castling moves have correct from and to squares for White kingside.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteKingsideCastlingCorrectSquares) {
  Board board("8/8/8/8/8/8/8/R3K2R w KQ - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  bool found_kingside = false;
  for (const auto& move : moves) {
    if (move.is_castling && move.to == Square(Square::G1)) {
      EXPECT_EQ(move.from, Square(Square::E1));
      EXPECT_FALSE(move.is_capture);
      EXPECT_FALSE(move.is_en_passant);
      EXPECT_FALSE(move.promotion.has_value());
      found_kingside = true;
    }
  }
  EXPECT_TRUE(found_kingside) << "White kingside castling move should be present";
}

/**
 * @test Verifies castling moves have correct from and to squares for White queenside.
 */
TEST_F(KingPseudoLegalMovesTest, WhiteQueensideCastlingCorrectSquares) {
  Board board("8/8/8/8/8/8/8/R3K2R w KQ - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  bool found_queenside = false;
  for (const auto& move : moves) {
    if (move.is_castling && move.to == Square(Square::C1)) {
      EXPECT_EQ(move.from, Square(Square::E1));
      EXPECT_FALSE(move.is_capture);
      EXPECT_FALSE(move.is_en_passant);
      EXPECT_FALSE(move.promotion.has_value());
      found_queenside = true;
    }
  }
  EXPECT_TRUE(found_queenside) << "White queenside castling move should be present";
}

/**
 * @test Verifies castling moves have correct from and to squares for Black kingside.
 */
TEST_F(KingPseudoLegalMovesTest, BlackKingsideCastlingCorrectSquares) {
  Board board("r3k2r/8/8/8/8/8/8/8 b kq - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  bool found_kingside = false;
  for (const auto& move : moves) {
    if (move.is_castling && move.to == Square(Square::G8)) {
      EXPECT_EQ(move.from, Square(Square::E8));
      EXPECT_FALSE(move.is_capture);
      EXPECT_FALSE(move.is_en_passant);
      EXPECT_FALSE(move.promotion.has_value());
      found_kingside = true;
    }
  }
  EXPECT_TRUE(found_kingside) << "Black kingside castling move should be present";
}

/**
 * @test Verifies castling moves have correct from and to squares for Black queenside.
 */
TEST_F(KingPseudoLegalMovesTest, BlackQueensideCastlingCorrectSquares) {
  Board board("r3k2r/8/8/8/8/8/8/8 b kq - 0 1");

  KingMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  bool found_queenside = false;
  for (const auto& move : moves) {
    if (move.is_castling && move.to == Square(Square::C8)) {
      EXPECT_EQ(move.from, Square(Square::E8));
      EXPECT_FALSE(move.is_capture);
      EXPECT_FALSE(move.is_en_passant);
      EXPECT_FALSE(move.promotion.has_value());
      found_queenside = true;
    }
  }
  EXPECT_TRUE(found_queenside) << "Black queenside castling move should be present";
}
