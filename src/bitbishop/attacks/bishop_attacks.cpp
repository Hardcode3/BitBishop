#include <bitbishop/attacks/bishop_attacks.hpp>
#include <bitbishop/lookups/bishop_rays.hpp>

Bitboard bishop_north_east_attacks(Square from, const Bitboard& occupied) {
  Bitboard ne_ray = Lookups::BISHOP_NORTHEAST_RAYS[from.value()];
  Bitboard blockers = ne_ray & occupied;

  // Closest blocker (NE goes up, so use lsb)
  std::optional<Square> first_blocker = blockers.lsb();

  if (first_blocker.has_value()) {
    Square blocker_square = first_blocker.value();
    const Bitboard& beyond_blocker = Lookups::BISHOP_NORTHEAST_RAYS[blocker_square.value()];
    ne_ray &= ~beyond_blocker;
  }

  return ne_ray;
}

Bitboard bishop_north_west_attacks(Square from, const Bitboard& occupied) {
  Bitboard nw_ray = Lookups::BISHOP_NORTHWEST_RAYS[from.value()];
  Bitboard blockers = nw_ray & occupied;

  // Closest blocker (NW goes up, so use lsb)
  std::optional<Square> first_blocker = blockers.lsb();

  if (first_blocker.has_value()) {
    Square blocker_square = first_blocker.value();
    const Bitboard& beyond_blocker = Lookups::BISHOP_NORTHWEST_RAYS[blocker_square.value()];
    nw_ray &= ~beyond_blocker;
  }

  return nw_ray;
}

Bitboard bishop_south_east_attacks(Square from, const Bitboard& occupied) {
  Bitboard se_ray = Lookups::BISHOP_SOUTHEAST_RAYS[from.value()];
  Bitboard blockers = se_ray & occupied;

  // Closest blocker (SE goes down, so use msb)
  std::optional<Square> first_blocker = blockers.msb();

  if (first_blocker.has_value()) {
    Square blocker_square = first_blocker.value();
    const Bitboard& beyond_blocker = Lookups::BISHOP_SOUTHEAST_RAYS[blocker_square.value()];
    se_ray &= ~beyond_blocker;
  }

  return se_ray;
}

Bitboard bishop_south_west_attacks(Square from, const Bitboard& occupied) {
  Bitboard sw_ray = Lookups::BISHOP_SOUTHWEST_RAYS[from.value()];
  Bitboard blockers = sw_ray & occupied;

  // Closest blocker (SW goes down, so use msb)
  std::optional<Square> first_blocker = blockers.msb();

  if (first_blocker.has_value()) {
    Square blocker_square = first_blocker.value();
    const Bitboard& beyond_blocker = Lookups::BISHOP_SOUTHWEST_RAYS[blocker_square.value()];
    sw_ray &= ~beyond_blocker;
  }

  return sw_ray;
}

Bitboard bishop_attacks(Square from, const Bitboard& occupied) {
  Bitboard attacks;
  attacks |= bishop_south_east_attacks(from, occupied);
  attacks |= bishop_south_west_attacks(from, occupied);
  attacks |= bishop_north_east_attacks(from, occupied);
  attacks |= bishop_north_west_attacks(from, occupied);
  return attacks;
}
