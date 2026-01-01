#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test Empty boards are equal.
 * @brief Confirms two empty boards compare as equal.
 */
TEST(BoardEqualityTest, EmptyBoardsEqual) {
  Board board1 = Board::Empty();
  Board board2 = Board::Empty();

  EXPECT_TRUE(board1 == board2);
  EXPECT_FALSE(board1 != board2);
}

/**
 * @test Starting positions are equal.
 * @brief Confirms two starting position boards compare as equal.
 */
TEST(BoardEqualityTest, StartingPositionsEqual) {
  Board board1 = Board::StartingPosition();
  Board board2 = Board::StartingPosition();

  EXPECT_TRUE(board1 == board2);
  EXPECT_FALSE(board1 != board2);
}

/**
 * @test Board equals itself.
 * @brief Confirms a board compares equal to itself.
 */
TEST(BoardEqualityTest, BoardEqualsItself) {
  Board board = Board::StartingPosition();

  EXPECT_TRUE(board == board);
  EXPECT_FALSE(board != board);
}

/**
 * @test Different piece placement makes boards unequal.
 * @brief Confirms boards with different pieces are not equal.
 */
TEST(BoardEqualityTest, DifferentPiecePlacement) {
  Board board1 = Board::Empty();
  Board board2 = Board::Empty();

  board1.set_piece(E4, WHITE_PAWN);
  board2.set_piece(E5, WHITE_PAWN);

  EXPECT_FALSE(board1 == board2);
  EXPECT_TRUE(board1 != board2);
}

/**
 * @test Different piece types makes boards unequal.
 * @brief Confirms boards with different piece types on same square are not equal.
 */
TEST(BoardEqualityTest, DifferentPieceTypes) {
  Board board1 = Board::Empty();
  Board board2 = Board::Empty();

  board1.set_piece(E4, WHITE_PAWN);
  board2.set_piece(E4, WHITE_KNIGHT);

  EXPECT_FALSE(board1 == board2);
  EXPECT_TRUE(board1 != board2);
}

/**
 * @test Different piece colors makes boards unequal.
 * @brief Confirms boards with different colored pieces on same square are not equal.
 */
TEST(BoardEqualityTest, DifferentPieceColors) {
  Board board1 = Board::Empty();
  Board board2 = Board::Empty();

  board1.set_piece(E4, WHITE_PAWN);
  board2.set_piece(E4, BLACK_PAWN);

  EXPECT_FALSE(board1 == board2);
  EXPECT_TRUE(board1 != board2);
}

/**
 * @test Extra piece makes boards unequal.
 * @brief Confirms board with additional piece is not equal.
 */
TEST(BoardEqualityTest, ExtraPieceMakesUnequal) {
  Board board1 = Board::Empty();
  Board board2 = Board::Empty();

  board1.set_piece(E4, WHITE_PAWN);
  board1.set_piece(D4, WHITE_PAWN);
  board2.set_piece(E4, WHITE_PAWN);

  EXPECT_FALSE(board1 == board2);
  EXPECT_TRUE(board1 != board2);
}

/**
 * @test Missing piece makes boards unequal.
 * @brief Confirms board with missing piece is not equal.
 */
TEST(BoardEqualityTest, MissingPieceMakesUnequal) {
  Board board1 = Board::Empty();
  Board board2 = Board::Empty();

  board1.set_piece(E4, WHITE_PAWN);
  board2.set_piece(E4, WHITE_PAWN);
  board2.set_piece(D4, WHITE_PAWN);

  EXPECT_FALSE(board1 == board2);
  EXPECT_TRUE(board1 != board2);
}

/**
 * @test Boards with same pieces are equal.
 * @brief Confirms boards with identical piece placement are equal.
 */
TEST(BoardEqualityTest, SamePiecesEqual) {
  Board board1 = Board::Empty();
  Board board2 = Board::Empty();

  board1.set_piece(E4, WHITE_PAWN);
  board1.set_piece(E5, BLACK_PAWN);
  board1.set_piece(D4, WHITE_KNIGHT);

  board2.set_piece(E4, WHITE_PAWN);
  board2.set_piece(E5, BLACK_PAWN);
  board2.set_piece(D4, WHITE_KNIGHT);

  EXPECT_TRUE(board1 == board2);
  EXPECT_FALSE(board1 != board2);
}

/**
 * @test Complex position equality.
 * @brief Confirms equality works for complex board positions.
 */
TEST(BoardEqualityTest, ComplexPositionEquality) {
  Board board1 = Board::Empty();
  Board board2 = Board::Empty();

  // Set up complex position
  board1.set_piece(E1, WHITE_KING);
  board1.set_piece(A1, WHITE_ROOK);
  board1.set_piece(H1, WHITE_ROOK);
  board1.set_piece(D2, WHITE_PAWN);
  board1.set_piece(E2, WHITE_PAWN);
  board1.set_piece(F2, WHITE_PAWN);

  board1.set_piece(E8, BLACK_KING);
  board1.set_piece(A8, BLACK_ROOK);
  board1.set_piece(D7, BLACK_PAWN);
  board1.set_piece(E7, BLACK_PAWN);

  board2.set_piece(E1, WHITE_KING);
  board2.set_piece(A1, WHITE_ROOK);
  board2.set_piece(H1, WHITE_ROOK);
  board2.set_piece(D2, WHITE_PAWN);
  board2.set_piece(E2, WHITE_PAWN);
  board2.set_piece(F2, WHITE_PAWN);

  board2.set_piece(E8, BLACK_KING);
  board2.set_piece(A8, BLACK_ROOK);
  board2.set_piece(D7, BLACK_PAWN);
  board2.set_piece(E7, BLACK_PAWN);

  EXPECT_TRUE(board1 == board2);
  EXPECT_FALSE(board1 != board2);
}

/**
 * @test Equality is symmetric.
 * @brief Confirms that if board1 == board2, then board2 == board1.
 */
TEST(BoardEqualityTest, EqualityIsSymmetric) {
  Board board1 = Board::StartingPosition();
  Board board2 = Board::StartingPosition();

  EXPECT_TRUE(board1 == board2);
  EXPECT_TRUE(board2 == board1);
}

/**
 * @test Inequality is symmetric.
 * @brief Confirms that if board1 != board2, then board2 != board1.
 */
TEST(BoardEqualityTest, InequalityIsSymmetric) {
  Board board1 = Board::Empty();
  Board board2 = Board::Empty();

  board1.set_piece(E4, WHITE_PAWN);

  EXPECT_TRUE(board1 != board2);
  EXPECT_TRUE(board2 != board1);
}

/**
 * @test Equality is transitive.
 * @brief Confirms that if board1 == board2 and board2 == board3, then board1 == board3.
 */
TEST(BoardEqualityTest, EqualityIsTransitive) {
  Board board1 = Board::StartingPosition();
  Board board2 = Board::StartingPosition();
  Board board3 = Board::StartingPosition();

  EXPECT_TRUE(board1 == board2);
  EXPECT_TRUE(board2 == board3);
  EXPECT_TRUE(board1 == board3);
}

/**
 * @test Empty vs non-empty board.
 * @brief Confirms empty board is not equal to board with pieces.
 */
TEST(BoardEqualityTest, EmptyVsNonEmpty) {
  Board board1 = Board::Empty();
  Board board2 = Board::StartingPosition();

  EXPECT_FALSE(board1 == board2);
  EXPECT_TRUE(board1 != board2);
}

/**
 * @test Order of piece placement doesn't matter.
 * @brief Confirms boards are equal regardless of order pieces were placed.
 */
TEST(BoardEqualityTest, OrderOfPlacementDoesNotMatter) {
  Board board1 = Board::Empty();
  Board board2 = Board::Empty();

  // Place pieces in different order
  board1.set_piece(E4, WHITE_PAWN);
  board1.set_piece(D4, WHITE_KNIGHT);
  board1.set_piece(F4, WHITE_BISHOP);

  board2.set_piece(F4, WHITE_BISHOP);
  board2.set_piece(E4, WHITE_PAWN);
  board2.set_piece(D4, WHITE_KNIGHT);

  EXPECT_TRUE(board1 == board2);
  EXPECT_FALSE(board1 != board2);
}

/**
 * @test All squares checked for equality.
 * @brief Confirms all 64 squares are considered in equality check.
 */
TEST(BoardEqualityTest, AllSquaresChecked) {
  Board board1 = Board::Empty();
  Board board2 = Board::Empty();

  // Place piece on corner square
  board1.set_piece(A1, WHITE_PAWN);
  board2.set_piece(A1, WHITE_PAWN);

  EXPECT_TRUE(board1 == board2);

  // Change piece on opposite corner
  board2.set_piece(H8, BLACK_PAWN);

  EXPECT_FALSE(board1 == board2);
  EXPECT_TRUE(board1 != board2);
}
