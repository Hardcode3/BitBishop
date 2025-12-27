
#include <bitbishop/movegen/pins.hpp>

void scan_pin_ray(Square king_sq, const PinRay& ray_info, const Board& board, Color us, PinResult& result) {
  const Bitboard occupied = board.occupied();
  const Bitboard own = board.friendly(us);
  const Color them = ColorUtil::opposite(us);

  Bitboard blockers = ray_info.ray & occupied;
  if (!blockers) {
    return;
  }

  Square first_blocker_sq = ray_info.first_blocker(blockers);
  Bitboard first_blocker_bb(first_blocker_sq);
  if (!(first_blocker_bb & own)) {
    return;
  }

  Bitboard beyond = ray_info.ray & ~(Lookups::BETWEEN[king_sq.value()][first_blocker_sq.value()] | first_blocker_bb);
  Bitboard second_blockers_bb = beyond & occupied;
  if (!second_blockers_bb) {
    return;
  }

  Square second_blocker_sq = ray_info.first_blocker(second_blockers_bb);
  Bitboard second_blocker_bb(second_blocker_sq);
  if (!ray_info.matches_slider(second_blocker_bb, board, them)) {
    return;
  }

  result.pinned |= first_blocker_bb;
  result.pin_ray[first_blocker_sq.value()] =
      Lookups::BETWEEN[king_sq.value()][second_blocker_sq.value()] | second_blocker_bb;
}

PinResult compute_pins(Square king_sq, const Board& board, Color us) {
  PinResult result;

  const std::array<PinRay, 8> ray_infos = {{
      PinRay{.ray = Lookups::ROOK_NORTH_RAYS[king_sq.value()], .type = RayType::ROOK, .dir = RayDir::N},
      PinRay{.ray = Lookups::ROOK_SOUTH_RAYS[king_sq.value()], .type = RayType::ROOK, .dir = RayDir::S},
      PinRay{.ray = Lookups::ROOK_EAST_RAYS[king_sq.value()], .type = RayType::ROOK, .dir = RayDir::E},
      PinRay{.ray = Lookups::ROOK_WEST_RAYS[king_sq.value()], .type = RayType::ROOK, .dir = RayDir::W},
      PinRay{.ray = Lookups::BISHOP_NORTHEAST_RAYS[king_sq.value()], .type = RayType::BISHOP, .dir = RayDir::NE},
      PinRay{.ray = Lookups::BISHOP_NORTHWEST_RAYS[king_sq.value()], .type = RayType::BISHOP, .dir = RayDir::NW},
      PinRay{.ray = Lookups::BISHOP_SOUTHEAST_RAYS[king_sq.value()], .type = RayType::BISHOP, .dir = RayDir::SE},
      PinRay{.ray = Lookups::BISHOP_SOUTHWEST_RAYS[king_sq.value()], .type = RayType::BISHOP, .dir = RayDir::SW},
  }};

  for (const auto& ray_info : ray_infos) {
    scan_pin_ray(king_sq, ray_info, board, us, result);
  }

  return result;
}
