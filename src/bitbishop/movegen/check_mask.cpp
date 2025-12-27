#include <bitbishop/lookups/between_squares.hpp>
#include <bitbishop/movegen/check_mask.hpp>

Bitboard compute_check_mask(Square king_sq, const Bitboard& checkers, const Board& board) {
  if (!checkers) {
    return Bitboard::Ones();
  }

  if (checkers.count() > 1) {
    return Bitboard::Zeros();
  }

  Square checker_sq = checkers.lsb().value();

  const Bitboard knights_and_pawns =
      board.knights(Color::WHITE) | board.knights(Color::BLACK) | board.pawns(Color::WHITE) | board.pawns(Color::BLACK);
  const bool is_knight_or_pawn = knights_and_pawns.test(checker_sq);
  if (is_knight_or_pawn) {
    return checkers;
  }

  return Lookups::ray_between(checker_sq, king_sq) | checkers;
}
