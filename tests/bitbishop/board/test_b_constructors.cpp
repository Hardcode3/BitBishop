#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>

/**
 * @test BoardTest.DefaultStartingPosDefaultConstructor
 * @brief Verifies that a newly constructed board has the default FEN starting postion
 */
TEST(BoardTest, DefaultStartingPosDefaultConstructor) {
  Board board;

  // Check major pieces
  EXPECT_EQ(board.get_piece(Square(0, 0)), Pieces::WHITE_ROOK);
  EXPECT_EQ(board.get_piece(Square(4, 0)), Pieces::WHITE_KING);
  EXPECT_EQ(board.get_piece(Square(0, 7)), Pieces::BLACK_ROOK);
  EXPECT_EQ(board.get_piece(Square(4, 7)), Pieces::BLACK_KING);

  // Pawns
  for (int file = 0; file < 8; ++file) {
    EXPECT_EQ(board.get_piece(Square(file, 1)), Pieces::WHITE_PAWN);
    EXPECT_EQ(board.get_piece(Square(file, 6)), Pieces::BLACK_PAWN);
  }

  EXPECT_EQ(board.white_pieces().value() & board.black_pieces().value(), 0ULL);  // no overlap
  EXPECT_EQ(board.occupied().value(), board.white_pieces().value() | board.black_pieces().value());
}

/**
 * @test BoardTest.FENConstructor
 * @brief Verifies constructing a board from FEN string.
 *
 * Example FEN: standard starting position.
 */
TEST(BoardTest, FENConstructor) {
  std::string start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  Board board(start_fen);

  // Check major pieces
  EXPECT_EQ(board.get_piece(Square(0, 0)), Pieces::WHITE_ROOK);
  EXPECT_EQ(board.get_piece(Square(4, 0)), Pieces::WHITE_KING);
  EXPECT_EQ(board.get_piece(Square(0, 7)), Pieces::BLACK_ROOK);
  EXPECT_EQ(board.get_piece(Square(4, 7)), Pieces::BLACK_KING);

  // Pawns
  for (int file = 0; file < 8; ++file) {
    EXPECT_EQ(board.get_piece(Square(file, 1)), Pieces::WHITE_PAWN);
    EXPECT_EQ(board.get_piece(Square(file, 6)), Pieces::BLACK_PAWN);
  }

  EXPECT_EQ(board.white_pieces().value() & board.black_pieces().value(), 0ULL);  // no overlap
  EXPECT_EQ(board.occupied().value(), board.white_pieces().value() | board.black_pieces().value());
}
