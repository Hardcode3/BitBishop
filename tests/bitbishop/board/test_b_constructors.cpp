#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test BoardTest.DefaultStartingPosDefaultConstructor
 * @brief Verifies that a newly constructed board has the default FEN starting postion
 */
TEST(BoardTest, DefaultStartingPosDefaultConstructor) {
  Board board;

  // Check major pieces
  EXPECT_EQ(board.get_piece(Square(0, 0)), WHITE_ROOK);
  EXPECT_EQ(board.get_piece(Square(4, 0)), WHITE_KING);
  EXPECT_EQ(board.get_piece(Square(0, 7)), BLACK_ROOK);
  EXPECT_EQ(board.get_piece(Square(4, 7)), BLACK_KING);

  // Pawns
  for (int file = 0; file < 8; ++file) {
    EXPECT_EQ(board.get_piece(Square(file, 1)), WHITE_PAWN);
    EXPECT_EQ(board.get_piece(Square(file, 6)), BLACK_PAWN);
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
  EXPECT_EQ(board.get_piece(Square(0, 0)), WHITE_ROOK);
  EXPECT_EQ(board.get_piece(Square(4, 0)), WHITE_KING);
  EXPECT_EQ(board.get_piece(Square(0, 7)), BLACK_ROOK);
  EXPECT_EQ(board.get_piece(Square(4, 7)), BLACK_KING);

  // Pawns
  for (int file = 0; file < 8; ++file) {
    EXPECT_EQ(board.get_piece(Square(file, 1)), WHITE_PAWN);
    EXPECT_EQ(board.get_piece(Square(file, 6)), BLACK_PAWN);
  }

  EXPECT_EQ(board.white_pieces().value() & board.black_pieces().value(), 0ULL);  // no overlap
  EXPECT_EQ(board.occupied().value(), board.white_pieces().value() | board.black_pieces().value());
}

/**
 * @test Move constructor preserves board state.
 * @brief Confirms that moving a board with the move constructor produces an identical board.
 */
TEST(BoardCopyTest, MoveConstructor) {
  Board original = Board::Empty();

  original.set_piece(E1, WHITE_KING);
  original.set_piece(E8, BLACK_KING);
  original.set_piece(D4, WHITE_QUEEN);
  original.set_piece(A7, BLACK_PAWN);

  Board copy(std::move(original));  // Copy constructor

  // Piece positions
  EXPECT_EQ(copy.get_piece(E1), WHITE_KING);
  EXPECT_EQ(copy.get_piece(E8), BLACK_KING);
  EXPECT_EQ(copy.get_piece(D4), WHITE_QUEEN);
  EXPECT_EQ(copy.get_piece(A7), BLACK_PAWN);

  // Empty squares
  EXPECT_EQ(copy.get_piece(E2), std::nullopt);

  // Ensure equality operator confirms identical boards
  EXPECT_EQ(copy, original);
}

/**
 * @test Move assignment preserves board state.
 * @brief Confirms that moving a board via assignment produces an identical board.
 */
TEST(BoardCopyTest, MoveAssignment) {
  Board original = Board::Empty();

  // Set up pieces and state
  original.set_piece(E1, WHITE_KING);
  original.set_piece(E8, BLACK_KING);
  original.set_piece(C3, WHITE_BISHOP);
  original.set_piece(H7, BLACK_PAWN);

  Board copy = Board::Empty();
  copy = std::move(original);  // Copy assignment

  // Piece positions
  EXPECT_EQ(copy.get_piece(E1), WHITE_KING);
  EXPECT_EQ(copy.get_piece(E8), BLACK_KING);
  EXPECT_EQ(copy.get_piece(C3), WHITE_BISHOP);
  EXPECT_EQ(copy.get_piece(H7), BLACK_PAWN);

  // Empty squares
  EXPECT_EQ(copy.get_piece(D4), std::nullopt);

  // Ensure equality operator confirms identical boards
  EXPECT_EQ(copy, original);
}

/**
 * @test Move produces independent boards.
 * @brief Modifying the copy does not affect the original board.
 */
TEST(BoardCopyTest, IndependenceAfterMove) {
  Board original = Board::Empty();
  original.set_piece(E1, WHITE_KING);

  Board copy(std::move(original));
  copy.set_piece(E2, WHITE_PAWN);

  // Test that copy has the new piece
  EXPECT_EQ(copy.get_piece(E2), WHITE_PAWN);

  // Original is in a valid state; we can safely assign new values
  original.set_piece(D1, WHITE_QUEEN);
  EXPECT_EQ(original.get_piece(D1), WHITE_QUEEN);
}
