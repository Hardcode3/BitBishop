#include <bitbishop/attacks/rook_attacks.hpp>
#include <bitbishop/lookups/rook_rays.hpp>

Bitboard rook_north_attacks(Square from, const Bitboard& occupied) {
  Bitboard n_ray = Lookups::ROOK_NORTH_RAYS[from.value()];
  Bitboard blockers = n_ray & occupied;

  // Closest blocker (N goes up towards msb, so use lsb)
  std::optional<Square> first_blocker = blockers.lsb();

  if (first_blocker.has_value()) {
    Square blocker_square = first_blocker.value();
    const Bitboard& beyond_blocker = Lookups::ROOK_NORTH_RAYS[blocker_square.value()];
    n_ray &= ~beyond_blocker;
  }

  return n_ray;
}

Bitboard rook_south_attacks(Square from, const Bitboard& occupied) {
  Bitboard s_ray = Lookups::ROOK_SOUTH_RAYS[from.value()];
  Bitboard blockers = s_ray & occupied;

  // Closest blocker (S goes down towards lsb, so use msb)
  std::optional<Square> first_blocker = blockers.msb();

  if (first_blocker.has_value()) {
    Square blocker_square = first_blocker.value();
    const Bitboard& beyond_blocker = Lookups::ROOK_SOUTH_RAYS[blocker_square.value()];
    s_ray &= ~beyond_blocker;
  }

  return s_ray;
}

Bitboard rook_east_attacks(Square from, const Bitboard& occupied) {
  Bitboard e_ray = Lookups::ROOK_EAST_RAYS[from.value()];
  Bitboard blockers = e_ray & occupied;

  // Closest blocker (E goes twoards msb, so use lsb)
  std::optional<Square> first_blocker = blockers.lsb();

  if (first_blocker.has_value()) {
    Square blocker_square = first_blocker.value();
    const Bitboard& beyond_blocker = Lookups::ROOK_EAST_RAYS[blocker_square.value()];
    e_ray &= ~beyond_blocker;
  }

  return e_ray;
}

Bitboard rook_west_attacks(Square from, const Bitboard& occupied) {
  Bitboard w_ray = Lookups::ROOK_WEST_RAYS[from.value()];
  Bitboard blockers = w_ray & occupied;

  // Closest blocker (W goes towards lsb, so use msb)
  std::optional<Square> first_blocker = blockers.msb();

  if (first_blocker.has_value()) {
    Square blocker_square = first_blocker.value();
    const Bitboard& beyond_blocker = Lookups::ROOK_WEST_RAYS[blocker_square.value()];
    w_ray &= ~beyond_blocker;
  }

  return w_ray;
}

Bitboard rook_attacks(Square from, const Bitboard& occupied) {
  Bitboard attacks;
  attacks |= rook_north_attacks(from, occupied);
  attacks |= rook_south_attacks(from, occupied);
  attacks |= rook_east_attacks(from, occupied);
  attacks |= rook_west_attacks(from, occupied);
  return attacks;
}
