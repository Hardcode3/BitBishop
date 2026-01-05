#include <gtest/gtest.h>

#include <bitbishop/attacks/checkers.hpp>
#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test No checkers on empty board.
 * @brief Confirms compute_checkers() returns empty bitboard when no
 *        attacking pieces present.
 */
TEST(ComputeCheckersTest, NoCheckersEmptyBoard) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);

  Bitboard checkers = compute_checkers(board, E4, Color::BLACK);

  EXPECT_EQ(checkers, Bitboard::Zeros());
  EXPECT_EQ(checkers.count(), 0);
}

/**
 * @test No checkers when pieces too far.
 * @brief Confirms compute_checkers() returns empty when attacker pieces
 *        cannot reach king.
 */
TEST(ComputeCheckersTest, NoCheckersWhenTooFar) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(A8, BLACK_ROOK);
  board.set_piece(H2, BLACK_BISHOP);

  Bitboard checkers = compute_checkers(board, E4, Color::BLACK);

  EXPECT_EQ(checkers, Bitboard::Zeros());
}

/**
 * @test Knight checking king.
 * @brief Confirms compute_checkers() detects knight check.
 */
TEST(ComputeCheckersTest, KnightCheck) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(D2, BLACK_KNIGHT);

  Bitboard checkers = compute_checkers(board, E4, Color::BLACK);

  EXPECT_EQ(checkers.count(), 1);
  EXPECT_TRUE(checkers.test(D2));
}

/**
 * @test Multiple knights checking king.
 * @brief Confirms compute_checkers() detects multiple knight checks.
 */
TEST(ComputeCheckersTest, MultipleKnightChecks) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(D2, BLACK_KNIGHT);
  board.set_piece(F2, BLACK_KNIGHT);

  Bitboard checkers = compute_checkers(board, E4, Color::BLACK);

  EXPECT_EQ(checkers.count(), 2);
  EXPECT_TRUE(checkers.test(D2));
  EXPECT_TRUE(checkers.test(F2));
}

/**
 * @test White pawn checking black king.
 * @brief Confirms compute_checkers() detects white pawn check.
 */
TEST(ComputeCheckersTest, WhitePawnCheck) {
  Board board = Board::Empty();
  board.set_piece(E5, BLACK_KING);
  board.set_piece(D4, WHITE_PAWN);

  Bitboard checkers = compute_checkers(board, E5, Color::WHITE);

  EXPECT_EQ(checkers.count(), 1);
  EXPECT_TRUE(checkers.test(D4));
}

/**
 * @test Black pawn checking white king.
 * @brief Confirms compute_checkers() detects black pawn check.
 */
TEST(ComputeCheckersTest, BlackPawnCheck) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(D5, BLACK_PAWN);

  Bitboard checkers = compute_checkers(board, E4, Color::BLACK);

  EXPECT_EQ(checkers.count(), 1);
  EXPECT_TRUE(checkers.test(D5));
}

/**
 * @test Multiple pawn checks.
 * @brief Confirms compute_checkers() detects both pawns checking king.
 */
TEST(ComputeCheckersTest, MultiplePawnChecks) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(D5, BLACK_PAWN);
  board.set_piece(F5, BLACK_PAWN);

  Bitboard checkers = compute_checkers(board, E4, Color::BLACK);

  EXPECT_EQ(checkers.count(), 2);
  EXPECT_TRUE(checkers.test(D5));
  EXPECT_TRUE(checkers.test(F5));
}

/**
 * @test Pawn not checking from wrong direction.
 * @brief Confirms compute_checkers() does not detect pawn check when
 *        pawn cannot attack king's square.
 */
TEST(ComputeCheckersTest, PawnNotCheckingWrongDirection) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E5, BLACK_PAWN);  // Pawns don't attack forward

  Bitboard checkers = compute_checkers(board, E4, Color::BLACK);

  EXPECT_EQ(checkers, Bitboard::Zeros());
}

/**
 * @test King checking king (adjacent kings).
 * @brief Confirms compute_checkers() detects when enemy king is adjacent.
 */
TEST(ComputeCheckersTest, KingCheckingKing) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E5, BLACK_KING);

  Bitboard checkers = compute_checkers(board, E4, Color::BLACK);

  EXPECT_EQ(checkers.count(), 1);
  EXPECT_TRUE(checkers.test(E5));
}

/**
 * @test Rook checking king orthogonally.
 * @brief Confirms compute_checkers() detects rook check along file.
 */
TEST(ComputeCheckersTest, RookCheckAlongFile) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E8, BLACK_ROOK);

  Bitboard checkers = compute_checkers(board, E1, Color::BLACK);

  EXPECT_EQ(checkers.count(), 1);
  EXPECT_TRUE(checkers.test(E8));
}

/**
 * @test Rook checking king along rank.
 * @brief Confirms compute_checkers() detects rook check along rank.
 */
TEST(ComputeCheckersTest, RookCheckAlongRank) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(A4, BLACK_ROOK);

  Bitboard checkers = compute_checkers(board, E4, Color::BLACK);

  EXPECT_EQ(checkers.count(), 1);
  EXPECT_TRUE(checkers.test(A4));
}

/**
 * @test Rook blocked does not check.
 * @brief Confirms compute_checkers() does not detect check when rook
 *        is blocked.
 */
TEST(ComputeCheckersTest, RookBlockedNoCheck) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, BLACK_PAWN);
  board.set_piece(E8, BLACK_ROOK);

  Bitboard checkers = compute_checkers(board, E1, Color::BLACK);

  EXPECT_EQ(checkers, Bitboard::Zeros());
}

/**
 * @test Bishop checking king diagonally.
 * @brief Confirms compute_checkers() detects bishop check along diagonal.
 */
TEST(ComputeCheckersTest, BishopCheckAlongDiagonal) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(A5, BLACK_BISHOP);

  Bitboard checkers = compute_checkers(board, E1, Color::BLACK);

  EXPECT_EQ(checkers.count(), 1);
  EXPECT_TRUE(checkers.test(A5));
}

/**
 * @test Bishop blocked does not check.
 * @brief Confirms compute_checkers() does not detect check when bishop
 *        is blocked.
 */
TEST(ComputeCheckersTest, BishopBlockedNoCheck) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(C3, BLACK_PAWN);
  board.set_piece(A5, BLACK_BISHOP);

  Bitboard checkers = compute_checkers(board, E1, Color::BLACK);

  EXPECT_EQ(checkers, Bitboard::Zeros());
}

/**
 * @test Queen checking orthogonally.
 * @brief Confirms compute_checkers() detects queen check along rank/file.
 */
TEST(ComputeCheckersTest, QueenCheckOrthogonally) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E8, BLACK_QUEEN);

  Bitboard checkers = compute_checkers(board, E1, Color::BLACK);

  EXPECT_EQ(checkers.count(), 1);
  EXPECT_TRUE(checkers.test(E8));
}

/**
 * @test Queen checking diagonally.
 * @brief Confirms compute_checkers() detects queen check along diagonal.
 */
TEST(ComputeCheckersTest, QueenCheckDiagonally) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(H4, BLACK_QUEEN);

  Bitboard checkers = compute_checkers(board, E1, Color::BLACK);

  EXPECT_EQ(checkers.count(), 1);
  EXPECT_TRUE(checkers.test(H4));
}

/**
 * @test Queen blocked does not check.
 * @brief Confirms compute_checkers() does not detect check when queen
 *        is blocked.
 */
TEST(ComputeCheckersTest, QueenBlockedNoCheck) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, WHITE_PAWN);
  board.set_piece(E8, BLACK_QUEEN);

  Bitboard checkers = compute_checkers(board, E1, Color::BLACK);

  EXPECT_EQ(checkers, Bitboard::Zeros());
}

/**
 * @test Double check detected.
 * @brief Confirms compute_checkers() detects multiple pieces checking king.
 */
TEST(ComputeCheckersTest, DoubleCheck) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E8, BLACK_ROOK);
  board.set_piece(D3, BLACK_KNIGHT);

  Bitboard checkers = compute_checkers(board, E1, Color::BLACK);

  EXPECT_EQ(checkers.count(), 2);
  EXPECT_TRUE(checkers.test(E8));
  EXPECT_TRUE(checkers.test(D3));
}

/**
 * @test Triple check detected.
 * @brief Confirms compute_checkers() detects all checking pieces.
 */
TEST(ComputeCheckersTest, TripleCheck) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E8, BLACK_ROOK);
  board.set_piece(A4, BLACK_ROOK);
  board.set_piece(D2, BLACK_KNIGHT);

  Bitboard checkers = compute_checkers(board, E4, Color::BLACK);

  EXPECT_EQ(checkers.count(), 3);
  EXPECT_TRUE(checkers.test(E8));
  EXPECT_TRUE(checkers.test(A4));
  EXPECT_TRUE(checkers.test(D2));
}

/**
 * @test Discovered check piece not marked as checker.
 * @brief Confirms compute_checkers() only marks direct checkers, not
 *        pieces that moved to discover check.
 */
TEST(ComputeCheckersTest, DiscoveredCheckNotMarked) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(D2, BLACK_KNIGHT);  // Moved, discovered check
  board.set_piece(E8, BLACK_ROOK);    // Actual checker

  Bitboard checkers = compute_checkers(board, E1, Color::BLACK);

  // Should only detect the rook as checker
  EXPECT_TRUE(checkers.test(E8));
}

/**
 * @test White attacking black king.
 * @brief Confirms compute_checkers() works when white is attacker.
 */
TEST(ComputeCheckersTest, WhiteAttackingBlack) {
  Board board = Board::Empty();
  board.set_piece(E8, BLACK_KING);
  board.set_piece(E1, WHITE_ROOK);

  Bitboard checkers = compute_checkers(board, E8, Color::WHITE);

  EXPECT_EQ(checkers.count(), 1);
  EXPECT_TRUE(checkers.test(E1));
}

/**
 * @test Black attacking white king.
 * @brief Confirms compute_checkers() works when black is attacker.
 */
TEST(ComputeCheckersTest, BlackAttackingWhite) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E8, BLACK_ROOK);

  Bitboard checkers = compute_checkers(board, E1, Color::BLACK);

  EXPECT_EQ(checkers.count(), 1);
  EXPECT_TRUE(checkers.test(E8));
}

/**
 * @test Friendly pieces do not check own king.
 * @brief Confirms compute_checkers() only considers pieces of attacker color.
 */
TEST(ComputeCheckersTest, FriendlyPiecesNotCheckers) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E8, WHITE_ROOK);  // White rook, not checking white king

  // Asking for black attackers, but only white rook exists
  Bitboard checkers = compute_checkers(board, E1, Color::BLACK);

  EXPECT_EQ(checkers, Bitboard::Zeros());
}

/**
 * @test King on corner with checker.
 * @brief Confirms compute_checkers() works with king on corner square.
 */
TEST(ComputeCheckersTest, KingOnCornerWithChecker) {
  Board board = Board::Empty();
  board.set_piece(A1, WHITE_KING);
  board.set_piece(A8, BLACK_ROOK);

  Bitboard checkers = compute_checkers(board, A1, Color::BLACK);

  EXPECT_EQ(checkers.count(), 1);
  EXPECT_TRUE(checkers.test(A8));
}

/**
 * @test King on edge with checker.
 * @brief Confirms compute_checkers() works with king on edge square.
 */
TEST(ComputeCheckersTest, KingOnEdgeWithChecker) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E8, BLACK_ROOK);

  Bitboard checkers = compute_checkers(board, E1, Color::BLACK);

  EXPECT_EQ(checkers.count(), 1);
  EXPECT_TRUE(checkers.test(E8));
}

/**
 * @test Multiple sliders on same line only one checking.
 * @brief Confirms compute_checkers() correctly identifies which slider
 *        is actually checking.
 */
TEST(ComputeCheckersTest, MultipleSlidersSameLine) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, BLACK_ROOK);
  board.set_piece(E8, BLACK_ROOK);

  Bitboard checkers = compute_checkers(board, E1, Color::BLACK);

  // Only E4 rook is checking (E8 is blocked by E4)
  EXPECT_EQ(checkers.count(), 1);
  EXPECT_TRUE(checkers.test(E4));
}

/**
 * @test Complex position with multiple potential checkers.
 * @brief Confirms compute_checkers() correctly identifies all checking
 *        pieces in complex position.
 */
TEST(ComputeCheckersTest, ComplexPositionMultipleCheckers) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(D2, BLACK_KNIGHT);  // Checking
  board.set_piece(E8, BLACK_ROOK);    // Checking
  board.set_piece(A4, BLACK_ROOK);    // Checking
  board.set_piece(A8, BLACK_BISHOP);  // Not checking (blocked)
  board.set_piece(C6, BLACK_PAWN);    // Blocker

  Bitboard checkers = compute_checkers(board, E4, Color::BLACK);

  EXPECT_EQ(checkers.count(), 3);
  EXPECT_TRUE(checkers.test(D2));
  EXPECT_TRUE(checkers.test(E8));
  EXPECT_TRUE(checkers.test(A4));
  EXPECT_FALSE(checkers.test(A8));
}

/**
 * @test Starting position no checkers.
 * @brief Confirms compute_checkers() returns empty for starting position.
 */
TEST(ComputeCheckersTest, StartingPositionNoCheckers) {
  Board board = Board::StartingPosition();

  Bitboard checkers_white = compute_checkers(board, E1, Color::BLACK);
  Bitboard checkers_black = compute_checkers(board, E8, Color::WHITE);

  EXPECT_EQ(checkers_white, Bitboard::Zeros());
  EXPECT_EQ(checkers_black, Bitboard::Zeros());
}

/**
 * @test Slider attack blocked by friendly piece.
 * @brief Confirms compute_checkers() does not detect check when friendly
 *        piece blocks slider.
 */
TEST(ComputeCheckersTest, SliderBlockedByFriendlyPiece) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, BLACK_PAWN);
  board.set_piece(E8, BLACK_ROOK);

  Bitboard checkers = compute_checkers(board, E1, Color::BLACK);

  EXPECT_EQ(checkers, Bitboard::Zeros());
}

/**
 * @test Slider attack blocked by enemy piece.
 * @brief Confirms compute_checkers() does not detect check when enemy
 *        piece (relative to king) blocks slider.
 */
TEST(ComputeCheckersTest, SliderBlockedByEnemyPiece) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, WHITE_PAWN);
  board.set_piece(E8, BLACK_ROOK);

  Bitboard checkers = compute_checkers(board, E1, Color::BLACK);

  EXPECT_EQ(checkers, Bitboard::Zeros());
}

/**
 * @test All piece types as checkers simultaneously.
 * @brief Confirms compute_checkers() can detect checks from all piece types.
 */
TEST(ComputeCheckersTest, AllPieceTypesAsCheckers) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(D2, BLACK_KNIGHT);  // Knight check
  board.set_piece(D5, BLACK_PAWN);    // Pawn check
  board.set_piece(E8, BLACK_ROOK);    // Rook check
  board.set_piece(H7, BLACK_BISHOP);  // Bishop check
  board.set_piece(H4, BLACK_QUEEN);   // Queen check

  Bitboard checkers = compute_checkers(board, E4, Color::BLACK);

  EXPECT_EQ(checkers.count(), 5);
  EXPECT_TRUE(checkers.test(D2));
  EXPECT_TRUE(checkers.test(D5));
  EXPECT_TRUE(checkers.test(E8));
  EXPECT_TRUE(checkers.test(H7));
  EXPECT_TRUE(checkers.test(H4));
}
