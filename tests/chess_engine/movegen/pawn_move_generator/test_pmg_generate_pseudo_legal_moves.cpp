#include <gtest/gtest.h>

#include <chess_engine/helpers/moves.hpp>
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

TEST_F(PawnPseudoLegalMovesTest, StartingPositionBlackHasNoPromotions) {
  Board board;

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_EQ(count_promotions(moves), 0);
}

TEST_F(PawnPseudoLegalMovesTest, StartingPositionWhiteHasCorrectSinglePushes) {
  Board board;

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::A3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::B2), Square(Square::B3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::C2), Square(Square::C3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::D2), Square(Square::D3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::E2), Square(Square::E3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::F2), Square(Square::F3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::G2), Square(Square::G3), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::H2), Square(Square::H3), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, StartingPositionBlackHasCorrectSinglePushes) {
  Board board;

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A7), Square(Square::A6), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::B7), Square(Square::B6), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::C7), Square(Square::C6), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::D7), Square(Square::D6), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::E7), Square(Square::E6), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::F7), Square(Square::F6), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::G7), Square(Square::G6), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::H7), Square(Square::H6), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, StartingPositionWhiteHasCorrectDoublePushes) {
  Board board;

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::A4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::B2), Square(Square::B4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::C2), Square(Square::C4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::D2), Square(Square::D4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::E2), Square(Square::E4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::F2), Square(Square::F4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::G2), Square(Square::G4), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::H2), Square(Square::H4), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, StartingPositionBlackHasCorrectDoublePushes) {
  Board board;

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A7), Square(Square::A5), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::B7), Square(Square::B5), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::C7), Square(Square::C5), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::D7), Square(Square::D5), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::E7), Square(Square::E5), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::F7), Square(Square::F5), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::G7), Square(Square::G5), std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::H7), Square(Square::H5), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, WhiteSinglePushBlockedByOwnPiece) {
  Board board("8/8/8/8/8/P7/P7/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(moves.size(), 1);
}

TEST_F(PawnPseudoLegalMovesTest, BlackSinglePushBlockedByOwnPiece) {
  Board board("8/p7/p7/8/8/8/8/8 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_EQ(moves.size(), 1);
}

TEST_F(PawnPseudoLegalMovesTest, WhiteSinglePushBlockedByEnemyPiece) {
  Board board("8/8/8/8/8/p7/P7/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, BlackSinglePushBlockedByEnemyPiece) {
  Board board("8/p7/P7/8/8/8/8/8 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, WhiteSinglePushToEmptySquare) {
  Board board("8/8/8/8/8/8/P7/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::A3), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, BlackSinglePushToEmptySquare) {
  Board board("8/7p/8/8/8/8/8/8 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::H7), Square(Square::H6), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, WhiteDoublePushFromStartingRank) {
  Board board("8/8/8/8/8/8/P7/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::A4), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, BlackDoublePushFromStartingRank) {
  Board board("8/7p/8/8/8/8/8/8 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::H7), Square(Square::H5), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, WhiteDoublePushBlockedByPieceOnThirdRank) {
  Board board("8/8/8/8/8/P7/P7/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_FALSE(contains_move(moves, {Square(Square::A2), Square(Square::A4), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, BlackDoublePushBlockedByPieceOnThirdRank) {
  Board board("8/7p/7p/8/8/8/8/8 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_FALSE(contains_move(moves, {Square(Square::H7), Square(Square::H5), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, WhiteDoublePushBlockedByPieceOnFourthRank) {
  Board board("8/8/8/8/P7/8/P7/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::A3), std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Square(Square::A2), Square(Square::A4), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, BlackDoublePushBlockedByPieceOnFourthRank) {
  Board board("8/7p/8/7p/8/8/8/8 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::H7), Square(Square::H6), std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Square(Square::H7), Square(Square::H5), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, WhiteDoublePushNotAvailableFromThirdRank) {
  Board board("8/8/8/8/8/P7/8/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {Square(Square::A3), Square(Square::A4), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, BlackDoublePushNotAvailableFromSixthRank) {
  Board board("8/8/7p/8/8/8/8/8 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {Square(Square::H6), Square(Square::H5), std::nullopt, false, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, WhitePawnCanCaptureDiagonally) {
  Board board("8/8/8/8/8/1p6/P7/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::A2), Square(Square::B3), std::nullopt, true, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, BlackPawnCanCaptureDiagonally) {
  Board board("8/7p/6P1/8/8/8/8/8 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::H7), Square(Square::G6), std::nullopt, true, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, WhitePawnCanCaptureBothDiagonals) {
  Board board("8/8/8/8/8/pPp5/1P6/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::B2), Square(Square::A3), std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::B2), Square(Square::C3), std::nullopt, true, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, BlackPawnCanCaptureBothDiagonals) {
  Board board("8/6p1/5PpP/8/8/8/8/8 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::G7), Square(Square::H6), std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::G7), Square(Square::F6), std::nullopt, true, false, false}));
}

TEST_F(PawnPseudoLegalMovesTest, WhitePawnCannotCaptureOwnPieces) {
  Board board("8/8/8/8/8/PPP5/1P6/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(count_captures(moves), 0);
}

TEST_F(PawnPseudoLegalMovesTest, BlackPawnCannotCaptureOwnPieces) {
  Board board("8/5ppp/6p1/8/8/8/8/8 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_EQ(count_captures(moves), 0);
}

TEST_F(PawnPseudoLegalMovesTest, WhitePawnCannotCaptureForward) {
  Board board("8/8/8/8/8/p7/P7/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, BlackPawnCannotCaptureForward) {
  Board board("8/7p/7P/8/8/8/8/8 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, WhitePawnPromotesOnEighthRank) {
  Board board("8/P7/8/8/8/8/8/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(moves.size(), 4);
  EXPECT_EQ(count_promotions(moves), 4);
}

TEST_F(PawnPseudoLegalMovesTest, BlackPawnPromotesOnFirstRank) {
  Board board("8/8/8/8/8/8/p7/8 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_EQ(moves.size(), 4);
  EXPECT_EQ(count_promotions(moves), 4);
}

TEST_F(PawnPseudoLegalMovesTest, WhitePawnPromotesWithCapture) {
  Board board("1p6/P7/8/8/8/8/8/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_promotions(moves), 8);
  EXPECT_EQ(count_captures(moves), 4);
}

TEST_F(PawnPseudoLegalMovesTest, BlackPawnPromotesWithCapture) {
  Board board("8/8/8/8/8/8/p7/1P6 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_promotions(moves), 8);
  EXPECT_EQ(count_captures(moves), 4);
}

TEST_F(PawnPseudoLegalMovesTest, WhitePawnPromotionBlockedByPiece) {
  Board board("R7/P7/8/8/8/8/8/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, BlackPawnPromotionBlockedByPiece) {
  Board board("8/8/8/8/8/8/7P/7R b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, WhitePawnCanCaptureEnPassant) {
  Board board("8/8/8/3Pp3/8/8/8/8 w - e6 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::D5), Square(Square::E6), std::nullopt, true, true, false}));
  EXPECT_EQ(count_en_passant(moves), 1);
}

TEST_F(PawnPseudoLegalMovesTest, BlackPawnCanCaptureEnPassant) {
  Board board("8/8/8/8/3pP3/8/8/8 b - e3 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::D4), Square(Square::E3), std::nullopt, true, true, false}));
  EXPECT_EQ(count_en_passant(moves), 1);
}

TEST_F(PawnPseudoLegalMovesTest, WhiteEnPassantNotAvailableWhenNoTarget) {
  Board board("8/8/8/3Pp3/8/8/8/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(count_en_passant(moves), 0);
}

TEST_F(PawnPseudoLegalMovesTest, BlackEnPassantNotAvailableWhenNoTarget) {
  Board board("8/8/8/8/3pP3/8/8/8 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_EQ(count_en_passant(moves), 0);
}

TEST_F(PawnPseudoLegalMovesTest, WhiteEnPassantBothSidesCanCapture) {
  Board board("8/8/8/3PpP2/8/8/8/8 w - e6 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_TRUE(contains_move(moves, {Square(Square::D5), Square(Square::E6), std::nullopt, true, true, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::F5), Square(Square::E6), std::nullopt, true, true, false}));
  EXPECT_EQ(count_en_passant(moves), 2);
}

TEST_F(PawnPseudoLegalMovesTest, BlackEnPassantBothSidesCanCapture) {
  Board board("8/8/8/8/3pPp2/8/8/8 b - e3 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_TRUE(contains_move(moves, {Square(Square::D4), Square(Square::E3), std::nullopt, true, true, false}));
  EXPECT_TRUE(contains_move(moves, {Square(Square::F4), Square(Square::E3), std::nullopt, true, true, false}));
  EXPECT_EQ(count_en_passant(moves), 2);
}

TEST_F(PawnPseudoLegalMovesTest, WhiteEmptyBoardGeneratesNoMoves) {
  Board board("8/8/8/8/8/8/8/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, BlackEmptyBoardGeneratesNoMoves) {
  Board board("8/8/8/8/8/8/8/8 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, NoWhitePawnsGeneratesNoMoves) {
  Board board("rnbqkbnr/8/8/1p6/8/8/8/RNBQKBNR w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, NoBlackPawnsGeneratesNoMoves) {
  Board board("rnbqkbnr/8/8/1P6/8/8/8/RNBQKBNR b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, ComplexPositionWhitePawns) {
  Board board("8/8/8/8/1p1p1p2/P1P1P3/1P1P4/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_GT(moves.size(), 0);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_castling);
  }

  EXPECT_EQ(moves.size(), 10);
}

TEST_F(PawnPseudoLegalMovesTest, ComplexPositionBlackPawns) {
  Board board("8/8/8/8/1p1p1p2/P1P1P3/1P1P4/8 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_GT(moves.size(), 0);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_castling);
  }

  EXPECT_EQ(moves.size(), 8);
}

TEST_F(PawnPseudoLegalMovesTest, MultipleWhitePawnsGenerateMultipleMoves) {
  Board board("8/8/8/8/8/8/PPPPPPPP/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(moves.size(), 16);
}

TEST_F(PawnPseudoLegalMovesTest, MultipleBlackPawnsGenerateMultipleMoves) {
  Board board("8/pppppppp/8/8/8/8/8/8 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  EXPECT_EQ(moves.size(), 16);
}

TEST_F(PawnPseudoLegalMovesTest, WhitePawnOnSeventhRankMultiplePromotions) {
  Board board("8/PPP5/8/8/8/8/8/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  // 3 pawns × 4 promotions each = 12 moves
  EXPECT_EQ(moves.size(), 12);
  EXPECT_EQ(count_promotions(moves), 12);
}

TEST_F(PawnPseudoLegalMovesTest, BlackPawnOnSeventhRankMultiplePromotions) {
  Board board("8/8/8/8/8/8/ppp5/8 b - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::BLACK);

  // 3 pawns × 4 promotions each = 12 moves
  EXPECT_EQ(moves.size(), 12);
  EXPECT_EQ(count_promotions(moves), 12);
}

TEST_F(PawnPseudoLegalMovesTest, PawnOnEdgeFileAFile) {
  Board board("8/8/8/8/8/p7/P7/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, PawnOnEdgeFileHFile) {
  Board board("8/8/8/8/8/7p/7P/8 w - - 0 1");

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnPseudoLegalMovesTest, AllMovesHaveNoCastlingFlag) {
  Board board;  // Starting position

  auto moves = generator.generate_pseudo_legal_moves(board, Color::WHITE);

  for (const auto& move : moves) {
    EXPECT_FALSE(move.is_castling) << "Pawn moves should never have castling flag";
  }
}
