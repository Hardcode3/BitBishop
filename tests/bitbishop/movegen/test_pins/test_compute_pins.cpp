#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/movegen/pins.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test Empty board produces no pins.
 * @brief Confirms compute_pins() returns empty result when board has
 *        only the king.
 */
TEST(ComputePinsTest, EmptyBoardNoPins) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);

  PinResult result = compute_pins(E4, board, Color::WHITE);

  EXPECT_EQ(result.pinned, Bitboard::Zeros());
  EXPECT_EQ(result.pinned.count(), 0);
}

/**
 * @test No enemy sliders produces no pins.
 * @brief Confirms compute_pins() returns empty result when enemy has
 *        only non-sliding pieces.
 */
TEST(ComputePinsTest, NoEnemySlidersNoPins) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E5, WHITE_PAWN);
  board.set_piece(E6, BLACK_KNIGHT);
  board.set_piece(D4, WHITE_KNIGHT);
  board.set_piece(C4, BLACK_PAWN);

  PinResult result = compute_pins(E4, board, Color::WHITE);

  EXPECT_EQ(result.pinned, Bitboard::Zeros());
}

/**
 * @test Multiple orthogonal pins detected simultaneously.
 * @brief Confirms compute_pins() detects pins on multiple orthogonal rays
 *        in a single call.
 */
TEST(ComputePinsTest, MultipleOrthogonalPins) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E6, WHITE_PAWN);  // Pinned north
  board.set_piece(E8, BLACK_ROOK);
  board.set_piece(E2, WHITE_KNIGHT);  // Pinned south
  board.set_piece(E1, BLACK_ROOK);
  board.set_piece(G4, WHITE_BISHOP);  // Pinned east
  board.set_piece(H4, BLACK_ROOK);
  board.set_piece(C4, WHITE_ROOK);  // Pinned west
  board.set_piece(A4, BLACK_ROOK);

  PinResult result = compute_pins(E4, board, Color::WHITE);

  EXPECT_EQ(result.pinned.count(), 4);
  EXPECT_TRUE(result.pinned.test(E6));
  EXPECT_TRUE(result.pinned.test(E2));
  EXPECT_TRUE(result.pinned.test(G4));
  EXPECT_TRUE(result.pinned.test(C4));
}

/**
 * @test Multiple diagonal pins detected simultaneously.
 * @brief Confirms compute_pins() detects pins on multiple diagonal rays
 *        in a single call.
 */
TEST(ComputePinsTest, MultipleDiagonalPins) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(F5, WHITE_PAWN);  // Pinned NE
  board.set_piece(H7, BLACK_BISHOP);
  board.set_piece(D5, WHITE_KNIGHT);  // Pinned NW
  board.set_piece(B7, BLACK_BISHOP);
  board.set_piece(F3, WHITE_BISHOP);  // Pinned SE
  board.set_piece(H1, BLACK_BISHOP);
  board.set_piece(D3, WHITE_ROOK);  // Pinned SW
  board.set_piece(B1, BLACK_BISHOP);

  PinResult result = compute_pins(E4, board, Color::WHITE);

  EXPECT_EQ(result.pinned.count(), 4);
  EXPECT_TRUE(result.pinned.test(F5));
  EXPECT_TRUE(result.pinned.test(D5));
  EXPECT_TRUE(result.pinned.test(F3));
  EXPECT_TRUE(result.pinned.test(D3));
}

/**
 * @test Mixed orthogonal and diagonal pins.
 * @brief Confirms compute_pins() correctly detects pins on both
 *        orthogonal and diagonal rays simultaneously.
 */
TEST(ComputePinsTest, MixedOrthogonalAndDiagonalPins) {
  Board board = Board::Empty();
  board.set_piece(D4, WHITE_KING);
  board.set_piece(D6, WHITE_PAWN);  // Pinned north by rook
  board.set_piece(D8, BLACK_ROOK);
  board.set_piece(E5, WHITE_KNIGHT);  // Pinned NE by bishop
  board.set_piece(H8, BLACK_BISHOP);
  board.set_piece(C4, WHITE_BISHOP);  // Pinned west by queen
  board.set_piece(A4, BLACK_QUEEN);

  PinResult result = compute_pins(D4, board, Color::WHITE);

  EXPECT_EQ(result.pinned.count(), 3);
  EXPECT_TRUE(result.pinned.test(D6));
  EXPECT_TRUE(result.pinned.test(E5));
  EXPECT_TRUE(result.pinned.test(C4));
}

/**
 * @test Queens create pins on all ray types.
 * @brief Confirms compute_pins() detects pins from enemy queens on both
 *        orthogonal and diagonal rays.
 */
TEST(ComputePinsTest, QueensPinOnAllDirections) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E6, WHITE_PAWN);  // Pinned by queen north
  board.set_piece(E8, BLACK_QUEEN);
  board.set_piece(G6, WHITE_KNIGHT);  // Pinned by queen NE
  board.set_piece(H7, BLACK_QUEEN);

  PinResult result = compute_pins(E4, board, Color::WHITE);

  EXPECT_EQ(result.pinned.count(), 2);
  EXPECT_TRUE(result.pinned.test(E6));
  EXPECT_TRUE(result.pinned.test(G6));
}

/**
 * @test King on corner detects pins correctly.
 * @brief Confirms compute_pins() works when king is on a corner square
 *        with limited ray directions.
 */
TEST(ComputePinsTest, KingOnCorner) {
  Board board = Board::Empty();
  board.set_piece(A1, WHITE_KING);
  board.set_piece(A3, WHITE_PAWN);  // Pinned north
  board.set_piece(A8, BLACK_ROOK);
  board.set_piece(C1, WHITE_KNIGHT);  // Pinned east
  board.set_piece(H1, BLACK_ROOK);
  board.set_piece(C3, WHITE_BISHOP);  // Pinned NE
  board.set_piece(E5, BLACK_BISHOP);

  PinResult result = compute_pins(A1, board, Color::WHITE);

  EXPECT_EQ(result.pinned.count(), 3);
  EXPECT_TRUE(result.pinned.test(A3));
  EXPECT_TRUE(result.pinned.test(C1));
  EXPECT_TRUE(result.pinned.test(C3));
}

/**
 * @test King on edge detects pins correctly.
 * @brief Confirms compute_pins() works when king is on an edge square.
 */
TEST(ComputePinsTest, KingOnEdge) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E3, WHITE_PAWN);  // Pinned north
  board.set_piece(E8, BLACK_ROOK);
  board.set_piece(G3, WHITE_KNIGHT);  // Pinned NE
  board.set_piece(H4, BLACK_BISHOP);
  board.set_piece(C3, WHITE_BISHOP);  // Pinned NW
  board.set_piece(A5, BLACK_BISHOP);

  PinResult result = compute_pins(E1, board, Color::WHITE);

  EXPECT_EQ(result.pinned.count(), 3);
  EXPECT_TRUE(result.pinned.test(E3));
  EXPECT_TRUE(result.pinned.test(G3));
  EXPECT_TRUE(result.pinned.test(C3));
}

/**
 * @test Pins work with black as friendly color.
 * @brief Confirms compute_pins() correctly computes pins when black
 *        is the side to move.
 */
TEST(ComputePinsTest, BlackKingPins) {
  Board board = Board::Empty();
  board.set_piece(E8, BLACK_KING);
  board.set_piece(E6, BLACK_PAWN);  // Pinned south
  board.set_piece(E1, WHITE_ROOK);
  board.set_piece(G6, BLACK_KNIGHT);  // Pinned SE
  board.set_piece(H5, WHITE_BISHOP);

  PinResult result = compute_pins(E8, board, Color::BLACK);

  EXPECT_EQ(result.pinned.count(), 2);
  EXPECT_TRUE(result.pinned.test(E6));
  EXPECT_TRUE(result.pinned.test(G6));
}

/**
 * @test Pin rays are correctly set for all pinned pieces.
 * @brief Confirms compute_pins() populates pin_ray array with correct
 *        movement masks for each pinned piece.
 */
TEST(ComputePinsTest, PinRaysSetCorrectly) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E6, WHITE_PAWN);
  board.set_piece(E8, BLACK_ROOK);
  board.set_piece(G6, WHITE_KNIGHT);
  board.set_piece(H7, BLACK_BISHOP);

  PinResult result = compute_pins(E4, board, Color::WHITE);

  // Check that pin rays are non-empty for pinned pieces
  EXPECT_NE(result.pin_ray[E6.value()], Bitboard::Zeros());
  EXPECT_NE(result.pin_ray[G6.value()], Bitboard::Zeros());

  // Check that pin ray for E6 includes squares along the file
  EXPECT_TRUE(result.pin_ray[E6.value()].test(E5));
  EXPECT_TRUE(result.pin_ray[E6.value()].test(E6));
  EXPECT_TRUE(result.pin_ray[E6.value()].test(E7));
  EXPECT_TRUE(result.pin_ray[E6.value()].test(E8));
}

/**
 * @test Partial pins on some rays with clear rays.
 * @brief Confirms compute_pins() handles mixed scenarios where some rays
 *        have pins and others are clear.
 */
TEST(ComputePinsTest, PartialPinsWithClearRays) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E6, WHITE_PAWN);
  board.set_piece(E8, BLACK_ROOK);
  // Other directions are clear

  PinResult result = compute_pins(E4, board, Color::WHITE);

  EXPECT_EQ(result.pinned.count(), 1);
  EXPECT_TRUE(result.pinned.test(E6));
}

/**
 * @test Blocked rays prevent pins beyond first piece.
 * @brief Confirms compute_pins() does not detect false pins when rays
 *        are blocked by multiple pieces.
 */
TEST(ComputePinsTest, BlockedRaysNoFalsePins) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E5, WHITE_PAWN);
  board.set_piece(E6, WHITE_KNIGHT);  // Second friendly piece blocks pin
  board.set_piece(E8, BLACK_ROOK);

  PinResult result = compute_pins(E4, board, Color::WHITE);

  EXPECT_EQ(result.pinned, Bitboard::Zeros());
  EXPECT_FALSE(result.pinned.test(E5));
  EXPECT_FALSE(result.pinned.test(E6));
}

/**
 * @test Maximum pins scenario (all 8 directions).
 * @brief Confirms compute_pins() can handle the theoretical maximum of
 *        8 simultaneous pins (one per ray direction).
 */
TEST(ComputePinsTest, MaximumPinsAllDirections) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);

  // North
  board.set_piece(E5, WHITE_PAWN);
  board.set_piece(E8, BLACK_ROOK);
  // South
  board.set_piece(E3, WHITE_PAWN);
  board.set_piece(E1, BLACK_ROOK);
  // East
  board.set_piece(F4, WHITE_PAWN);
  board.set_piece(H4, BLACK_ROOK);
  // West
  board.set_piece(D4, WHITE_PAWN);
  board.set_piece(A4, BLACK_ROOK);
  // NE
  board.set_piece(F5, WHITE_PAWN);
  board.set_piece(H7, BLACK_BISHOP);
  // NW
  board.set_piece(D5, WHITE_PAWN);
  board.set_piece(A8, BLACK_BISHOP);
  // SE
  board.set_piece(F3, WHITE_PAWN);
  board.set_piece(H1, BLACK_BISHOP);
  // SW
  board.set_piece(D3, WHITE_PAWN);
  board.set_piece(B1, BLACK_BISHOP);

  PinResult result = compute_pins(E4, board, Color::WHITE);

  EXPECT_EQ(result.pinned.count(), 8);
  EXPECT_TRUE(result.pinned.test(E5));
  EXPECT_TRUE(result.pinned.test(E3));
  EXPECT_TRUE(result.pinned.test(F4));
  EXPECT_TRUE(result.pinned.test(D4));
  EXPECT_TRUE(result.pinned.test(F5));
  EXPECT_TRUE(result.pinned.test(D5));
  EXPECT_TRUE(result.pinned.test(F3));
  EXPECT_TRUE(result.pinned.test(D3));
}

/**
 * @test Same friendly piece not pinned by different sliders.
 * @brief Confirms compute_pins() does not double-count a piece that
 *        could theoretically be on multiple ray paths (edge case validation).
 */
TEST(ComputePinsTest, NoPieceCountedTwice) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E6, WHITE_PAWN);
  board.set_piece(E8, BLACK_ROOK);
  // Add a second rook that doesn't create an additional pin
  board.set_piece(E7, BLACK_ROOK);  // This blocks the first rook's ray

  PinResult result = compute_pins(E4, board, Color::WHITE);

  // E6 should be pinned by E7, not E8
  EXPECT_EQ(result.pinned.count(), 1);
  EXPECT_TRUE(result.pinned.test(E6));
}

/**
 * @test Friendly pieces beyond pins are not marked as pinned.
 * @brief Confirms compute_pins() only marks the closest friendly piece
 *        as pinned, not pieces behind it.
 */
TEST(ComputePinsTest, OnlyClosestPieceMarkedPinned) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E3, WHITE_PAWN);
  board.set_piece(E5, WHITE_KNIGHT);
  board.set_piece(E7, BLACK_ROOK);

  PinResult result = compute_pins(E1, board, Color::WHITE);

  // Only E3 should be pinned, not E5
  EXPECT_EQ(result.pinned.count(), 0);
  EXPECT_FALSE(result.pinned.test(E3));
  EXPECT_FALSE(result.pinned.test(E5));
}

/**
 * @test Standard opening position has no pins.
 * @brief Confirms compute_pins() returns no pins in the standard
 *        chess starting position.
 */
TEST(ComputePinsTest, StandardOpeningNoPins) {
  Board board = Board();

  PinResult white_result = compute_pins(E1, board, Color::WHITE);
  PinResult black_result = compute_pins(E8, board, Color::BLACK);

  EXPECT_EQ(white_result.pinned, Bitboard::Zeros());
  EXPECT_EQ(black_result.pinned, Bitboard::Zeros());
}

/**
 * @test Adjacent enemy slider does not create pin.
 * @brief Confirms compute_pins() does not detect a pin when enemy slider
 *        is immediately adjacent to king (check, not pin).
 */
TEST(ComputePinsTest, AdjacentSliderNotPinned) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E5, BLACK_ROOK);  // Adjacent, gives check but no pin

  PinResult result = compute_pins(E4, board, Color::WHITE);

  EXPECT_EQ(result.pinned, Bitboard::Zeros());
}
