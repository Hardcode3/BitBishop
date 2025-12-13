#include <bitbishop/lookups/bishop.hpp>
#include <bitbishop/moves/bishop_move_gen.hpp>
#include <optional>

void BishopMoveGenerator::generate_pseudo_legal_moves(std::vector<Move>& moves, const Board& board, Color side) {
  Bitboard bishops = board.bishops(side);
  Bitboard empty = board.unoccupied();
  Bitboard enemy = board.enemy(side);
  Bitboard occupied = board.occupied();

  // warning: this loop is destructive on Bitboard bishops
  while (auto from_opt = bishops.pop_lsb()) {
    Square from = from_opt.value();

    Bitboard bishop_attacks;
    bishop_attacks |= north_east_ray(from, occupied);
    bishop_attacks |= north_west_ray(from, occupied);
    bishop_attacks |= south_east_ray(from, occupied);
    bishop_attacks |= south_west_ray(from, occupied);

    // Bishop sliding moves
    Bitboard bishop_moves = bishop_attacks & empty;
    for (auto to : bishop_moves) {
      moves.emplace_back(from, to, std::nullopt, false, false, false);
    }

    // Bishop sliding captures
    Bitboard bishop_captures = bishop_attacks & enemy;
    for (auto to : bishop_captures) {
      moves.emplace_back(from, to, std::nullopt, true, false, false);
    }
  }
}

Bitboard BishopMoveGenerator::north_east_ray(Square from, const Bitboard& occupied) {
  Bitboard ne_ray = Lookups::BISHOP_NORTHEAST_ATTACKS[from.value()];
  Bitboard blockers = ne_ray & occupied;

  // Closest blocker (NE goes up, so use lsb)
  std::optional<Square> first_blocker = blockers.lsb();

  if (first_blocker.has_value()) {
    Square blocker_square = first_blocker.value();
    const Bitboard& beyond_blocker = Lookups::BISHOP_NORTHEAST_ATTACKS[blocker_square.value()];
    ne_ray &= ~beyond_blocker;
  }

  return ne_ray;
}

Bitboard BishopMoveGenerator::north_west_ray(Square from, const Bitboard& occupied) {
  Bitboard nw_ray = Lookups::BISHOP_NORTHWEST_ATTACKS[from.value()];
  Bitboard blockers = nw_ray & occupied;

  // Closest blocker (NW goes up, so use lsb)
  std::optional<Square> first_blocker = blockers.lsb();

  if (first_blocker.has_value()) {
    Square blocker_square = first_blocker.value();
    const Bitboard& beyond_blocker = Lookups::BISHOP_NORTHWEST_ATTACKS[blocker_square.value()];
    nw_ray &= ~beyond_blocker;
  }

  return nw_ray;
}

Bitboard BishopMoveGenerator::south_east_ray(Square from, const Bitboard& occupied) {
  Bitboard se_ray = Lookups::BISHOP_SOUTHEAST_ATTACKS[from.value()];
  Bitboard blockers = se_ray & occupied;

  // Closest blocker (SE goes down, so use msb)
  std::optional<Square> first_blocker = blockers.msb();

  if (first_blocker.has_value()) {
    Square blocker_square = first_blocker.value();
    const Bitboard& beyond_blocker = Lookups::BISHOP_SOUTHEAST_ATTACKS[blocker_square.value()];
    se_ray &= ~beyond_blocker;
  }

  return se_ray;
}

Bitboard BishopMoveGenerator::south_west_ray(Square from, const Bitboard& occupied) {
  Bitboard sw_ray = Lookups::BISHOP_SOUTHWEST_ATTACKS[from.value()];
  Bitboard blockers = sw_ray & occupied;

  // Closest blocker (SW goes down, so use msb)
  std::optional<Square> first_blocker = blockers.msb();

  if (first_blocker.has_value()) {
    Square blocker_square = first_blocker.value();
    const Bitboard& beyond_blocker = Lookups::BISHOP_SOUTHWEST_ATTACKS[blocker_square.value()];
    sw_ray &= ~beyond_blocker;
  }

  return sw_ray;
}
