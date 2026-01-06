#include <bitbishop/attacks/bishop_attacks.hpp>
#include <bitbishop/attacks/queen_attacks.hpp>
#include <bitbishop/attacks/rook_attacks.hpp>
#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/lookups/attackers.hpp>
#include <bitbishop/lookups/king_attacks.hpp>
#include <bitbishop/lookups/knight_attacks.hpp>
#include <bitbishop/lookups/pawn_attacks.hpp>
#include <bitbishop/square.hpp>

Bitboard compute_checkers(const Board& board, Square king_sq, Color attacker) {
  using namespace Lookups;

  Bitboard checkers;
  Bitboard occupied = board.occupied();

  checkers |= KNIGHT_ATTACKERS[king_sq.value()] & board.knights(attacker);

  if (attacker == Color::WHITE) {
    checkers |= WHITE_PAWN_ATTACKERS[king_sq.value()] & board.pawns(attacker);
  } else {
    checkers |= BLACK_PAWN_ATTACKERS[king_sq.value()] & board.pawns(attacker);
  }

  checkers |= KING_ATTACKERS[king_sq.value()] & board.king(attacker);

  Bitboard diag_sliders = BISHOP_ATTACKER_RAYS[king_sq.value()] & (board.bishops(attacker) | board.queens(attacker));
  while (diag_sliders) {
    Square sq = diag_sliders.pop_lsb().value();
    if (bishop_attacks(sq, occupied).test(king_sq)) {
      checkers.set(sq);
    }
  }

  Bitboard ortho_sliders = ROOK_ATTACKER_RAYS[king_sq.value()] & (board.rooks(attacker) | board.queens(attacker));
  while (ortho_sliders) {
    Square sq = ortho_sliders.pop_lsb().value();
    if (rook_attacks(sq, occupied).test(king_sq)) {
      checkers.set(sq);
    }
  }

  return checkers;
}
