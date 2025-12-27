#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/movegen/pins.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test matches_slider accepts enemy rook on rook ray.
 * @brief Confirms PinRay::matches_slider() returns true for enemy rook
 *        on an orthogonal ray.
 */
TEST(PinRayTest, MatchesSliderRookOnRookRay) {
  Board board = Board::Empty();
  const Piece piece = BLACK_ROOK;
  board.set_piece(E8, piece);

  PinRay ray{Lookups::ROOK_NORTH_RAYS[E1.value()], RayType::ROOK, RayDir::N};
  Bitboard piece_bb = board.rooks(piece.color());

  EXPECT_TRUE(ray.matches_slider(piece_bb, board, piece.color()));
}

/**
 * @test matches_slider accepts enemy queen on rook ray.
 * @brief Confirms PinRay::matches_slider() returns true for enemy queen
 *        on an orthogonal ray.
 */
TEST(PinRayTest, MatchesSliderQueenOnRookRay) {
  Board board = Board::Empty();
  const Piece piece = BLACK_QUEEN;
  board.set_piece(E8, piece);

  PinRay ray{Lookups::ROOK_NORTH_RAYS[E1.value()], RayType::ROOK, RayDir::N};
  Bitboard piece_bb = board.queens(piece.color());

  EXPECT_TRUE(ray.matches_slider(piece_bb, board, piece.color()));
}

/**
 * @test matches_slider rejects enemy bishop on rook ray.
 * @brief Confirms PinRay::matches_slider() returns false for enemy bishop
 *        on an orthogonal ray (bishops cannot create orthogonal pins).
 */
TEST(PinRayTest, MatchesSliderBishopOnRookRay) {
  Board board = Board::Empty();
  const Piece piece = BLACK_BISHOP;
  board.set_piece(E8, piece);

  PinRay ray{Lookups::ROOK_NORTH_RAYS[E1.value()], RayType::ROOK, RayDir::N};
  Bitboard piece_bb = board.bishops(piece.color());

  EXPECT_FALSE(ray.matches_slider(piece_bb, board, piece.color()));
}

/**
 * @test matches_slider accepts enemy bishop on bishop ray.
 * @brief Confirms PinRay::matches_slider() returns true for enemy bishop
 *        on a diagonal ray.
 */
TEST(PinRayTest, MatchesSliderBishopOnBishopRay) {
  Board board = Board::Empty();
  const Piece piece = BLACK_BISHOP;
  board.set_piece(H8, piece);

  PinRay ray{Lookups::BISHOP_NORTHEAST_RAYS[A1.value()], RayType::BISHOP, RayDir::NE};
  Bitboard piece_bb = board.bishops(piece.color());

  EXPECT_TRUE(ray.matches_slider(piece_bb, board, piece.color()));
}

/**
 * @test matches_slider accepts enemy queen on bishop ray.
 * @brief Confirms PinRay::matches_slider() returns true for enemy queen
 *        on a diagonal ray.
 */
TEST(PinRayTest, MatchesSliderQueenOnBishopRay) {
  Board board = Board::Empty();
  const Piece piece = BLACK_QUEEN;
  board.set_piece(H8, piece);

  PinRay ray{Lookups::BISHOP_NORTHEAST_RAYS[A1.value()], RayType::BISHOP, RayDir::NE};
  Bitboard piece_bb = board.queens(piece.color());

  EXPECT_TRUE(ray.matches_slider(piece_bb, board, piece.color()));
}

/**
 * @test matches_slider rejects enemy rook on bishop ray.
 * @brief Confirms PinRay::matches_slider() returns false for enemy rook
 *        on a diagonal ray (rooks cannot create diagonal pins).
 */
TEST(PinRayTest, MatchesSliderRookOnBishopRay) {
  Board board = Board::Empty();
  const Piece piece = BLACK_ROOK;
  board.set_piece(H8, piece);

  PinRay ray{Lookups::BISHOP_NORTHEAST_RAYS[A1.value()], RayType::BISHOP, RayDir::NE};
  Bitboard piece_bb = board.rooks(piece.color());

  EXPECT_FALSE(ray.matches_slider(piece_bb, board, piece.color()));
}

/**
 * @test matches_slider rejects enemy knight on any ray.
 * @brief Confirms PinRay::matches_slider() returns false for enemy knight
 *        (knights cannot create pins).
 */
TEST(PinRayTest, MatchesSliderKnightOnRay) {
  Board board = Board::Empty();
  const Piece piece = BLACK_KNIGHT;
  board.set_piece(E8, piece);

  PinRay ray{Lookups::ROOK_NORTH_RAYS[E1.value()], RayType::ROOK, RayDir::N};
  Bitboard piece_bb = board.knights(piece.color());

  EXPECT_FALSE(ray.matches_slider(piece_bb, board, piece.color()));
}

/**
 * @test matches_slider rejects enemy pawn on any ray.
 * @brief Confirms PinRay::matches_slider() returns false for enemy pawn
 *        (pawns cannot create pins).
 */
TEST(PinRayTest, MatchesSliderPawnOnRay) {
  Board board = Board::Empty();
  const Piece piece = BLACK_PAWN;
  board.set_piece(E8, piece);

  PinRay ray{Lookups::ROOK_NORTH_RAYS[E1.value()], RayType::ROOK, RayDir::N};
  Bitboard piece_bb = board.pawns(piece.color());

  EXPECT_FALSE(ray.matches_slider(piece_bb, board, piece.color()));
}

/**
 * @test matches_slider works with white pieces as enemy.
 * @brief Confirms PinRay::matches_slider() correctly identifies white
 *        enemy sliders when passed Color::WHITE.
 */
TEST(PinRayTest, MatchesSliderWhiteRookAsEnemy) {
  Board board = Board::Empty();
  const Piece piece = WHITE_ROOK;
  board.set_piece(E1, piece);

  PinRay ray{Lookups::ROOK_SOUTH_RAYS[E8.value()], RayType::ROOK, RayDir::S};
  Bitboard piece_bb = board.rooks(piece.color());

  EXPECT_TRUE(ray.matches_slider(piece_bb, board, piece.color()));
}
