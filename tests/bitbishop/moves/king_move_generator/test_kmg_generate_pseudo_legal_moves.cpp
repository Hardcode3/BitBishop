#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/moves/king_move_gen.hpp>

/**
 * @brief Test fixture for king pseudo-legal move generation.
 */
class KingPseudoLegalMovesTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  void TearDown() override {}
};

TEST_F(KingPseudoLegalMovesTest, WhiteStartingPositionHas0Moves) {}

TEST_F(KingPseudoLegalMovesTest, BlackStartingPositionHas0Moves) {}

TEST_F(KingPseudoLegalMovesTest, WhiteStartingPositionHasNoCapture) {}

TEST_F(KingPseudoLegalMovesTest, BlackStartingPositionHasNoCapture) {}

TEST_F(KingPseudoLegalMovesTest, WhitePushBlockedByOwnPiece) {}

TEST_F(KingPseudoLegalMovesTest, BlackPushBlockedByOwnPiece) {}

TEST_F(KingPseudoLegalMovesTest, WhitePushBlockedByEnemyPiece) {}

TEST_F(KingPseudoLegalMovesTest, BlackPushBlockedByEnemyPiece) {}

TEST_F(KingPseudoLegalMovesTest, WhitePushToEmptySquare) {}

TEST_F(KingPseudoLegalMovesTest, BlackPushToEmptySquare) {}

TEST_F(KingPseudoLegalMovesTest, WhiteCanCaptureNorth) {}

TEST_F(KingPseudoLegalMovesTest, WhiteCanCaptureNorthEast) {}

TEST_F(KingPseudoLegalMovesTest, WhiteCanCaptureEast) {}

TEST_F(KingPseudoLegalMovesTest, WhiteCanCaptureSouthEast) {}

TEST_F(KingPseudoLegalMovesTest, WhiteCanCaptureSouth) {}

TEST_F(KingPseudoLegalMovesTest, WhiteCanCaptureSouthWest) {}

TEST_F(KingPseudoLegalMovesTest, WhiteCanCaptureWest) {}

TEST_F(KingPseudoLegalMovesTest, WhiteCanCaptureNorthWest) {}

TEST_F(KingPseudoLegalMovesTest, BlackCanCaptureNorth) {}

TEST_F(KingPseudoLegalMovesTest, BlackCanCaptureNorthEast) {}

TEST_F(KingPseudoLegalMovesTest, BlackCanCaptureEast) {}

TEST_F(KingPseudoLegalMovesTest, BlackCanCaptureSouthEast) {}

TEST_F(KingPseudoLegalMovesTest, BlackCanCaptureSouth) {}

TEST_F(KingPseudoLegalMovesTest, BlackCanCaptureSouthWest) {}

TEST_F(KingPseudoLegalMovesTest, BlackCanCaptureWest) {}

TEST_F(KingPseudoLegalMovesTest, BlackCanCaptureNorthWest) {}

TEST_F(KingPseudoLegalMovesTest, WhiteEmptyBoardGeneratesNoMoves) {}

TEST_F(KingPseudoLegalMovesTest, BlackEmptyBoardGeneratesNoMoves) {}

TEST_F(KingPseudoLegalMovesTest, WhiteNoWhiteKingGeneratesError) {}

TEST_F(KingPseudoLegalMovesTest, WhiteMoreThan1WhiteKingGeneratesError) {}

TEST_F(KingPseudoLegalMovesTest, BlackNoWhiteKingGeneratesError) {}

TEST_F(KingPseudoLegalMovesTest, BlackMoreThan1WhiteKingGeneratesError) {}

TEST_F(KingPseudoLegalMovesTest, ComplexPositionWhitePawns) {}

TEST_F(KingPseudoLegalMovesTest, ComplexPositionBlackPawns) {}

TEST_F(KingPseudoLegalMovesTest, KingOnEdgeAFile) {}

TEST_F(KingPseudoLegalMovesTest, KingOnEdgeHFile) {}
