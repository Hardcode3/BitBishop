#include <gtest/gtest.h>

#include <chess_engine/helpers/moves.hpp>
#include <chess_engine/move.hpp>
#include <chess_engine/moves/pawn_move_gen.hpp>

/**
 * @brief Test fixture for pawn pseudo-legal move generation.
 */
class PawnPseudoLegalMovesTest : public ::testing::Test {
 protected:
  std::vector<Move> moves;

  void SetUp() override {}

  void TearDown() override {}
};

/**
 * @test Verifies White has 16 pawn moves from starting position (8 pawns × 2 moves).
 */
TEST_F(PawnPseudoLegalMovesTest, StartingPositionWhiteHas16Moves) {
  Board board;

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 16);
}

/**
 * @test Verifies Black has 16 pawn moves from starting position (8 pawns × 2 moves).
 */
TEST_F(PawnPseudoLegalMovesTest, StartingPositionBlackHas16Moves) {
  Board board;

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 16);
}

/**
 * @test Verifies White has no captures from starting position.
 */
TEST_F(PawnPseudoLegalMovesTest, StartingPositionWhiteHasNoCaptures) {
  Board board;

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies Black has no captures from starting position.
 */
TEST_F(PawnPseudoLegalMovesTest, StartingPositionBlackHasNoCaptures) {
  Board board;

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies White has no promotions from starting position.
 */
TEST_F(PawnPseudoLegalMovesTest, StartingPositionWhiteHasNoPromotions) {
  Board board;

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_promotions(moves), 0);
}

/**
 * @test Verifies Black has no promotions from starting position.
 */
TEST_F(PawnPseudoLegalMovesTest, StartingPositionBlackHasNoPromotions) {
  Board board;

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_promotions(moves), 0);
}

/**
 * @test Verifies White generates all 8 single pushes from starting position (rank 2 to rank 3).
 */
TEST_F(PawnPseudoLegalMovesTest, StartingPositionWhiteHasCorrectSinglePushes) {
  Board board;

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::A3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::B2), Square(Square::B3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::C2), Square(Square::C3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::D2), Square(Square::D3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::E2), Square(Square::E3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::F2), Square(Square::F3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::G2), Square(Square::G3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::H2), Square(Square::H3), std::nullopt, false, false, false}));
}

/**
 * @test Verifies Black generates all 8 single pushes from starting position (rank 7 to rank 6).
 */
TEST_F(PawnPseudoLegalMovesTest, StartingPositionBlackHasCorrectSinglePushes) {
  Board board;

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A7), Square(Square::A6), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::B7), Square(Square::B6), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::C7), Square(Square::C6), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::D7), Square(Square::D6), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::E7), Square(Square::E6), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::F7), Square(Square::F6), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::G7), Square(Square::G6), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::H7), Square(Square::H6), std::nullopt, false, false, false}));
}

/**
 * @test Verifies White generates all 8 double pushes from starting position (rank 2 to rank 4).
 */
TEST_F(PawnPseudoLegalMovesTest, StartingPositionWhiteHasCorrectDoublePushes) {
  Board board;

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::A4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::B2), Square(Square::B4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::C2), Square(Square::C4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::D2), Square(Square::D4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::E2), Square(Square::E4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::F2), Square(Square::F4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::G2), Square(Square::G4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::H2), Square(Square::H4), std::nullopt, false, false, false}));
}

/**
 * @test Verifies Black generates all 8 double pushes from starting position (rank 7 to rank 5).
 */
TEST_F(PawnPseudoLegalMovesTest, StartingPositionBlackHasCorrectDoublePushes) {
  Board board;

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A7), Square(Square::A5), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::B7), Square(Square::B5), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::C7), Square(Square::C5), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::D7), Square(Square::D5), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::E7), Square(Square::E5), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::F7), Square(Square::F5), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::G7), Square(Square::G5), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::H7), Square(Square::H5), std::nullopt, false, false, false}));
}

/**
 * @test Verifies White single push is blocked when own piece occupies destination.
 */
TEST_F(PawnPseudoLegalMovesTest, WhiteSinglePushBlockedByOwnPiece) {
  Board board("8/8/8/8/8/P7/P7/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 1);
}

/**
 * @test Verifies Black single push is blocked when own piece occupies destination.
 */
TEST_F(PawnPseudoLegalMovesTest, BlackSinglePushBlockedByOwnPiece) {
  Board board("8/p7/p7/8/8/8/8/8 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 1);
}

/**
 * @test Verifies White single push is blocked when enemy piece occupies destination.
 */
TEST_F(PawnPseudoLegalMovesTest, WhiteSinglePushBlockedByEnemyPiece) {
  Board board("8/8/8/8/8/p7/P7/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies Black single push is blocked when enemy piece occupies destination.
 */
TEST_F(PawnPseudoLegalMovesTest, BlackSinglePushBlockedByEnemyPiece) {
  Board board("8/p7/P7/8/8/8/8/8 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies White pawn can single push to empty square.
 */
TEST_F(PawnPseudoLegalMovesTest, WhiteSinglePushToEmptySquare) {
  Board board("8/8/8/8/8/8/P7/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::A3), std::nullopt, false, false, false}));
}

/**
 * @test Verifies Black pawn can single push to empty square.
 */
TEST_F(PawnPseudoLegalMovesTest, BlackSinglePushToEmptySquare) {
  Board board("8/7p/8/8/8/8/8/8 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::H7), Square(Square::H6), std::nullopt, false, false, false}));
}

/**
 * @test Verifies White pawn can double push from starting rank.
 */
TEST_F(PawnPseudoLegalMovesTest, WhiteDoublePushFromStartingRank) {
  Board board("8/8/8/8/8/8/P7/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::A4), std::nullopt, false, false, false}));
}

/**
 * @test Verifies Black pawn can double push from starting rank.
 */
TEST_F(PawnPseudoLegalMovesTest, BlackDoublePushFromStartingRank) {
  Board board("8/7p/8/8/8/8/8/8 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::H7), Square(Square::H5), std::nullopt, false, false, false}));
}

/**
 * @test Verifies White double push is blocked when intermediate square is occupied.
 */
TEST_F(PawnPseudoLegalMovesTest, WhiteDoublePushBlockedByPieceOnThirdRank) {
  Board board("8/8/8/8/8/P7/P7/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_FALSE(contains_move(moves, {Square(Square::A2), Square(Square::A4), std::nullopt, false, false, false}));
}

/**
 * @test Verifies Black double push is blocked when intermediate square is occupied.
 */
TEST_F(PawnPseudoLegalMovesTest, BlackDoublePushBlockedByPieceOnThirdRank) {
  Board board("8/7p/7p/8/8/8/8/8 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_FALSE(contains_move(moves, {Square(Square::H7), Square(Square::H5), std::nullopt, false, false, false}));
}

/**
 * @test Verifies White double push is blocked when destination square is occupied.
 */
TEST_F(PawnPseudoLegalMovesTest, WhiteDoublePushBlockedByPieceOnFourthRank) {
  Board board("8/8/8/8/P7/8/P7/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::A3), std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Square(Square::A2), Square(Square::A4), std::nullopt, false, false, false}));
}

/**
 * @test Verifies Black double push is blocked when destination square is occupied.
 */
TEST_F(PawnPseudoLegalMovesTest, BlackDoublePushBlockedByPieceOnFourthRank) {
  Board board("8/7p/8/7p/8/8/8/8 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::H7), Square(Square::H6), std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Square(Square::H7), Square(Square::H5), std::nullopt, false, false, false}));
}

/**
 * @test Verifies White double push is unavailable from non-starting rank.
 */
TEST_F(PawnPseudoLegalMovesTest, WhiteDoublePushNotAvailableFromThirdRank) {
  Board board("8/8/8/8/8/P7/8/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {Square(Square::A3), Square(Square::A4), std::nullopt, false, false, false}));
}

/**
 * @test Verifies Black double push is unavailable from non-starting rank.
 */
TEST_F(PawnPseudoLegalMovesTest, BlackDoublePushNotAvailableFromSixthRank) {
  Board board("8/8/7p/8/8/8/8/8 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {Square(Square::H6), Square(Square::H5), std::nullopt, false, false, false}));
}

/**
 * @test Verifies White pawn can capture diagonally.
 */
TEST_F(PawnPseudoLegalMovesTest, WhitePawnCanCaptureDiagonally) {
  Board board("8/8/8/8/8/1p6/P7/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::B3), std::nullopt, true, false, false}));
}

/**
 * @test Verifies Black pawn can capture diagonally.
 */
TEST_F(PawnPseudoLegalMovesTest, BlackPawnCanCaptureDiagonally) {
  Board board("8/7p/6P1/8/8/8/8/8 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::H7), Square(Square::G6), std::nullopt, true, false, false}));
}

/**
 * @test Verifies White pawn can capture on both diagonals simultaneously.
 */
TEST_F(PawnPseudoLegalMovesTest, WhitePawnCanCaptureBothDiagonals) {
  Board board("8/8/8/8/8/pPp5/1P6/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::B2), Square(Square::A3), std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::B2), Square(Square::C3), std::nullopt, true, false, false}));
}

/**
 * @test Verifies Black pawn can capture on both diagonals simultaneously.
 */
TEST_F(PawnPseudoLegalMovesTest, BlackPawnCanCaptureBothDiagonals) {
  Board board("8/6p1/5PpP/8/8/8/8/8 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::G7), Square(Square::H6), std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::G7), Square(Square::F6), std::nullopt, true, false, false}));
}

/**
 * @test Verifies White pawn cannot capture own pieces.
 */
TEST_F(PawnPseudoLegalMovesTest, WhitePawnCannotCaptureOwnPieces) {
  Board board("8/8/8/8/8/PPP5/1P6/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies Black pawn cannot capture own pieces.
 */
TEST_F(PawnPseudoLegalMovesTest, BlackPawnCannotCaptureOwnPieces) {
  Board board("8/5ppp/6p1/8/8/8/8/8 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Verifies White pawn cannot capture by moving forward.
 */
TEST_F(PawnPseudoLegalMovesTest, WhitePawnCannotCaptureForward) {
  Board board("8/8/8/8/8/p7/P7/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies Black pawn cannot capture by moving forward.
 */
TEST_F(PawnPseudoLegalMovesTest, BlackPawnCannotCaptureForward) {
  Board board("8/7p/7P/8/8/8/8/8 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies White pawn promotes when reaching eighth rank (4 promotion options).
 */
TEST_F(PawnPseudoLegalMovesTest, WhitePawnPromotesOnEighthRank) {
  Board board("8/P7/8/8/8/8/8/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 4);
  EXPECT_EQ(count_promotions(moves), 4);
}

/**
 * @test Verifies Black pawn promotes when reaching first rank (4 promotion options).
 */
TEST_F(PawnPseudoLegalMovesTest, BlackPawnPromotesOnFirstRank) {
  Board board("8/8/8/8/8/8/p7/8 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 4);
  EXPECT_EQ(count_promotions(moves), 4);
}

/**
 * @test Verifies White promotion generates capture and non-capture variants (8 total moves).
 */
TEST_F(PawnPseudoLegalMovesTest, WhitePawnPromotesWithCapture) {
  Board board("1p6/P7/8/8/8/8/8/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_promotions(moves), 8);
  EXPECT_EQ(count_captures(moves), 4);
}

/**
 * @test Verifies Black promotion generates capture and non-capture variants (8 total moves).
 */
TEST_F(PawnPseudoLegalMovesTest, BlackPawnPromotesWithCapture) {
  Board board("8/8/8/8/8/8/p7/1P6 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_promotions(moves), 8);
  EXPECT_EQ(count_captures(moves), 4);
}

/**
 * @test Verifies White promotion is blocked when destination is occupied.
 */
TEST_F(PawnPseudoLegalMovesTest, WhitePawnPromotionBlockedByPiece) {
  Board board("R7/P7/8/8/8/8/8/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies Black promotion is blocked when destination is occupied.
 */
TEST_F(PawnPseudoLegalMovesTest, BlackPawnPromotionBlockedByPiece) {
  Board board("8/8/8/8/8/8/7P/7R b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies White pawn can capture en passant when available.
 */
TEST_F(PawnPseudoLegalMovesTest, WhitePawnCanCaptureEnPassant) {
  Board board("8/8/8/3Pp3/8/8/8/8 w - e6 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::D5), Square(Square::E6), std::nullopt, true, true, false}));
  EXPECT_EQ(count_en_passant(moves), 1);
}

/**
 * @test Verifies Black pawn can capture en passant when available.
 */
TEST_F(PawnPseudoLegalMovesTest, BlackPawnCanCaptureEnPassant) {
  Board board("8/8/8/8/3pP3/8/8/8 b - e3 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::D4), Square(Square::E3), std::nullopt, true, true, false}));
  EXPECT_EQ(count_en_passant(moves), 1);
}

/**
 * @test Verifies White en passant is unavailable when no target square is set.
 */
TEST_F(PawnPseudoLegalMovesTest, WhiteEnPassantNotAvailableWhenNoTarget) {
  Board board("8/8/8/3Pp3/8/8/8/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_en_passant(moves), 0);
}

/**
 * @test Verifies Black en passant is unavailable when no target square is set.
 */
TEST_F(PawnPseudoLegalMovesTest, BlackEnPassantNotAvailableWhenNoTarget) {
  Board board("8/8/8/8/3pP3/8/8/8 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_en_passant(moves), 0);
}

/**
 * @test Verifies both White pawns can capture the same en passant target.
 */
TEST_F(PawnPseudoLegalMovesTest, WhiteEnPassantBothSidesCanCapture) {
  Board board("8/8/8/3PpP2/8/8/8/8 w - e6 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::D5), Square(Square::E6), std::nullopt, true, true, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::F5), Square(Square::E6), std::nullopt, true, true, false}));
  EXPECT_EQ(count_en_passant(moves), 2);
}

/**
 * @test Verifies both Black pawns can capture the same en passant target.
 */
TEST_F(PawnPseudoLegalMovesTest, BlackEnPassantBothSidesCanCapture) {
  Board board("8/8/8/8/3pPp2/8/8/8 b - e3 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::D4), Square(Square::E3), std::nullopt, true, true, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::F4), Square(Square::E3), std::nullopt, true, true, false}));
  EXPECT_EQ(count_en_passant(moves), 2);
}

/**
 * @test Verifies empty board generates no White pawn moves.
 */
TEST_F(PawnPseudoLegalMovesTest, WhiteEmptyBoardGeneratesNoMoves) {
  Board board("8/8/8/8/8/8/8/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies empty board generates no Black pawn moves.
 */
TEST_F(PawnPseudoLegalMovesTest, BlackEmptyBoardGeneratesNoMoves) {
  Board board("8/8/8/8/8/8/8/8 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies no moves when White has no pawns.
 */
TEST_F(PawnPseudoLegalMovesTest, NoWhitePawnsGeneratesNoMoves) {
  Board board("rnbqkbnr/8/8/1p6/8/8/8/RNBQKBNR w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies no moves when Black has no pawns.
 */
TEST_F(PawnPseudoLegalMovesTest, NoBlackPawnsGeneratesNoMoves) {
  Board board("rnbqkbnr/8/8/1P6/8/8/8/RNBQKBNR b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies complex position generates correct number of White pawn moves.
 */
TEST_F(PawnPseudoLegalMovesTest, ComplexPositionWhitePawns) {
  Board board("8/8/8/8/1p1p1p2/P1P1P3/1P1P4/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_GT(moves.size(), 0);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_castling);
  }

  EXPECT_EQ(moves.size(), 10);
}

/**
 * @test Verifies complex position generates correct number of Black pawn moves.
 */
TEST_F(PawnPseudoLegalMovesTest, ComplexPositionBlackPawns) {
  Board board("8/8/8/8/1p1p1p2/P1P1P3/1P1P4/8 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_GT(moves.size(), 0);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_castling);
  }

  EXPECT_EQ(moves.size(), 8);
}

/**
 * @test Verifies 8 White pawns on rank 2 generate 16 moves (single + double push each).
 */
TEST_F(PawnPseudoLegalMovesTest, MultipleWhitePawnsGenerateMultipleMoves) {
  Board board("8/8/8/8/8/8/PPPPPPPP/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 16);
}

/**
 * @test Verifies 8 Black pawns on rank 7 generate 16 moves (single + double push each).
 */
TEST_F(PawnPseudoLegalMovesTest, MultipleBlackPawnsGenerateMultipleMoves) {
  Board board("8/pppppppp/8/8/8/8/8/8 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 16);
}

/**
 * @test Verifies 3 White pawns on rank 7 generate 12 promotion moves (3 × 4).
 */
TEST_F(PawnPseudoLegalMovesTest, WhitePawnOnSeventhRankMultiplePromotions) {
  Board board("8/PPP5/8/8/8/8/8/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 12);
  EXPECT_EQ(count_promotions(moves), 12);
}

/**
 * @test Verifies 3 Black pawns on rank 2 generate 12 promotion moves (3 × 4).
 */
TEST_F(PawnPseudoLegalMovesTest, BlackPawnOnSeventhRankMultiplePromotions) {
  Board board("8/8/8/8/8/8/ppp5/8 b - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 12);
  EXPECT_EQ(count_promotions(moves), 12);
}

/**
 * @test Verifies White pawn blocked on A-file edge.
 */
TEST_F(PawnPseudoLegalMovesTest, PawnOnEdgeFileAFile) {
  Board board("8/8/8/8/8/p7/P7/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies White pawn blocked on H-file edge.
 */
TEST_F(PawnPseudoLegalMovesTest, PawnOnEdgeFileHFile) {
  Board board("8/8/8/8/8/7p/7P/8 w - - 0 1");

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Verifies pawn moves never have castling flag set.
 */
TEST_F(PawnPseudoLegalMovesTest, AllMovesHaveNoCastlingFlag) {
  Board board;

  PawnMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_castling) << "Pawn moves should never have castling flag";
  }
}
