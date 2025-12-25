#include <bitbishop/attacks/bishop_attacks.hpp>
#include <bitbishop/attacks/queen_attacks.hpp>
#include <bitbishop/attacks/rook_attacks.hpp>

Bitboard queen_attacks(Square from, const Bitboard& occupied) {
  Bitboard attacks;
  attacks |= rook_attacks(from, occupied);
  attacks |= bishop_attacks(from, occupied);
  return attacks;
}
