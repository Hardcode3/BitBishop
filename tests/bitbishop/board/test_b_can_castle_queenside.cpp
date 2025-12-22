#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/square.hpp>

/**
 * @test BoardTest.CanCastleQueensideWhiteSuccess
 * @brief Validates that White can castle queenside when all conditions are met.
 */
TEST(BoardTest, CanCastleQueensideWhiteSuccess) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
  EXPECT_TRUE(board.can_castle_queenside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleQueensideBlackSuccess
 * @brief Validates that Black can castle queenside when all conditions are met.
 */
TEST(BoardTest, CanCastleQueensideBlackSuccess) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
  EXPECT_TRUE(board.can_castle_queenside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleQueensideWhiteNoRights
 * @brief Validates that White cannot castle queenside without castling rights.
 */
TEST(BoardTest, CanCastleQueensideWhiteNoRights) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w Kkq - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleQueensideBlackNoRights
 * @brief Validates that Black cannot castle queenside without castling rights.
 */
TEST(BoardTest, CanCastleQueensideBlackNoRights) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R b Kk - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleQueensideWhiteKingMoved
 * @brief Validates that White cannot castle queenside if king is not on starting square.
 */
TEST(BoardTest, CanCastleQueensideWhiteKingMoved) {
  Board board("r3k2r/8/8/8/8/8/8/R2K3R w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleQueensideBlackKingMoved
 * @brief Validates that Black cannot castle queenside if king is not on starting square.
 */
TEST(BoardTest, CanCastleQueensideBlackKingMoved) {
  Board board("r2k3r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleQueensideWhiteNoRook
 * @brief Validates that White cannot castle queenside if rook is missing from a1.
 */
TEST(BoardTest, CanCastleQueensideWhiteNoRook) {
  Board board("r3k2r/8/8/8/8/8/8/4K2R w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleQueensideBlackNoRook
 * @brief Validates that Black cannot castle queenside if rook is missing from a8.
 */
TEST(BoardTest, CanCastleQueensideBlackNoRook) {
  Board board("4k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleQueensideWhiteBlockedB1
 * @brief Validates that White cannot castle queenside if b1 is occupied.
 */
TEST(BoardTest, CanCastleQueensideWhiteBlockedB1) {
  Board board("r3k2r/8/8/8/8/8/8/RN2K2R w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleQueensideBlackBlockedB8
 * @brief Validates that Black cannot castle queenside if b8 is occupied.
 */
TEST(BoardTest, CanCastleQueensideBlackBlockedB8) {
  Board board("rn2k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleQueensideWhiteBlockedC1
 * @brief Validates that White cannot castle queenside if c1 is occupied.
 */
TEST(BoardTest, CanCastleQueensideWhiteBlockedC1) {
  Board board("r3k2r/8/8/8/8/8/8/R1B1K2R w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleQueensideBlackBlockedC8
 * @brief Validates that Black cannot castle queenside if c8 is occupied.
 */
TEST(BoardTest, CanCastleQueensideBlackBlockedC8) {
  Board board("r1b1k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleQueensideWhiteBlockedD1
 * @brief Validates that White cannot castle queenside if d1 is occupied.
 */
TEST(BoardTest, CanCastleQueensideWhiteBlockedD1) {
  Board board("r3k2r/8/8/8/8/8/8/R2QK2R w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleQueensideBlackBlockedD8
 * @brief Validates that Black cannot castle queenside if d8 is occupied.
 */
TEST(BoardTest, CanCastleQueensideBlackBlockedD8) {
  Board board("r2qk2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleQueensideWhiteMultipleBlocks
 * @brief Validates that White cannot castle queenside if multiple squares are blocked.
 */
TEST(BoardTest, CanCastleQueensideWhiteMultipleBlocks) {
  Board board("r3k2r/8/8/8/8/8/8/RNbQK2R w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleQueensideBlackMultipleBlocks
 * @brief Validates that Black cannot castle queenside if multiple squares are blocked.
 */
TEST(BoardTest, CanCastleQueensideBlackMultipleBlocks) {
  Board board("r1bqk2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleQueensideStartingPosition
 * @brief Validates castling from standard starting position after pieces move.
 */
TEST(BoardTest, CanCastleQueensideStartingPosition) {
  Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::WHITE));
  EXPECT_FALSE(board.can_castle_queenside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleQueensideNoRightsAtAll
 * @brief Validates behavior when no castling rights exist for either side.
 */
TEST(BoardTest, CanCastleQueensideNoRightsAtAll) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::WHITE));
  EXPECT_FALSE(board.can_castle_queenside(Color::BLACK));
}

/**
 * @test BoardTest.CanCastleQueensideOppositeColorPieceBlocking
 * @brief Validates that opposite color pieces also block castling path.
 */
TEST(BoardTest, CanCastleQueensideOppositeColorPieceBlocking) {
  Board board("r3k2r/8/8/8/8/8/8/R1n1K2R w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::WHITE));
}

/**
 * @test BoardTest.CanCastleQueensideWrongRookPosition
 * @brief Validates that rook must be on the correct square (a1/a8).
 */
TEST(BoardTest, CanCastleQueensideWrongRookPosition) {
  Board board("r3k2r/8/8/8/8/8/8/1R2K2R w KQkq - 0 1");
  EXPECT_FALSE(board.can_castle_queenside(Color::WHITE));
}
