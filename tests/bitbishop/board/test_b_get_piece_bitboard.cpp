#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>

/**
 * @test BoardTest.PawnsBitboard
 * @brief Ensures that pawns() correctly returns the bitboard for each side.
 */
TEST(BoardTest, PawnsBitboard) {
  Board board;

  Bitboard white_pawns = board.pawns(Color::WHITE);
  Bitboard black_pawns = board.pawns(Color::BLACK);

  EXPECT_EQ(white_pawns.count(), 8);
  EXPECT_EQ(black_pawns.count(), 8);
  EXPECT_TRUE(white_pawns.test(Square::A2));
  EXPECT_FALSE(white_pawns.test(Square::H7));
  EXPECT_TRUE(black_pawns.test(Square::H7));
  EXPECT_FALSE(black_pawns.test(Square::A2));
}

/**
 * @test BoardTest.KingBitboard
 * @brief Ensures that king() correctly returns the bitboard for each side.
 */
TEST(BoardTest, KingBitboard) {
  Board board;

  Bitboard white_king = board.king(Color::WHITE);
  Bitboard black_king = board.king(Color::BLACK);

  EXPECT_EQ(white_king.count(), 1);
  EXPECT_EQ(black_king.count(), 1);
  EXPECT_TRUE(white_king.test(Square::E1));
  EXPECT_TRUE(black_king.test(Square::E8));
}

/**
 * @test BoardTest.RookBitboard
 * @brief Ensures that king() correctly returns the bitboard for each side.
 */
TEST(BoardTest, RookBitboard) {
  Board board;

  Bitboard white_rooks = board.rooks(Color::WHITE);
  Bitboard black_rooks = board.rooks(Color::BLACK);

  EXPECT_EQ(white_rooks.count(), 2);
  EXPECT_EQ(black_rooks.count(), 2);
  EXPECT_TRUE(white_rooks.test(Square::H1));
  EXPECT_TRUE(white_rooks.test(Square::A1));
  EXPECT_TRUE(black_rooks.test(Square::H8));
  EXPECT_TRUE(black_rooks.test(Square::A8));
}

/**
 * @test BoardTest.KnightsBitboard
 * @brief Ensures that knights() correctly returns the bitboard for each side.
 */
TEST(BoardTest, KnightsBitboard) {
  Board board;

  Bitboard white_knights = board.knights(Color::WHITE);
  Bitboard black_knights = board.knights(Color::BLACK);

  EXPECT_EQ(white_knights.count(), 2);
  EXPECT_EQ(black_knights.count(), 2);
  EXPECT_TRUE(white_knights.test(Squares::B1));
  EXPECT_TRUE(white_knights.test(Squares::G1));
  EXPECT_TRUE(black_knights.test(Square::B8));
  EXPECT_TRUE(black_knights.test(Square::G8));
}

/**
 * @test BoardTest.BishopsBitboard
 * @brief Ensures that bishops() correctly returns the bitboard for each side.
 */
TEST(BoardTest, BishopsBitboard) {
  Board board;

  Bitboard white_bishops = board.bishops(Color::WHITE);
  Bitboard black_bishops = board.bishops(Color::BLACK);

  EXPECT_EQ(white_bishops.count(), 2);
  EXPECT_EQ(black_bishops.count(), 2);
  EXPECT_TRUE(white_bishops.test(Squares::C1));
  EXPECT_TRUE(white_bishops.test(Squares::F1));
  EXPECT_TRUE(black_bishops.test(Square::C8));
  EXPECT_TRUE(black_bishops.test(Square::F8));
}
