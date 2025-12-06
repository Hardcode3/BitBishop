#include <gtest/gtest.h>

#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/moves/knight_move_gen.hpp>

/**
 * @brief Test fixture for knight pseudo-legal move generation.
 */
class KnightPseudoLegalMovesTest : public ::testing::Test {
 protected:
  std::vector<Move> moves;

  void SetUp() override {}

  void TearDown() override {}
};

/**
 * @test Verifies White knights have 4 moves from starting position.
 */
TEST_F(KnightPseudoLegalMovesTest, StartingPositionWhiteHas4Moves) {
  Board board;

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 4);
}

/**
 * @test Verifies Black knights have 4 moves from starting position.
 */
TEST_F(KnightPseudoLegalMovesTest, StartingPositionBlackHas4Moves) {
  Board board;

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 4);
}

/**
 * @test Verifies White knights have no captures from starting position.
 */
TEST_F(KnightPseudoLegalMovesTest, StartingPositionWhiteHasNoCaptures) {
  Board board;

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies Black knights have no captures from starting position.
 */
TEST_F(KnightPseudoLegalMovesTest, StartingPositionBlackHasNoCaptures) {
  Board board;

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies White knight on empty board generates 8 moves (center position).
 */
TEST_F(KnightPseudoLegalMovesTest, WhiteKnightCenterEmptyBoardHas8Moves) {
  Board board("8/8/8/8/3N4/8/8/8 w - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies Black knight on empty board generates 8 moves (center position).
 */
TEST_F(KnightPseudoLegalMovesTest, BlackKnightCenterEmptyBoardHas8Moves) {
  Board board("8/8/8/8/3n4/8/8/8 b - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies White knight in corner has only 2 moves.
 */
TEST_F(KnightPseudoLegalMovesTest, WhiteKnightInCornerHas2Moves) {
  Board board("8/8/8/8/8/8/8/N7 w - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 2);
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::B3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::C2, std::nullopt, false, false, false}));
}

/**
 * @test Verifies Black knight in corner has only 2 moves.
 */
TEST_F(KnightPseudoLegalMovesTest, BlackKnightInCornerHas2Moves) {
  Board board("n7/8/8/8/8/8/8/8 b - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 2);
  EXPECT_TRUE(contains_move(moves, {Squares::A8, Squares::B6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A8, Squares::C7, std::nullopt, false, false, false}));
}

/**
 * @test Verifies White knight on edge has 4 moves.
 */
TEST_F(KnightPseudoLegalMovesTest, WhiteKnightOnEdgeHas4Moves) {
  Board board("8/8/8/8/N7/8/8/8 w - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 4);
}

/**
 * @test Verifies Black knight on edge has 4 moves.
 */
TEST_F(KnightPseudoLegalMovesTest, BlackKnightOnEdgeHas4Moves) {
  Board board("8/8/8/8/n7/8/8/8 b - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 4);
}

/**
 * @test Verifies White knight can capture enemy pieces.
 */
TEST_F(KnightPseudoLegalMovesTest, WhiteKnightCanCaptureEnemyPieces) {
  Board board("8/8/8/2p1p3/1p3p2/3N4/1p3p2/2p1p3 w - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_captures(moves), 8);
}

/**
 * @test Verifies Black knight can capture enemy pieces.
 */
TEST_F(KnightPseudoLegalMovesTest, BlackKnightCanCaptureEnemyPieces) {
  Board board("8/8/8/2P1P3/1P3P2/3n4/1P3P2/2P1P3 b - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_captures(moves), 8);
}

/**
 * @test Verifies White knight cannot capture own pieces.
 */
TEST_F(KnightPseudoLegalMovesTest, WhiteKnightCannotCaptureOwnPieces) {
  Board board("8/8/8/2P1P3/1P3P2/3N4/1P3P2/2P1P3 w - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies Black knight cannot capture own pieces.
 */
TEST_F(KnightPseudoLegalMovesTest, BlackKnightCannotCaptureOwnPieces) {
  Board board("8/8/8/2p1p3/1p3p2/3n4/1p3p2/2p1p3 b - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies White knight with mix of blocked and available squares.
 */
TEST_F(KnightPseudoLegalMovesTest, WhiteKnightMixedOccupancy) {
  Board board("8/8/8/2P1p3/1p3P2/3N4/1P3p2/2p1P3 w - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_captures(moves), 4);
  EXPECT_EQ(moves.size(), 4);
}

/**
 * @test Verifies Black knight with mix of blocked and available squares.
 */
TEST_F(KnightPseudoLegalMovesTest, BlackKnightMixedOccupancy) {
  Board board("8/8/8/2p1P3/1P3p2/3n4/1p3P2/2P1p3 b - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_captures(moves), 4);
  EXPECT_EQ(moves.size(), 4);
}

/**
 * @test Verifies White knight near edge has 6 moves.
 */
TEST_F(KnightPseudoLegalMovesTest, WhiteKnightNearEdgeHas6Moves) {
  Board board("8/8/8/8/8/8/N7/8 w - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 3);
}

/**
 * @test Verifies Black knight near edge has 6 moves.
 */
TEST_F(KnightPseudoLegalMovesTest, BlackKnightNearEdgeHas6Moves) {
  Board board("8/n7/8/8/8/8/8/8 b - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 3);
}

/**
 * @test Verifies multiple White knights generate correct total moves.
 */
TEST_F(KnightPseudoLegalMovesTest, MultipleWhiteKnights) {
  Board board("8/8/8/8/3N4/8/8/N7 w - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 10);  // 8 from center knight + 2 from corner knight
}

/**
 * @test Verifies multiple Black knights generate correct total moves.
 */
TEST_F(KnightPseudoLegalMovesTest, MultipleBlackKnights) {
  Board board("n7/8/8/8/3n4/8/8/8 b - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 10);  // 8 from center knight + 2 from corner knight
}

/**
 * @test Verifies no moves when no knights on board.
 */
TEST_F(KnightPseudoLegalMovesTest, NoKnightsNoMoves) {
  Board board("8/8/8/8/8/8/8/8 w - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies knight moves are not marked as captures when landing on empty squares.
 */
TEST_F(KnightPseudoLegalMovesTest, NonCaptureMovesFlaggedCorrectly) {
  Board board("8/8/8/8/3N4/8/8/8 w - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_capture);
  }
}

/**
 * @test Verifies knight captures are marked correctly.
 */
TEST_F(KnightPseudoLegalMovesTest, CaptureMovesFlaggedCorrectly) {
  Board board("8/8/8/2p1p3/1p3p2/3N4/1p3p2/2p1p3 w - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  for (const auto& move : moves) {
    EXPECT_TRUE(move.is_capture);
  }
}

/**
 * @test Verifies specific knight move targets from d4.
 */
TEST_F(KnightPseudoLegalMovesTest, SpecificKnightMovesFromD4) {
  Board board("8/8/8/8/3N4/8/8/8 w - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E6, std::nullopt, false, false, false}));
}

/**
 * @test Verifies knight moves do not include promotion flags.
 */
TEST_F(KnightPseudoLegalMovesTest, KnightMovesHaveNoPromotionFlags) {
  Board board("8/8/8/8/3N4/8/8/8 w - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.promotion.has_value());
  }
}

/**
 * @test Verifies knight moves do not include castling flags.
 */
TEST_F(KnightPseudoLegalMovesTest, KnightMovesHaveNoCastlingFlags) {
  Board board("8/8/8/8/3N4/8/8/8 w - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_castling);
  }
}

/**
 * @test Verifies knight moves do not include en passant flags.
 */
TEST_F(KnightPseudoLegalMovesTest, KnightMovesHaveNoEnPassantFlags) {
  Board board("8/8/8/8/3N4/8/8/8 w - - 0 1");

  KnightMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_en_passant);
  }
}
