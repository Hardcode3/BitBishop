#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/square.hpp>

/**
 * @test BoardTest.CanCastleKingsideWhiteSuccess
 * @brief Validates that White can castle kingside when all conditions are met.
 */
TEST(BoardTest, CanCastleKingsideWhiteSuccess) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
  EXPECT_TRUE(board.can_castle_kingside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleKingsideBlackSuccess
 * @brief Validates that Black can castle kingside when all conditions are met.
 */
TEST(BoardTest, CanCastleKingsideBlackSuccess) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
  EXPECT_TRUE(board.can_castle_kingside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleKingsideWhiteNoRights
 * @brief Validates that White cannot castle kingside without castling rights.
 */
TEST(BoardTest, CanCastleKingsideWhiteNoRights) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w Qkq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleKingsideBlackNoRights
 * @brief Validates that Black cannot castle kingside without castling rights.
 */
TEST(BoardTest, CanCastleKingsideBlackNoRights) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R b Qq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleKingsideWhiteKingMoved
 * @brief Validates that White cannot castle kingside if king is not on starting square.
 */
TEST(BoardTest, CanCastleKingsideWhiteKingMoved) {
  Board board("r3k2r/8/8/8/8/8/8/R4K1R w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleKingsideBlackKingMoved
 * @brief Validates that Black cannot castle kingside if king is not on starting square.
 */
TEST(BoardTest, CanCastleKingsideBlackKingMoved) {
  Board board("r4k1r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleKingsideWhiteNoRook
 * @brief Validates that White cannot castle kingside if rook is missing from h1.
 */
TEST(BoardTest, CanCastleKingsideWhiteNoRook) {
  Board board("r3k2r/8/8/8/8/8/8/R3K3 w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleKingsideBlackNoRook
 * @brief Validates that Black cannot castle kingside if rook is missing from h8.
 */
TEST(BoardTest, CanCastleKingsideBlackNoRook) {
  Board board("r3k3/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleKingsideWhiteBlockedF1
 * @brief Validates that White cannot castle kingside if f1 is occupied.
 */
TEST(BoardTest, CanCastleKingsideWhiteBlockedF1) {
  Board board("r3k2r/8/8/8/8/8/8/R3KB1R w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleKingsideBlackBlockedF8
 * @brief Validates that Black cannot castle kingside if f8 is occupied.
 */
TEST(BoardTest, CanCastleKingsideBlackBlockedF8) {
  Board board("r3kb1r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleKingsideWhiteBlockedG1
 * @brief Validates that White cannot castle kingside if g1 is occupied.
 */
TEST(BoardTest, CanCastleKingsideWhiteBlockedG1) {
  Board board("r3k2r/8/8/8/8/8/8/R3K1NR w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleKingsideBlackBlockedG8
 * @brief Validates that Black cannot castle kingside if g8 is occupied.
 */
TEST(BoardTest, CanCastleKingsideBlackBlockedG8) {
  Board board("r3k1nr/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleKingsideWhiteBothSquaresBlocked
 * @brief Validates that White cannot castle kingside if both f1 and g1 are occupied.
 */
TEST(BoardTest, CanCastleKingsideWhiteBothSquaresBlocked) {
  Board board("r3k2r/8/8/8/8/8/8/R3KBNR w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleKingsideBlackBothSquaresBlocked
 * @brief Validates that Black cannot castle kingside if both f8 and g8 are occupied.
 */
TEST(BoardTest, CanCastleKingsideBlackBothSquaresBlocked) {
  Board board("r3kbnr/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleKingsideStartingPosition
 * @brief Validates castling from standard starting position after pieces move.
 */
TEST(BoardTest, CanCastleKingsideStartingPosition) {
  Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::WHITE));
  EXPECT_FALSE(board.can_castle_kingside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleKingsideNoRightsAtAll
 * @brief Validates behavior when no castling rights exist for either side.
 */
TEST(BoardTest, CanCastleKingsideNoRightsAtAll) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::WHITE));
  EXPECT_FALSE(board.can_castle_kingside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleKingsideOppositeColorPieceBlocking
 * @brief Validates that opposite color pieces also block castling path.
 */
TEST(BoardTest, CanCastleKingsideOppositeColorPieceBlocking) {
  Board board("r3k2r/8/8/8/8/8/8/R3Kb1R w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleKingsideWrongRookPosition
 * @brief Validates that rook must be on the correct square (h1/h8).
 */
TEST(BoardTest, CanCastleKingsideWrongRookPosition) {
  Board board("r3k2r/8/8/8/8/8/8/R3K1R1 w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleKingsideMixedRights
 * @brief Validates correct behavior when only one side has kingside rights.
 */
TEST(BoardTest, CanCastleKingsideMixedRights) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w Kq - 0 1");
  EXPECT_TRUE(board.can_castle_kingside(Color::WHITE));
  EXPECT_FALSE(board.can_castle_kingside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleKingsideKingOnWrongSquare
 * @brief Validates that king on completely different square prevents castling.
 */
TEST(BoardTest, CanCastleKingsideKingOnWrongSquare) {
  Board board("r3k2r/8/8/8/3K4/8/8/R6R w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleKingsideOnlyQueensideRights
 * @brief Validates that having only queenside rights doesn't allow kingside castling.
 */
TEST(BoardTest, CanCastleKingsideOnlyQueensideRights) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w Qq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::WHITE));
  EXPECT_FALSE(board.can_castle_kingside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleKingsideBothSidesSimultaneously
 * @brief Validates that both sides can have kingside castling available.
 */
TEST(BoardTest, CanCastleKingsideBothSidesSimultaneously) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w Kk - 0 1");
  EXPECT_TRUE(board.can_castle_kingside(Color::WHITE));
  EXPECT_TRUE(board.can_castle_kingside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleKingsideRookReplacedByQueen
 * @brief Validates that a queen on the rook square doesn't allow castling.
 */
TEST(BoardTest, CanCastleKingsideRookReplacedByQueen) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2Q w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_kingside(Color::WHITE));
}
