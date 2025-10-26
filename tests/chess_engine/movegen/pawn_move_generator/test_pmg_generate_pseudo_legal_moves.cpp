#include <gtest/gtest.h>

#include <chess_engine/helpers/moves.cpp>
#include <chess_engine/movegen/pawn_move_gen.hpp>

class PawnPseudoLegalMovesTest : public ::testing::Test {
 protected:
  PawnMoveGenerator generator;

  void SetUp() override {}

  void TearDown() override {}
};

TEST_F(PawnPseudoLegalMovesTest, StartingPositionWhiteHas16Moves) {
  Board board;  // Default constructor creates starting position

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // 8 pawns × 2 moves each (single + double push)
  EXPECT_EQ(moves.size(), 16);
}

TEST_F(PawnPseudoLegalMovesTest, StartingPositionBlackHas16Moves) {
  Board board;

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  // 8 pawns × 2 moves each (single + double push)
  EXPECT_EQ(moves.size(), 16);
}

TEST_F(PawnPseudoLegalMovesTest, StartingPositionWhiteHasNoCaptures) {
  Board board;

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(count_captures(moves), 0);
}

TEST_F(PawnPseudoLegalMovesTest, StartingPositionBlackHasNoCaptures) {
  Board board;

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_EQ(count_captures(moves), 0);
}

TEST_F(PawnPseudoLegalMovesTest, StartingPositionWhiteHasNoPromotions) {
  Board board;

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(count_promotions(moves), 0);
}

TEST_F(PawnPseudoLegalMovesTest, StartingPositionWhiteHasCorrectSinglePushes) {
  Board board;

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // Check that each pawn can move one square forward
  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::A3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::B2), Square(Square::B3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::C2), Square(Square::C3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::D2), Square(Square::D3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::E2), Square(Square::E3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::F2), Square(Square::F3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::G2), Square(Square::G3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::H2), Square(Square::H3), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, StartingPositionWhiteHasCorrectDoublePushes) {
  Board board;

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // Check that each pawn can move two squares forward
  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::A4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::B2), Square(Square::B4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::C2), Square(Square::C4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::D2), Square(Square::D4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::E2), Square(Square::E4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::F2), Square(Square::F4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::G2), Square(Square::G4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::H2), Square(Square::H4), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, WhiteSinglePushBlockedByOwnPiece) {
  Board board("8/8/8/8/8/P7/P7/8 w - - 0 1");  // White pawn on a2, blocked by pawn on a3

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // No moves should be generated for the blocked pawn
  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, BlackSinglePushBlockedByOwnPiece) {
  Board board("8/p7/p7/8/8/8/8/8 b - - 0 1");  // Black pawn on a7, blocked by pawn on a6

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, WhiteSinglePushBlockedByEnemyPiece) {
  Board board("8/8/8/8/8/p7/P7/8 w - - 0 1");  // White pawn on a2, blocked by black pawn on a3

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // No push moves (pawn is blocked), but we should check if there are any diagonal captures
  EXPECT_EQ(count_moves_to(moves, Square(Square::A3)), 0);
}

TEST_F(PawnPseudoLegalMovesTest, WhiteSinglePushToEmptySquare) {
  Board board("8/8/8/8/8/8/P7/8 w - - 0 1");  // White pawn on a2, empty ahead

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::A3), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, BlackSinglePushToEmptySquare) {
  Board board("8/p7/8/8/8/8/8/8 b - - 0 1");  // Black pawn on a7, empty ahead

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A7), Square(Square::A6), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, WhiteDoublePushFromStartingRank) {
  Board board("8/8/8/8/8/8/P7/8 w - - 0 1");  // White pawn on a2

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::A4), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, BlackDoublePushFromStartingRank) {
  Board board("8/p7/8/8/8/8/8/8 b - - 0 1");  // Black pawn on a7

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A7), Square(Square::A5), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, WhiteDoublePushBlockedByPieceOnThirdRank) {
  Board board("8/8/8/8/8/P7/P7/8 w - - 0 1");  // White pawn on a2, blocked on a3

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // Should not have double push to a4
  EXPECT_FALSE(contains_move(moves, {Square(Square::A2), Square(Square::A4), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, WhiteDoublePushBlockedByPieceOnFourthRank) {
  Board board("8/8/8/8/P7/8/P7/8 w - - 0 1");  // White pawn on a2, piece on a4

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // Should have single push but not double push
  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::A3), std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Square(Square::A2), Square(Square::A4), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, WhiteDoublePushNotAvailableFromThirdRank) {
  Board board("8/8/8/8/8/P7/8/8 w - - 0 1");  // White pawn on a3 (not starting rank)

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // Should only have single push, no double push
  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {Square(Square::A3), Square(Square::A4), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, BlackDoublePushNotAvailableFromSixthRank) {
  Board board("8/8/p7/8/8/8/8/8 b - - 0 1");  // Black pawn on a6 (not starting rank)

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  // Should only have single push, no double push
  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {Square(Square::A6), Square(Square::A5), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, WhitePawnCanCaptureDiagonally) {
  Board board("8/8/8/8/8/1p6/P7/8 w - - 0 1");  // White pawn on a2, black pawn on b3

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::B3), std::nullopt, true, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, BlackPawnCanCaptureDiagonally) {
  Board board("8/p7/1P6/8/8/8/8/8 b - - 0 1");  // Black pawn on a7, white pawn on b6

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A7), Square(Square::B6), std::nullopt, true, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, WhitePawnCanCaptureBothDiagonals) {
  Board board("8/8/8/8/8/pPp5/1P6/8 w - - 0 1");  // White pawn on b2, black pawns on a3 and c3

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::B2), Square(Square::A3), std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::B2), Square(Square::C3), std::nullopt, true, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, WhitePawnCannotCaptureOwnPieces) {
  Board board("8/8/8/8/8/PPP5/1P6/8 w - - 0 1");  // White pawn on b2, white pawns on a3, b3, c3

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // Should have no captures
  EXPECT_EQ(count_captures(moves), 0);
}

TEST_F(PawnPseudoLegalMovesTest, WhitePawnCannotCaptureForward) {
  Board board("8/8/8/8/8/p7/P7/8 w - - 0 1");  // White pawn on a2, black pawn on a3

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // No moves should be generated (can't capture forward, can't push forward)
  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, WhitePawnPromotesOnEighthRank) {
  Board board("8/P7/8/8/8/8/8/8 w - - 0 1");  // White pawn on a7, can promote to a8

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // Should have 4 promotion moves (Q, R, B, N)
  EXPECT_EQ(moves.size(), 4);
  EXPECT_EQ(count_promotions(moves), 4);
}

TEST_F(PawnPseudoLegalMovesTest, BlackPawnPromotesOnFirstRank) {
  Board board("8/8/8/8/8/8/p7/8 b - - 0 1");  // Black pawn on a2, can promote to a1

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  // Should have 4 promotion moves (q, r, b, n)
  EXPECT_EQ(moves.size(), 4);
  EXPECT_EQ(count_promotions(moves), 4);
}

TEST_F(PawnPseudoLegalMovesTest, WhitePawnPromotesWithCapture) {
  Board board("1p6/P7/8/8/8/8/8/8 w - - 0 1");  // White pawn on a7, black pawn on b8

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // Should have 4 promotion moves (pushing) + 4 promotion captures = 8 total
  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_promotions(moves), 8);
  EXPECT_EQ(count_captures(moves), 4);
}

TEST_F(PawnPseudoLegalMovesTest, BlackPawnPromotesWithCapture) {
  Board board("8/8/8/8/8/8/p7/1P6 b - - 0 1");  // Black pawn on a2, white pawn on b1

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  // Should have 4 promotion moves (pushing) + 4 promotion captures = 8 total
  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_promotions(moves), 8);
  EXPECT_EQ(count_captures(moves), 4);
}

TEST_F(PawnPseudoLegalMovesTest, WhitePawnPromotionBlockedByPiece) {
  Board board("P7/P7/8/8/8/8/8/8 w - - 0 1");  // White pawn on a7, blocked by pawn on a8

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // No promotion moves should be generated (blocked)
  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, WhitePawnCanCaptureEnPassant) {
  // Black pawn just moved from e7 to e5, white pawn on d5
  Board board("8/8/8/3Pp3/8/8/8/8 w - e6 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // Should have en passant capture move
  EXPECT_TRUE(contains_move(moves, {Square(Square::D5), Square(Square::E6), std::nullopt, true, true, false}));
  EXPECT_EQ(count_en_passant(moves), 1);
}

TEST_F(PawnPseudoLegalMovesTest, BlackPawnCanCaptureEnPassant) {
  // White pawn just moved from d2 to d4, black pawn on e4
  Board board("8/8/8/8/3pP3/8/8/8 b - d3 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  // Should have en passant capture move
  EXPECT_TRUE(contains_move(moves, {Square(Square::E4), Square(Square::D3), std::nullopt, true, true, false}));
  EXPECT_EQ(count_en_passant(moves), 1);
}

TEST_F(PawnPseudoLegalMovesTest, EnPassantNotAvailableWhenNoTarget) {
  Board board("8/8/8/3Pp3/8/8/8/8 w - - 0 1");  // No en passant square set

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(count_en_passant(moves), 0);
}

TEST_F(PawnPseudoLegalMovesTest, EnPassantBothSidesCanCapture) {
  // Black pawn just moved from e7 to e5, white pawns on d5 and f5
  Board board("8/8/8/3PpP2/8/8/8/8 w - e6 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // Both pawns should be able to capture en passant
  EXPECT_TRUE(contains_move(moves, {Square(Square::D5), Square(Square::E6), std::nullopt, true, true, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::F5), Square(Square::E6), std::nullopt, true, true, false}));
  EXPECT_EQ(count_en_passant(moves), 2);
}

TEST_F(PawnPseudoLegalMovesTest, EmptyBoardGeneratesNoMoves) {
  Board board("8/8/8/8/8/8/8/8 w - - 0 1");  // Empty board

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, NoPawnsGeneratesNoMoves) {
  Board board("rnbqkbnr/8/8/8/8/8/8/RNBQKBNR w - - 0 1");  // All pieces except pawns

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, ComplexPositionWhitePawns) {
  // Multiple pawns with various move possibilities
  Board board("8/8/8/8/1p1p1p2/P1P1P3/1P1P4/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // Verify no invalid moves (all moves should be valid)
  EXPECT_GT(moves.size(), 0);

  // All moves should have no castling flag
  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_castling);
  }
}

TEST_F(PawnPseudoLegalMovesTest, MultiplePawnsGenerateMultipleMoves) {
  Board board("8/8/8/8/8/8/PPPPPPPP/8 w - - 0 1");  // 8 white pawns on second rank

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // Each pawn should have single and double push = 16 moves total
  EXPECT_EQ(moves.size(), 16);
}

TEST_F(PawnPseudoLegalMovesTest, PawnOnSeventhRankMultiplePromotions) {
  Board board("8/PPP5/8/8/8/8/8/8 w - - 0 1");  // 3 white pawns on a7, b7, c7

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // 3 pawns × 4 promotions each = 12 moves
  EXPECT_EQ(moves.size(), 12);
  EXPECT_EQ(count_promotions(moves), 12);
}

TEST_F(PawnPseudoLegalMovesTest, PawnOnEdgeFileAFile) {
  Board board("8/8/8/8/8/p7/P7/8 w - - 0 1");  // White pawn on a2, black pawn on a3

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // Should have no moves (blocked, no right diagonal available)
  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, PawnOnEdgeFileHFile) {
  Board board("8/8/8/8/8/7p/7P/8 w - - 0 1");  // White pawn on h2, black pawn on h3

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // Should have no moves (blocked, no left diagonal available)
  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, AllMovesHaveNoCastlingFlag) {
  Board board;  // Starting position

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_castling) << "Pawn moves should never have castling flag";
  }
}
