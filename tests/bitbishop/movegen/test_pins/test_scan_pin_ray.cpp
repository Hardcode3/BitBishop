#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/movegen/pins.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test No blockers on ray produces no pins.
 * @brief Confirms scan_pin_ray() does not mark any pins when the ray is empty.
 */
TEST(ScanPinRayTest, NoBlockersNoPins) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);

  PinRay ray{Lookups::ROOK_NORTH_RAYS[E1.value()], RayType::ROOK, RayDir::N};
  PinResult result;

  scan_pin_ray(E1, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned, Bitboard::Zeros());
  EXPECT_EQ(result.pinned.count(), 0);
}

/**
 * @test Single enemy blocker produces no pins.
 * @brief Confirms scan_pin_ray() does not mark pins when only an enemy piece
 *        is on the ray (no friendly piece to pin).
 */
TEST(ScanPinRayTest, SingleEnemyBlockerNoPins) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E5, BLACK_ROOK);

  PinRay ray{Lookups::ROOK_NORTH_RAYS[E1.value()], RayType::ROOK, RayDir::N};
  PinResult result;

  scan_pin_ray(E1, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned, Bitboard::Zeros());
  EXPECT_EQ(result.pinned.count(), 0);
}

/**
 * @test Single friendly blocker produces no pins.
 * @brief Confirms scan_pin_ray() does not mark pins when only a friendly piece
 *        is on the ray (no enemy slider beyond it).
 */
TEST(ScanPinRayTest, SingleFriendlyBlockerNoPins) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E3, WHITE_PAWN);

  PinRay ray{Lookups::ROOK_NORTH_RAYS[E1.value()], RayType::ROOK, RayDir::N};
  PinResult result;

  scan_pin_ray(E1, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned, Bitboard::Zeros());
  EXPECT_EQ(result.pinned.count(), 0);
}

/**
 * @test Classic rook pin pattern detected.
 * @brief Confirms scan_pin_ray() correctly detects a pin when:
 *        king -> friendly piece -> enemy rook are aligned orthogonally.
 */
TEST(ScanPinRayTest, ClassicRookPinDetected) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E3, WHITE_PAWN);
  board.set_piece(E5, BLACK_ROOK);

  PinRay ray{Lookups::ROOK_NORTH_RAYS[E1.value()], RayType::ROOK, RayDir::N};
  PinResult result;

  scan_pin_ray(E1, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned.count(), 1);
  EXPECT_TRUE(result.pinned.test(E3));
  EXPECT_FALSE(result.pinned.test(E5));
}

/**
 * @test Classic queen pin on rook ray detected.
 * @brief Confirms scan_pin_ray() correctly detects a pin when:
 *        king -> friendly piece -> enemy queen are aligned orthogonally.
 */
TEST(ScanPinRayTest, ClassicQueenPinOnRookRay) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E3, WHITE_KNIGHT);
  board.set_piece(E7, BLACK_QUEEN);

  PinRay ray{Lookups::ROOK_NORTH_RAYS[E1.value()], RayType::ROOK, RayDir::N};
  PinResult result;

  scan_pin_ray(E1, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned.count(), 1);
  EXPECT_TRUE(result.pinned.test(E3));
}

/**
 * @test Classic bishop pin pattern detected.
 * @brief Confirms scan_pin_ray() correctly detects a pin when:
 *        king -> friendly piece -> enemy bishop are aligned diagonally.
 */
TEST(ScanPinRayTest, ClassicBishopPinDetected) {
  Board board = Board::Empty();
  board.set_piece(A1, WHITE_KING);
  board.set_piece(C3, WHITE_PAWN);
  board.set_piece(E5, BLACK_BISHOP);

  PinRay ray{Lookups::BISHOP_NORTHEAST_RAYS[A1.value()], RayType::BISHOP, RayDir::NE};
  PinResult result;

  scan_pin_ray(A1, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned.count(), 1);
  EXPECT_TRUE(result.pinned.test(C3));
}

/**
 * @test Classic queen pin on bishop ray detected.
 * @brief Confirms scan_pin_ray() correctly detects a pin when:
 *        king -> friendly piece -> enemy queen are aligned diagonally.
 */
TEST(ScanPinRayTest, ClassicQueenPinOnBishopRay) {
  Board board = Board::Empty();
  board.set_piece(A1, WHITE_KING);
  board.set_piece(D4, WHITE_ROOK);
  board.set_piece(G7, BLACK_QUEEN);

  PinRay ray{Lookups::BISHOP_NORTHEAST_RAYS[A1.value()], RayType::BISHOP, RayDir::NE};
  PinResult result;

  scan_pin_ray(A1, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned.count(), 1);
  EXPECT_TRUE(result.pinned.test(D4));
}

/**
 * @test Wrong slider type produces no pin.
 * @brief Confirms scan_pin_ray() does not detect a pin when the enemy
 *        slider type doesn't match the ray type (bishop on rook ray).
 */
TEST(ScanPinRayTest, WrongSliderTypeNoPinBishopOnRookRay) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E3, WHITE_PAWN);
  board.set_piece(E5, BLACK_BISHOP);

  PinRay ray{Lookups::ROOK_NORTH_RAYS[E1.value()], RayType::ROOK, RayDir::N};
  PinResult result;

  scan_pin_ray(E1, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned, Bitboard::Zeros());
}

/**
 * @test Wrong slider type produces no pin.
 * @brief Confirms scan_pin_ray() does not detect a pin when the enemy
 *        slider type doesn't match the ray type (rook on bishop ray).
 */
TEST(ScanPinRayTest, WrongSliderTypeNoPinRookOnBishopRay) {
  Board board = Board::Empty();
  board.set_piece(A1, WHITE_KING);
  board.set_piece(C3, WHITE_PAWN);
  board.set_piece(E5, BLACK_ROOK);

  PinRay ray{Lookups::BISHOP_NORTHEAST_RAYS[A1.value()], RayType::BISHOP, RayDir::NE};
  PinResult result;

  scan_pin_ray(A1, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned, Bitboard::Zeros());
}

/**
 * @test Non-sliding enemy piece produces no pin.
 * @brief Confirms scan_pin_ray() does not detect a pin when the second
 *        blocker is not a sliding piece (e.g., knight).
 */
TEST(ScanPinRayTest, NonSlidingEnemyNoPinKnight) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E3, WHITE_PAWN);
  board.set_piece(E5, BLACK_KNIGHT);

  PinRay ray{Lookups::ROOK_NORTH_RAYS[E1.value()], RayType::ROOK, RayDir::N};
  PinResult result;

  scan_pin_ray(E1, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned, Bitboard::Zeros());
}

/**
 * @test Two friendly pieces block pin.
 * @brief Confirms scan_pin_ray() does not detect a pin when two friendly
 *        pieces are between the king and enemy slider.
 */
TEST(ScanPinRayTest, TwoFriendlyPiecesNoPins) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E2, WHITE_PAWN);
  board.set_piece(E3, WHITE_KNIGHT);
  board.set_piece(E5, BLACK_ROOK);

  PinRay ray{Lookups::ROOK_NORTH_RAYS[E1.value()], RayType::ROOK, RayDir::N};
  PinResult result;

  scan_pin_ray(E1, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned, Bitboard::Zeros());
}

/**
 * @test Pin ray includes attacker square.
 * @brief Confirms scan_pin_ray() sets pin_ray to include both the path
 *        between king and attacker, and the attacker's square itself.
 */
TEST(ScanPinRayTest, PinRayIncludesAttackerSquare) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E3, WHITE_PAWN);
  board.set_piece(E6, BLACK_ROOK);

  PinRay ray{Lookups::ROOK_NORTH_RAYS[E1.value()], RayType::ROOK, RayDir::N};
  PinResult result;

  scan_pin_ray(E1, ray, board, Color::WHITE, result);

  Bitboard expected_ray = Bitboard::Zeros();
  expected_ray.set(E2);
  expected_ray.set(E3);
  expected_ray.set(E4);
  expected_ray.set(E5);
  expected_ray.set(E6);

  EXPECT_EQ(result.pin_ray[E3.value()], expected_ray);
}

/**
 * @test Pin ray excludes king square.
 * @brief Confirms scan_pin_ray() sets pin_ray to not include the king's square.
 */
TEST(ScanPinRayTest, PinRayExcludesKingSquare) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E3, WHITE_PAWN);
  board.set_piece(E5, BLACK_ROOK);

  PinRay ray{Lookups::ROOK_NORTH_RAYS[E1.value()], RayType::ROOK, RayDir::N};
  PinResult result;

  scan_pin_ray(E1, ray, board, Color::WHITE, result);

  EXPECT_FALSE(result.pin_ray[E3.value()].test(E1));
}

/**
 * @test Southward pin detected correctly.
 * @brief Confirms scan_pin_ray() correctly detects pins when scanning
 *        from king toward lower ranks.
 */
TEST(ScanPinRayTest, SouthwardPinDetected) {
  Board board = Board::Empty();
  board.set_piece(E8, WHITE_KING);
  board.set_piece(E6, WHITE_PAWN);
  board.set_piece(E3, BLACK_ROOK);

  PinRay ray{Lookups::ROOK_SOUTH_RAYS[E8.value()], RayType::ROOK, RayDir::S};
  PinResult result;

  scan_pin_ray(E8, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned.count(), 1);
  EXPECT_TRUE(result.pinned.test(E6));
}

/**
 * @test Westward pin detected correctly.
 * @brief Confirms scan_pin_ray() correctly detects pins when scanning
 *        from king toward lower files.
 */
TEST(ScanPinRayTest, WestwardPinDetected) {
  Board board = Board::Empty();
  board.set_piece(H4, WHITE_KING);
  board.set_piece(F4, WHITE_PAWN);
  board.set_piece(C4, BLACK_ROOK);

  PinRay ray{Lookups::ROOK_WEST_RAYS[H4.value()], RayType::ROOK, RayDir::W};
  PinResult result;

  scan_pin_ray(H4, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned.count(), 1);
  EXPECT_TRUE(result.pinned.test(F4));
}

/**
 * @test Eastward pin detected correctly.
 * @brief Confirms scan_pin_ray() correctly detects pins when scanning
 *        from king toward higher files.
 */
TEST(ScanPinRayTest, EastwardPinDetected) {
  Board board = Board::Empty();
  board.set_piece(A4, WHITE_KING);
  board.set_piece(C4, WHITE_PAWN);
  board.set_piece(F4, BLACK_ROOK);

  PinRay ray{Lookups::ROOK_EAST_RAYS[A4.value()], RayType::ROOK, RayDir::E};
  PinResult result;

  scan_pin_ray(A4, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned.count(), 1);
  EXPECT_TRUE(result.pinned.test(C4));
}

/**
 * @test Southwest diagonal pin detected correctly.
 * @brief Confirms scan_pin_ray() correctly detects pins along the
 *        southwest diagonal.
 */
TEST(ScanPinRayTest, SouthwestPinDetected) {
  Board board = Board::Empty();
  board.set_piece(H8, WHITE_KING);
  board.set_piece(F6, WHITE_PAWN);
  board.set_piece(C3, BLACK_BISHOP);

  PinRay ray{Lookups::BISHOP_SOUTHWEST_RAYS[H8.value()], RayType::BISHOP, RayDir::SW};
  PinResult result;

  scan_pin_ray(H8, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned.count(), 1);
  EXPECT_TRUE(result.pinned.test(F6));
}

/**
 * @test Southeast diagonal pin detected correctly.
 * @brief Confirms scan_pin_ray() correctly detects pins along the
 *        southeast diagonal.
 */
TEST(ScanPinRayTest, SoutheastPinDetected) {
  Board board = Board::Empty();
  board.set_piece(A8, WHITE_KING);
  board.set_piece(C6, WHITE_PAWN);
  board.set_piece(F3, BLACK_BISHOP);

  PinRay ray{Lookups::BISHOP_SOUTHEAST_RAYS[A8.value()], RayType::BISHOP, RayDir::SE};
  PinResult result;

  scan_pin_ray(A8, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned.count(), 1);
  EXPECT_TRUE(result.pinned.test(C6));
}

/**
 * @test Northwest diagonal pin detected correctly.
 * @brief Confirms scan_pin_ray() correctly detects pins along the
 *        northwest diagonal.
 */
TEST(ScanPinRayTest, NorthwestPinDetected) {
  Board board = Board::Empty();
  board.set_piece(H1, WHITE_KING);
  board.set_piece(F3, WHITE_PAWN);
  board.set_piece(C6, BLACK_BISHOP);

  PinRay ray{Lookups::BISHOP_NORTHWEST_RAYS[H1.value()], RayType::BISHOP, RayDir::NW};
  PinResult result;

  scan_pin_ray(H1, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned.count(), 1);
  EXPECT_TRUE(result.pinned.test(F3));
}

/**
 * @test Pin detection works for black pieces.
 * @brief Confirms scan_pin_ray() correctly detects pins when the friendly
 *        side is black (enemy is white).
 */
TEST(ScanPinRayTest, BlackPiecesPinDetected) {
  Board board = Board::Empty();
  board.set_piece(E8, BLACK_KING);
  board.set_piece(E6, BLACK_PAWN);
  board.set_piece(E3, WHITE_ROOK);

  PinRay ray{Lookups::ROOK_SOUTH_RAYS[E8.value()], RayType::ROOK, RayDir::S};
  PinResult result;

  scan_pin_ray(E8, ray, board, Color::BLACK, result);

  EXPECT_EQ(result.pinned.count(), 1);
  EXPECT_TRUE(result.pinned.test(E6));
}

/**
 * @test Piece at board edge can be pinned.
 * @brief Confirms scan_pin_ray() correctly detects pins when the pinned
 *        piece is near the edge of the board.
 */
TEST(ScanPinRayTest, EdgePieceCanBePinned) {
  Board board = Board::Empty();
  board.set_piece(A1, WHITE_KING);
  board.set_piece(A7, WHITE_ROOK);
  board.set_piece(A8, BLACK_ROOK);

  PinRay ray{Lookups::ROOK_NORTH_RAYS[A1.value()], RayType::ROOK, RayDir::N};
  PinResult result;

  scan_pin_ray(A1, ray, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned.count(), 1);
  EXPECT_TRUE(result.pinned.test(A7));
}

/**
 * @test Multiple scans accumulate in result.
 * @brief Confirms scan_pin_ray() correctly adds new pins to existing
 *        PinResult without clearing previous data.
 */
TEST(ScanPinRayTest, MultipleScansAccumulate) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E6, WHITE_PAWN);
  board.set_piece(E8, BLACK_ROOK);
  board.set_piece(G4, WHITE_KNIGHT);
  board.set_piece(H4, BLACK_ROOK);

  PinResult result;

  PinRay ray_north{Lookups::ROOK_NORTH_RAYS[E4.value()], RayType::ROOK, RayDir::N};
  scan_pin_ray(E4, ray_north, board, Color::WHITE, result);

  PinRay ray_east{Lookups::ROOK_EAST_RAYS[E4.value()], RayType::ROOK, RayDir::E};
  scan_pin_ray(E4, ray_east, board, Color::WHITE, result);

  EXPECT_EQ(result.pinned.count(), 2);
  EXPECT_TRUE(result.pinned.test(E6));
  EXPECT_TRUE(result.pinned.test(G4));
}
