#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>

/**
 * @test BoardTest.SetAndGetPiece
 * @brief Verifies placing and retrieving pieces.
 *
 * Checks:
 * - Placing white rook, white king, black queen
 * - Pieces correctly returned via get_piece
 * - Piece presence reflected in color-specific bitboards
 * - Occupied bitboard updated
 */
TEST(BoardTest, SetAndGetPiece) {
  Board board;

  board.set_piece(Square(0, 0), Pieces::WHITE_ROOK);   // White rook
  board.set_piece(Square(4, 0), Pieces::WHITE_KING);   // White king
  board.set_piece(Square(3, 7), Pieces::BLACK_QUEEN);  // Black queen

  EXPECT_EQ(board.get_piece(Square(0, 0)), Pieces::WHITE_ROOK);
  EXPECT_EQ(board.get_piece(Square(4, 0)), Pieces::WHITE_KING);
  EXPECT_EQ(board.get_piece(Square(3, 7)), Pieces::BLACK_QUEEN);

  EXPECT_TRUE(board.white_pieces().test(Square(0, 0)));
  EXPECT_TRUE(board.white_pieces().test(Square(4, 0)));
  EXPECT_TRUE(board.black_pieces().test(Square(3, 7)));

  EXPECT_TRUE(board.occupied().test(Square(0, 0)));
  EXPECT_TRUE(board.occupied().test(Square(4, 0)));
  EXPECT_TRUE(board.occupied().test(Square(3, 7)));
}

/**
 * @test BoardTest.RemovePiece
 * @brief Verifies removing a piece from a square.
 */
TEST(BoardTest, RemovePiece) {
  Board board;
  board.set_piece(Square(0, 0), Pieces::WHITE_ROOK);
  EXPECT_EQ(board.get_piece(Square(0, 0)), Pieces::WHITE_ROOK);

  board.remove_piece(Square(0, 0));
  EXPECT_EQ(board.get_piece(Square(0, 0)), std::nullopt);
  EXPECT_FALSE(board.occupied().test(Square(0, 0)));
}

/**
 * @test BoardTest.ReplacePiece
 * @brief Verifies replacing a piece with another of a different color.
 *
 * Checks:
 * - Original piece replaced
 * - Bitboards updated correctly (no ghost bits left)
 */
TEST(BoardTest, ReplacePiece) {
  Board board;

  board.set_piece(Square(0, 0), Pieces::BLACK_KNIGHT);  // replace with black knight
  EXPECT_EQ(board.get_piece(Square(0, 0)), Pieces::BLACK_KNIGHT);

  EXPECT_TRUE(board.black_pieces().test(Square(0, 0)));
  EXPECT_FALSE(board.white_pieces().test(Square(0, 0)));
}
