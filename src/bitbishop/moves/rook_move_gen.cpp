#include <bitbishop/lookups/rook.hpp>
#include <bitbishop/moves/king_move_gen.hpp>
#include <bitbishop/moves/rook_move_gen.hpp>

void RookMoveGenerator::generate_pseudo_legal_moves(std::vector<Move>& moves, const Board& board, Color side) {
  Bitboard rooks = board.rooks(side);
  Bitboard empty = board.unoccupied();
  Bitboard enemy = board.enemy(side);
  Bitboard occupied = board.occupied();

  // warning: this loop is destructive on Bitboard rooks
  while (auto from_opt = rooks.pop_lsb()) {
    Square from = from_opt.value();

    Bitboard rook_attacks;
    rook_attacks |= north_ray(from, occupied);
    rook_attacks |= south_ray(from, occupied);
    rook_attacks |= east_ray(from, occupied);
    rook_attacks |= west_ray(from, occupied);

    // Silent moves
    Bitboard rook_moves = rook_attacks & empty;
    for (auto to : rook_moves) {
      moves.emplace_back(from, to, std::nullopt, false, false, false);
    }

    // Captures
    Bitboard rook_captures = rook_attacks & enemy;
    for (auto to : rook_captures) {
      moves.emplace_back(from, to, std::nullopt, true, false, false);
    }

    // Castling moves (pseudo-legal only checks piece positions, not attacks)
    add_rook_castling(moves, from, side, board);
  }
}

void RookMoveGenerator::add_rook_castling(std::vector<Move>& moves, Square from, Color side, const Board& board) {
  // TODO: These Castling utilities must be moved out of the KingMoveGenerator namespace
  // as it is common code with RookMoveGenerator
  Square kingside_rook = (side == Color::WHITE) ? Squares::H1 : Squares::H8;
  Square queenside_rook = (side == Color::WHITE) ? Squares::A1 : Squares::A8;

  // Kingside castling
  const bool can_castle_kingside = KingMoveGenerator::can_castle_kingside(board, side) && (from == kingside_rook);
  if (can_castle_kingside) {
    Square to = (side == Color::WHITE) ? Squares::F1 : Squares::F8;
    moves.emplace_back(from, to, std::nullopt, false, false, true);
  }

  // Queenside castling
  const bool can_castle_queenside = KingMoveGenerator::can_castle_queenside(board, side) && (from == queenside_rook);
  if (can_castle_queenside) {
    Square to = (side == Color::WHITE) ? Squares::D1 : Squares::D8;
    moves.emplace_back(from, to, std::nullopt, false, false, true);
  }
}

Bitboard RookMoveGenerator::north_ray(Square from, const Bitboard& occupied) {
  Bitboard n_ray = Lookups::ROOK_NORTH_ATTACKS[from.value()];
  Bitboard blockers = n_ray & occupied;

  // Closest blocker (N goes up towards msb, so use lsb)
  std::optional<Square> first_blocker = blockers.lsb();

  if (first_blocker.has_value()) {
    Square blocker_square = first_blocker.value();
    const Bitboard& beyond_blocker = Lookups::ROOK_NORTH_ATTACKS[blocker_square.value()];
    n_ray &= ~beyond_blocker;
  }

  return n_ray;
}

Bitboard RookMoveGenerator::south_ray(Square from, const Bitboard& occupied) {
  Bitboard s_ray = Lookups::ROOK_SOUTH_ATTACKS[from.value()];
  Bitboard blockers = s_ray & occupied;

  // Closest blocker (S goes down towards lsb, so use msb)
  std::optional<Square> first_blocker = blockers.msb();

  if (first_blocker.has_value()) {
    Square blocker_square = first_blocker.value();
    const Bitboard& beyond_blocker = Lookups::ROOK_SOUTH_ATTACKS[blocker_square.value()];
    s_ray &= ~beyond_blocker;
  }

  return s_ray;
}

Bitboard RookMoveGenerator::east_ray(Square from, const Bitboard& occupied) {
  Bitboard e_ray = Lookups::ROOK_EAST_ATTACKS[from.value()];
  Bitboard blockers = e_ray & occupied;

  // Closest blocker (E goes twoards msb, so use lsb)
  std::optional<Square> first_blocker = blockers.lsb();

  if (first_blocker.has_value()) {
    Square blocker_square = first_blocker.value();
    const Bitboard& beyond_blocker = Lookups::ROOK_EAST_ATTACKS[blocker_square.value()];
    e_ray &= ~beyond_blocker;
  }

  return e_ray;
}

Bitboard RookMoveGenerator::west_ray(Square from, const Bitboard& occupied) {
  Bitboard w_ray = Lookups::ROOK_WEST_ATTACKS[from.value()];
  Bitboard blockers = w_ray & occupied;

  // Closest blocker (W goes towards lsb, so use msb)
  std::optional<Square> first_blocker = blockers.msb();

  if (first_blocker.has_value()) {
    Square blocker_square = first_blocker.value();
    const Bitboard& beyond_blocker = Lookups::ROOK_WEST_ATTACKS[blocker_square.value()];
    w_ray &= ~beyond_blocker;
  }

  return w_ray;
}
