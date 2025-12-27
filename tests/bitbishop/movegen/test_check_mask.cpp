#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/movegen/check_mask.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test No checkers returns full board mask.
 * @brief Confirms compute_check_mask() returns all ones when not in check.
 */
TEST(CheckMaskTest, NoCheckersFullMask) {
  Board board = Board::Empty();
  Bitboard no_checkers = Bitboard::Zeros();

  Bitboard mask = compute_check_mask(E1, no_checkers, board);

  EXPECT_EQ(mask, Bitboard::Ones());
  EXPECT_EQ(mask.count(), 64);
}

/**
 * @test Double check returns empty mask.
 * @brief Confirms compute_check_mask() returns zeros for double check.
 */
TEST(CheckMaskTest, DoubleCheckEmptyMask) {
  Board board = Board::Empty();
  Bitboard checkers = Bitboard::Zeros();
  checkers.set(D2);
  checkers.set(F2);

  Bitboard mask = compute_check_mask(E1, checkers, board);

  EXPECT_EQ(mask, Bitboard::Zeros());
  EXPECT_EQ(mask.count(), 0);
}

/**
 * @test Knight check returns only checker square.
 * @brief Confirms knight checks produce mask with only the checker square.
 */
TEST(CheckMaskTest, KnightCheckSingleSquare) {
  Board board = Board::Empty();
  board.set_piece(D2, BLACK_KNIGHT);

  Bitboard checkers = Bitboard::Zeros();
  checkers.set(D2);

  Bitboard mask = compute_check_mask(E4, checkers, board);

  EXPECT_EQ(mask.count(), 1);
  EXPECT_TRUE(mask.test(D2));
  EXPECT_FALSE(mask.test(E4));
}

/**
 * @test Pawn check returns only checker square.
 * @brief Confirms pawn checks produce mask with only the checker square.
 */
TEST(CheckMaskTest, PawnCheckSingleSquare) {
  Board board = Board::Empty();
  board.set_piece(D4, BLACK_PAWN);

  Bitboard checkers = Bitboard::Zeros();
  checkers.set(D4);

  Bitboard mask = compute_check_mask(E5, checkers, board);

  EXPECT_EQ(mask.count(), 1);
  EXPECT_TRUE(mask.test(D4));
  EXPECT_FALSE(mask.test(E5));
}

/**
 * @test Rook check on same rank includes ray and checker.
 * @brief Confirms rook checks include all squares between king and checker.
 */
TEST(CheckMaskTest, RookCheckSameRank) {
  Board board = Board::Empty();
  board.set_piece(A4, BLACK_ROOK);

  Bitboard checkers = Bitboard::Zeros();
  checkers.set(A4);

  Bitboard mask = compute_check_mask(E4, checkers, board);

  // Should include A4 (checker) and B4, C4, D4 (between)
  EXPECT_TRUE(mask.test(A4));
  EXPECT_TRUE(mask.test(B4));
  EXPECT_TRUE(mask.test(C4));
  EXPECT_TRUE(mask.test(D4));
  EXPECT_FALSE(mask.test(E4));  // King square not included
  EXPECT_FALSE(mask.test(F4));  // Beyond king
}

/**
 * @test Rook check on same file includes ray and checker.
 * @brief Confirms rook checks include all squares between king and checker.
 */
TEST(CheckMaskTest, RookCheckSameFile) {
  Board board = Board::Empty();
  board.set_piece(E1, BLACK_ROOK);

  Bitboard checkers = Bitboard::Zeros();
  checkers.set(E1);

  Bitboard mask = compute_check_mask(E5, checkers, board);

  // Should include E1 (checker) and E2, E3, E4 (between)
  EXPECT_TRUE(mask.test(E1));
  EXPECT_TRUE(mask.test(E2));
  EXPECT_TRUE(mask.test(E3));
  EXPECT_TRUE(mask.test(E4));
  EXPECT_FALSE(mask.test(E5));  // King square not included
  EXPECT_FALSE(mask.test(E6));  // Beyond king
}

/**
 * @test Bishop check on diagonal includes ray and checker.
 * @brief Confirms bishop checks include all squares between king and checker.
 */
TEST(CheckMaskTest, BishopCheckDiagonal) {
  Board board = Board::Empty();
  board.set_piece(A1, BLACK_BISHOP);

  Bitboard checkers = Bitboard::Zeros();
  checkers.set(A1);

  Bitboard mask = compute_check_mask(D4, checkers, board);

  // Should include A1 (checker) and B2, C3 (between)
  EXPECT_TRUE(mask.test(A1));
  EXPECT_TRUE(mask.test(B2));
  EXPECT_TRUE(mask.test(C3));
  EXPECT_FALSE(mask.test(D4));  // King square not included
  EXPECT_FALSE(mask.test(E5));  // Beyond king
}

/**
 * @test Queen check on rank includes ray and checker.
 * @brief Confirms queen checks work like rook on ranks.
 */
TEST(CheckMaskTest, QueenCheckRank) {
  Board board = Board::Empty();
  board.set_piece(A4, BLACK_QUEEN);

  Bitboard checkers = Bitboard::Zeros();
  checkers.set(A4);

  Bitboard mask = compute_check_mask(E4, checkers, board);

  EXPECT_TRUE(mask.test(A4));
  EXPECT_TRUE(mask.test(B4));
  EXPECT_TRUE(mask.test(C4));
  EXPECT_TRUE(mask.test(D4));
  EXPECT_FALSE(mask.test(E4));
}

/**
 * @test Queen check on diagonal includes ray and checker.
 * @brief Confirms queen checks work like bishop on diagonals.
 */
TEST(CheckMaskTest, QueenCheckDiagonal) {
  Board board = Board::Empty();
  board.set_piece(B2, BLACK_QUEEN);

  Bitboard checkers = Bitboard::Zeros();
  checkers.set(B2);

  Bitboard mask = compute_check_mask(E5, checkers, board);

  EXPECT_TRUE(mask.test(B2));
  EXPECT_TRUE(mask.test(C3));
  EXPECT_TRUE(mask.test(D4));
  EXPECT_FALSE(mask.test(E5));
}

/**
 * @test Adjacent rook check includes only checker.
 * @brief Confirms adjacent slider checks have no intermediate squares.
 */
TEST(CheckMaskTest, AdjacentRookCheck) {
  Board board = Board::Empty();
  board.set_piece(E3, BLACK_ROOK);

  Bitboard checkers = Bitboard::Zeros();
  checkers.set(E3);

  Bitboard mask = compute_check_mask(E4, checkers, board);

  // Only the checker square, no squares between
  EXPECT_EQ(mask.count(), 1);
  EXPECT_TRUE(mask.test(E3));
}

/**
 * @test Adjacent bishop check includes only checker.
 * @brief Confirms adjacent slider checks have no intermediate squares.
 */
TEST(CheckMaskTest, AdjacentBishopCheck) {
  Board board = Board::Empty();
  board.set_piece(D3, BLACK_BISHOP);

  Bitboard checkers = Bitboard::Zeros();
  checkers.set(D3);

  Bitboard mask = compute_check_mask(E4, checkers, board);

  // Only the checker square, no squares between
  EXPECT_EQ(mask.count(), 1);
  EXPECT_TRUE(mask.test(D3));
}

/**
 * @test Long-range rook check includes many squares.
 * @brief Confirms distant slider checks include full ray.
 */
TEST(CheckMaskTest, LongRangeRookCheck) {
  Board board = Board::Empty();
  board.set_piece(A4, BLACK_ROOK);

  Bitboard checkers = Bitboard::Zeros();
  checkers.set(A4);

  Bitboard mask = compute_check_mask(H4, checkers, board);

  // Should have 7 squares: A4 + 6 between
  EXPECT_EQ(mask.count(), 7);
  EXPECT_TRUE(mask.test(A4));
  EXPECT_TRUE(mask.test(B4));
  EXPECT_TRUE(mask.test(C4));
  EXPECT_TRUE(mask.test(D4));
  EXPECT_TRUE(mask.test(E4));
  EXPECT_TRUE(mask.test(F4));
  EXPECT_TRUE(mask.test(G4));
  EXPECT_FALSE(mask.test(H4));
}

/**
 * @test Long-range bishop check includes many squares.
 * @brief Confirms distant slider checks include full ray.
 */
TEST(CheckMaskTest, LongRangeBishopCheck) {
  Board board = Board::Empty();
  board.set_piece(A1, BLACK_BISHOP);

  Bitboard checkers = Bitboard::Zeros();
  checkers.set(A1);

  Bitboard mask = compute_check_mask(H8, checkers, board);

  // Should have 7 squares: A1 + 6 between
  EXPECT_EQ(mask.count(), 7);
  EXPECT_TRUE(mask.test(A1));
  EXPECT_TRUE(mask.test(B2));
  EXPECT_TRUE(mask.test(C3));
  EXPECT_TRUE(mask.test(D4));
  EXPECT_TRUE(mask.test(E5));
  EXPECT_TRUE(mask.test(F6));
  EXPECT_TRUE(mask.test(G7));
  EXPECT_FALSE(mask.test(H8));
}

/**
 * @test White knight check detection.
 * @brief Confirms white knights are correctly identified as non-sliding.
 */
TEST(CheckMaskTest, WhiteKnightCheck) {
  Board board = Board::Empty();
  board.set_piece(C3, WHITE_KNIGHT);

  Bitboard checkers = Bitboard::Zeros();
  checkers.set(C3);

  Bitboard mask = compute_check_mask(E4, checkers, board);

  EXPECT_EQ(mask.count(), 1);
  EXPECT_TRUE(mask.test(C3));
}

/**
 * @test White pawn check detection.
 * @brief Confirms white pawns are correctly identified as non-sliding.
 */
TEST(CheckMaskTest, WhitePawnCheck) {
  Board board = Board::Empty();
  board.set_piece(D3, WHITE_PAWN);

  Bitboard checkers = Bitboard::Zeros();
  checkers.set(D3);

  Bitboard mask = compute_check_mask(E4, checkers, board);

  EXPECT_EQ(mask.count(), 1);
  EXPECT_TRUE(mask.test(D3));
}

/**
 * @test Black knight check detection.
 * @brief Confirms black knights are correctly identified as non-sliding.
 */
TEST(CheckMaskTest, BlackKnightCheck) {
  Board board = Board::Empty();
  board.set_piece(F6, BLACK_KNIGHT);

  Bitboard checkers = Bitboard::Zeros();
  checkers.set(F6);

  Bitboard mask = compute_check_mask(E4, checkers, board);

  EXPECT_EQ(mask.count(), 1);
  EXPECT_TRUE(mask.test(F6));
}

/**
 * @test Black pawn check detection.
 * @brief Confirms black pawns are correctly identified as non-sliding.
 */
TEST(CheckMaskTest, BlackPawnCheck) {
  Board board = Board::Empty();
  board.set_piece(F5, BLACK_PAWN);

  Bitboard checkers = Bitboard::Zeros();
  checkers.set(F5);

  Bitboard mask = compute_check_mask(E4, checkers, board);

  EXPECT_EQ(mask.count(), 1);
  EXPECT_TRUE(mask.test(F5));
}

/**
 * @test Empty board rook check.
 * @brief Confirms sliders work correctly on empty board.
 */
TEST(CheckMaskTest, EmptyBoardRookCheck) {
  Board board = Board::Empty();  // Empty board
  board.set_piece(A1, BLACK_ROOK);

  Bitboard checkers = Bitboard::Zeros();
  checkers.set(A1);

  Bitboard mask = compute_check_mask(A8, checkers, board);

  // A1 + 6 squares between
  EXPECT_EQ(mask.count(), 7);
  EXPECT_TRUE(mask.test(A1));
  EXPECT_TRUE(mask.test(A2));
  EXPECT_TRUE(mask.test(A7));
}

/**
 * @test Triple check returns empty mask.
 * @brief Confirms multiple checkers (>2) return empty mask.
 */
TEST(CheckMaskTest, TripleCheckEmptyMask) {
  Board board = Board::Empty();
  Bitboard checkers = Bitboard::Zeros();
  checkers.set(D2);
  checkers.set(F2);
  checkers.set(E3);

  Bitboard mask = compute_check_mask(E1, checkers, board);

  EXPECT_EQ(mask, Bitboard::Zeros());
  EXPECT_EQ(mask.count(), 0);
}

/**
 * @test King in corner with distant checker.
 * @brief Confirms check masks work correctly for corner kings.
 */
TEST(CheckMaskTest, CornerKingDistantChecker) {
  Board board = Board::Empty();
  board.set_piece(A1, BLACK_ROOK);

  Bitboard checkers = Bitboard::Zeros();
  checkers.set(A1);

  Bitboard mask = compute_check_mask(A8, checkers, board);

  EXPECT_EQ(mask.count(), 7);
  EXPECT_TRUE(mask.test(A1));
  EXPECT_TRUE(mask.test(A2));
  EXPECT_TRUE(mask.test(A7));
  EXPECT_FALSE(mask.test(A8));
}

/**
 * @test Checker square always included in mask.
 * @brief Confirms checker square is always part of the mask for single checks.
 */
TEST(CheckMaskTest, CheckerSquareAlwaysIncluded) {
  Board board = Board::Empty();

  // Test with various piece types
  const Square checker_squares[] = {D2, F6, A1, H8};
  const Piece::Type piece_types[] = {Piece::Type::KNIGHT, Piece::Type::PAWN, Piece::Type::ROOK, Piece::Type::BISHOP};

  for (int i = 0; i < 4; ++i) {
    Board test_board;
    test_board.set_piece(checker_squares[i], Piece(piece_types[i], Color::BLACK));

    Bitboard checkers = Bitboard::Zeros();
    checkers.set(checker_squares[i]);

    Bitboard mask = compute_check_mask(E4, checkers, test_board);

    EXPECT_TRUE(mask.test(checker_squares[i])) << "Checker square not included for piece type " << i;
  }
}
