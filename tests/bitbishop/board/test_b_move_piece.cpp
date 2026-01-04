#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test Move piece to empty square.
 * @brief Confirms move_piece() correctly moves a piece to an empty square.
 */
TEST(BoardMovePieceTest, MoveToEmptySquare) {
  Board board = Board::Empty();
  board.set_piece(E2, WHITE_PAWN);

  board.move_piece(E2, E4);

  EXPECT_EQ(board.get_piece(E2), std::nullopt);
  EXPECT_EQ(board.get_piece(E4), WHITE_PAWN);
}

/**
 * @test Move piece captures enemy piece.
 * @brief Confirms move_piece() removes captured piece and places moving piece.
 */
TEST(BoardMovePieceTest, CaptureEnemyPiece) {
  Board board = Board::Empty();
  board.set_piece(E2, WHITE_PAWN);
  board.set_piece(E4, BLACK_PAWN);

  board.move_piece(E2, E4);

  EXPECT_EQ(board.get_piece(E2), std::nullopt);
  EXPECT_EQ(board.get_piece(E4), WHITE_PAWN);
}

/**
 * @test Move piece captures friendly piece.
 * @brief Confirms move_piece() removes captured friendly piece (though illegal
 *        in chess, the function should handle it).
 */
TEST(BoardMovePieceTest, CaptureFriendlyPiece) {
  Board board = Board::Empty();
  board.set_piece(E2, WHITE_PAWN);
  board.set_piece(E4, WHITE_KNIGHT);

  board.move_piece(E2, E4);

  EXPECT_EQ(board.get_piece(E2), std::nullopt);
  EXPECT_EQ(board.get_piece(E4), WHITE_PAWN);
}

/**
 * @test Move from empty square does nothing.
 * @brief Confirms move_piece() silently returns when source square is empty.
 */
TEST(BoardMovePieceTest, MoveFromEmptySquare) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KNIGHT);

  board.move_piece(E2, E3);

  EXPECT_EQ(board.get_piece(E2), std::nullopt);
  EXPECT_EQ(board.get_piece(E3), std::nullopt);
  EXPECT_EQ(board.get_piece(E4), WHITE_KNIGHT);
}

/**
 * @test Move white pawn.
 * @brief Confirms move_piece() correctly handles white pawn movement.
 */
TEST(BoardMovePieceTest, MoveWhitePawn) {
  Board board = Board::Empty();
  board.set_piece(A2, WHITE_PAWN);

  board.move_piece(A2, A4);

  EXPECT_EQ(board.get_piece(A2), std::nullopt);
  EXPECT_EQ(board.get_piece(A4), WHITE_PAWN);
}

/**
 * @test Move black pawn.
 * @brief Confirms move_piece() correctly handles black pawn movement.
 */
TEST(BoardMovePieceTest, MoveBlackPawn) {
  Board board = Board::Empty();
  board.set_piece(A7, BLACK_PAWN);

  board.move_piece(A7, A5);

  EXPECT_EQ(board.get_piece(A7), std::nullopt);
  EXPECT_EQ(board.get_piece(A5), BLACK_PAWN);
}

/**
 * @test Move white knight.
 * @brief Confirms move_piece() correctly handles white knight movement.
 */
TEST(BoardMovePieceTest, MoveWhiteKnight) {
  Board board = Board::Empty();
  board.set_piece(B1, WHITE_KNIGHT);

  board.move_piece(B1, C3);

  EXPECT_EQ(board.get_piece(B1), std::nullopt);
  EXPECT_EQ(board.get_piece(C3), WHITE_KNIGHT);
}

/**
 * @test Move black knight.
 * @brief Confirms move_piece() correctly handles black knight movement.
 */
TEST(BoardMovePieceTest, MoveBlackKnight) {
  Board board = Board::Empty();
  board.set_piece(B8, BLACK_KNIGHT);

  board.move_piece(B8, C6);

  EXPECT_EQ(board.get_piece(B8), std::nullopt);
  EXPECT_EQ(board.get_piece(C6), BLACK_KNIGHT);
}

/**
 * @test Move white bishop.
 * @brief Confirms move_piece() correctly handles white bishop movement.
 */
TEST(BoardMovePieceTest, MoveWhiteBishop) {
  Board board = Board::Empty();
  board.set_piece(C1, WHITE_BISHOP);

  board.move_piece(C1, F4);

  EXPECT_EQ(board.get_piece(C1), std::nullopt);
  EXPECT_EQ(board.get_piece(F4), WHITE_BISHOP);
}

/**
 * @test Move black bishop.
 * @brief Confirms move_piece() correctly handles black bishop movement.
 */
TEST(BoardMovePieceTest, MoveBlackBishop) {
  Board board = Board::Empty();
  board.set_piece(C8, BLACK_BISHOP);

  board.move_piece(C8, F5);

  EXPECT_EQ(board.get_piece(C8), std::nullopt);
  EXPECT_EQ(board.get_piece(F5), BLACK_BISHOP);
}

/**
 * @test Move white rook.
 * @brief Confirms move_piece() correctly handles white rook movement.
 */
TEST(BoardMovePieceTest, MoveWhiteRook) {
  Board board = Board::Empty();
  board.set_piece(A1, WHITE_ROOK);

  board.move_piece(A1, A4);

  EXPECT_EQ(board.get_piece(A1), std::nullopt);
  EXPECT_EQ(board.get_piece(A4), WHITE_ROOK);
}

/**
 * @test Move black rook.
 * @brief Confirms move_piece() correctly handles black rook movement.
 */
TEST(BoardMovePieceTest, MoveBlackRook) {
  Board board = Board::Empty();
  board.set_piece(A8, BLACK_ROOK);

  board.move_piece(A8, A5);

  EXPECT_EQ(board.get_piece(A8), std::nullopt);
  EXPECT_EQ(board.get_piece(A5), BLACK_ROOK);
}

/**
 * @test Move white queen.
 * @brief Confirms move_piece() correctly handles white queen movement.
 */
TEST(BoardMovePieceTest, MoveWhiteQueen) {
  Board board = Board::Empty();
  board.set_piece(D1, WHITE_QUEEN);

  board.move_piece(D1, D5);

  EXPECT_EQ(board.get_piece(D1), std::nullopt);
  EXPECT_EQ(board.get_piece(D5), WHITE_QUEEN);
}

/**
 * @test Move black queen.
 * @brief Confirms move_piece() correctly handles black queen movement.
 */
TEST(BoardMovePieceTest, MoveBlackQueen) {
  Board board = Board::Empty();
  board.set_piece(D8, BLACK_QUEEN);

  board.move_piece(D8, D4);

  EXPECT_EQ(board.get_piece(D8), std::nullopt);
  EXPECT_EQ(board.get_piece(D4), BLACK_QUEEN);
}

/**
 * @test Move white king.
 * @brief Confirms move_piece() correctly handles white king movement.
 */
TEST(BoardMovePieceTest, MoveWhiteKing) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);

  board.move_piece(E1, E2);

  EXPECT_EQ(board.get_piece(E1), std::nullopt);
  EXPECT_EQ(board.get_piece(E2), WHITE_KING);
}

/**
 * @test Move black king.
 * @brief Confirms move_piece() correctly handles black king movement.
 */
TEST(BoardMovePieceTest, MoveBlackKing) {
  Board board = Board::Empty();
  board.set_piece(E8, BLACK_KING);

  board.move_piece(E8, E7);

  EXPECT_EQ(board.get_piece(E8), std::nullopt);
  EXPECT_EQ(board.get_piece(E7), BLACK_KING);
}

/**
 * @test Move piece to same square.
 * @brief Confirms move_piece() handles moving a piece to its own square
 *        (effectively a no-op, piece should remain).
 */
TEST(BoardMovePieceTest, MoveToSameSquare) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KNIGHT);

  board.move_piece(E4, E4);

  EXPECT_EQ(board.get_piece(E4), WHITE_KNIGHT);
}

/**
 * @test Move across board edges.
 * @brief Confirms move_piece() correctly handles moves from one edge to another.
 */
TEST(BoardMovePieceTest, MoveAcrossBoardEdges) {
  Board board = Board::Empty();
  board.set_piece(A1, WHITE_ROOK);

  board.move_piece(A1, H8);

  EXPECT_EQ(board.get_piece(A1), std::nullopt);
  EXPECT_EQ(board.get_piece(H8), WHITE_ROOK);
}

/**
 * @test Multiple sequential moves.
 * @brief Confirms move_piece() correctly handles multiple moves in sequence.
 */
TEST(BoardMovePieceTest, MultipleSequentialMoves) {
  Board board = Board::Empty();
  board.set_piece(E2, WHITE_PAWN);

  board.move_piece(E2, E3);
  board.move_piece(E3, E4);
  board.move_piece(E4, E5);

  EXPECT_EQ(board.get_piece(E2), std::nullopt);
  EXPECT_EQ(board.get_piece(E3), std::nullopt);
  EXPECT_EQ(board.get_piece(E4), std::nullopt);
  EXPECT_EQ(board.get_piece(E5), WHITE_PAWN);
}

/**
 * @test Move does not affect other pieces.
 * @brief Confirms move_piece() only affects the from and to squares.
 */
TEST(BoardMovePieceTest, DoesNotAffectOtherPieces) {
  Board board = Board::Empty();
  board.set_piece(E2, WHITE_PAWN);
  board.set_piece(D2, WHITE_PAWN);
  board.set_piece(F2, WHITE_PAWN);
  board.set_piece(E3, BLACK_PAWN);

  board.move_piece(E2, E4);

  EXPECT_EQ(board.get_piece(D2), WHITE_PAWN);
  EXPECT_EQ(board.get_piece(F2), WHITE_PAWN);
  EXPECT_EQ(board.get_piece(E3), BLACK_PAWN);
  EXPECT_EQ(board.get_piece(E4), WHITE_PAWN);
}

/**
 * @test Bitboard consistency after move.
 * @brief Confirms move_piece() maintains correct bitboard state.
 */
TEST(BoardMovePieceTest, BitboardConsistency) {
  Board board = Board::Empty();
  board.set_piece(E2, WHITE_PAWN);
  board.set_piece(E7, BLACK_PAWN);

  board.move_piece(E2, E4);

  Bitboard white_pawns = board.pawns(Color::WHITE);
  Bitboard black_pawns = board.pawns(Color::BLACK);

  EXPECT_TRUE(white_pawns.test(E4));
  EXPECT_FALSE(white_pawns.test(E2));
  EXPECT_TRUE(black_pawns.test(E7));
  EXPECT_EQ(white_pawns.count(), 1);
  EXPECT_EQ(black_pawns.count(), 1);
}

/**
 * @test Occupied squares after move.
 * @brief Confirms move_piece() correctly updates occupied bitboard.
 */
TEST(BoardMovePieceTest, OccupiedSquaresAfterMove) {
  Board board = Board::Empty();
  board.set_piece(E2, WHITE_PAWN);

  board.move_piece(E2, E4);

  Bitboard occupied = board.occupied();

  EXPECT_TRUE(occupied.test(E4));
  EXPECT_FALSE(occupied.test(E2));
  EXPECT_EQ(occupied.count(), 1);
}

/**
 * @test Occupied squares after capture.
 * @brief Confirms move_piece() correctly updates occupied bitboard after capture.
 */
TEST(BoardMovePieceTest, OccupiedSquaresAfterCapture) {
  Board board = Board::Empty();
  board.set_piece(E2, WHITE_PAWN);
  board.set_piece(E4, BLACK_PAWN);

  Bitboard occupied_before = board.occupied();
  EXPECT_EQ(occupied_before.count(), 2);

  board.move_piece(E2, E4);

  Bitboard occupied_after = board.occupied();
  EXPECT_TRUE(occupied_after.test(E4));
  EXPECT_FALSE(occupied_after.test(E2));
  EXPECT_EQ(occupied_after.count(), 1);
}

/**
 * @test Move in starting position.
 * @brief Confirms move_piece() works correctly from standard opening position.
 */
TEST(BoardMovePieceTest, MoveInStartingPosition) {
  Board board = Board::StartingPosition();

  board.move_piece(E2, E4);

  EXPECT_EQ(board.get_piece(E2), std::nullopt);
  EXPECT_EQ(board.get_piece(E4), WHITE_PAWN);

  // Verify other pieces remain unchanged
  EXPECT_EQ(board.get_piece(D2), WHITE_PAWN);
  EXPECT_EQ(board.get_piece(F2), WHITE_PAWN);
  EXPECT_EQ(board.get_piece(E1), WHITE_KING);
}

/**
 * @test Capture in starting position.
 * @brief Confirms move_piece() handles captures from starting position.
 */
TEST(BoardMovePieceTest, CaptureInStartingPosition) {
  Board board = Board::StartingPosition();

  // Move white pawn forward twice
  board.move_piece(E2, E4);
  board.move_piece(E4, E5);

  // Capture black pawn on E7 (illegal move, but testing function behavior)
  board.move_piece(E5, E7);

  EXPECT_EQ(board.get_piece(E7), WHITE_PAWN);
  EXPECT_EQ(board.get_piece(E5), std::nullopt);
}

/**
 * @test Move updates piece type bitboards correctly.
 * @brief Confirms move_piece() updates specific piece type bitboards.
 */
TEST(BoardMovePieceTest, UpdatesPieceTypeBitboards) {
  Board board = Board::Empty();
  board.set_piece(B1, WHITE_KNIGHT);

  Bitboard knights_before = board.knights(Color::WHITE);
  EXPECT_TRUE(knights_before.test(B1));
  EXPECT_FALSE(knights_before.test(C3));

  board.move_piece(B1, C3);

  Bitboard knights_after = board.knights(Color::WHITE);
  EXPECT_FALSE(knights_after.test(B1));
  EXPECT_TRUE(knights_after.test(C3));
  EXPECT_EQ(knights_after.count(), 1);
}

/**
 * @test Capture updates both color bitboards.
 * @brief Confirms move_piece() correctly updates bitboards for both colors
 *        during a capture.
 */
TEST(BoardMovePieceTest, CaptureUpdatesBothColors) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_ROOK);
  board.set_piece(E8, BLACK_ROOK);

  Bitboard white_rooks_before = board.rooks(Color::WHITE);
  Bitboard black_rooks_before = board.rooks(Color::BLACK);
  EXPECT_EQ(white_rooks_before.count(), 1);
  EXPECT_EQ(black_rooks_before.count(), 1);

  board.move_piece(E4, E8);

  Bitboard white_rooks_after = board.rooks(Color::WHITE);
  Bitboard black_rooks_after = board.rooks(Color::BLACK);
  EXPECT_TRUE(white_rooks_after.test(E8));
  EXPECT_FALSE(white_rooks_after.test(E4));
  EXPECT_EQ(white_rooks_after.count(), 1);
  EXPECT_EQ(black_rooks_after.count(), 0);
}
