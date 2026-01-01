#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test King square on starting position.
 * @brief Confirms king_square() returns correct squares for a normal starting board.
 */
TEST(BoardKingSquareTest, StartingPosition) {
  Board board = Board::StartingPosition();

  auto white_king_sq = board.king_square(Color::WHITE);
  auto black_king_sq = board.king_square(Color::BLACK);

  ASSERT_TRUE(white_king_sq.has_value());
  ASSERT_TRUE(black_king_sq.has_value());

  EXPECT_EQ(white_king_sq.value(), E1);
  EXPECT_EQ(black_king_sq.value(), E8);
}

/**
 * @test King square on empty board.
 * @brief Confirms king_square() returns std::nullopt when no kings are present.
 */
TEST(BoardKingSquareTest, EmptyBoard) {
  Board board = Board::Empty();

  auto white_king_sq = board.king_square(Color::WHITE);
  auto black_king_sq = board.king_square(Color::BLACK);

  EXPECT_FALSE(white_king_sq.has_value());
  EXPECT_FALSE(black_king_sq.has_value());
}

/**
 * @test King square with only white king present.
 * @brief Confirms king_square() returns std::nullopt for missing black king.
 */
TEST(BoardKingSquareTest, OnlyWhiteKing) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);

  auto white_king_sq = board.king_square(Color::WHITE);
  auto black_king_sq = board.king_square(Color::BLACK);

  ASSERT_TRUE(white_king_sq.has_value());
  EXPECT_EQ(white_king_sq.value(), E1);
  EXPECT_FALSE(black_king_sq.has_value());
}

/**
 * @test King square with only black king present.
 * @brief Confirms king_square() returns std::nullopt for missing white king.
 */
TEST(BoardKingSquareTest, OnlyBlackKing) {
  Board board = Board::Empty();
  board.set_piece(E8, BLACK_KING);

  auto white_king_sq = board.king_square(Color::WHITE);
  auto black_king_sq = board.king_square(Color::BLACK);

  EXPECT_FALSE(white_king_sq.has_value());
  ASSERT_TRUE(black_king_sq.has_value());
  EXPECT_EQ(black_king_sq.value(), E8);
}

/**
 * @test King square after moving the king.
 * @brief Confirms king_square() updates correctly when a king is moved.
 */
TEST(BoardKingSquareTest, KingMoved) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);

  board.move_piece(E1, G1);  // Simulate castling move

  auto white_king_sq = board.king_square(Color::WHITE);
  EXPECT_TRUE(white_king_sq.has_value());
  EXPECT_EQ(white_king_sq.value(), G1);
}
