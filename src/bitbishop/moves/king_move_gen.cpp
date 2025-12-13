#include <bitbishop/lookups/king.hpp>
#include <bitbishop/moves/king_move_gen.hpp>
#include <format>
#include <optional>

void KingMoveGenerator::generate_pseudo_legal_moves(std::vector<Move>& moves, const Board& board, Color side) {
  Bitboard king = board.king(side);
  const uint8_t nb_kings = king.count();

  if (nb_kings != 1) {
    throw std::runtime_error(std::format(
        "Failed to generate king pseudo-legal moves for {} pieces, expected 1 king, got {}", side, nb_kings));
  }

  const std::optional<Square> opt_sq = king.pop_lsb();
  if (!opt_sq.has_value()) {
    throw std::runtime_error(std::format("Failed to extract king square for {} side", side));
  }

  const Square from = opt_sq.value();
  const Bitboard& king_moves = Lookups::KING_ATTACKS[from.value()];
  const Bitboard empty = board.unoccupied();
  const Bitboard enemy = board.enemy(side);

  // Silent moves (pushes)
  Bitboard pushes = king_moves & empty;
  for (Square to : pushes) {
    moves.emplace_back(from, to, std::nullopt, false, false, false);
  }

  // Captures
  Bitboard captures = king_moves & enemy;
  for (Square to : captures) {
    moves.emplace_back(from, to, std::nullopt, true, false, false);
  }

  // Castling moves (pseudo-legal only checks piece positions, not attacks)
  add_king_castling(moves, from, side, board);
}

void KingMoveGenerator::generate_legal_moves(std::vector<Move>& moves, const Board& board, Color side) {
  // Check casting moves and classic moves separately
  // Check if the king is under attack
  // Check if the square the king passes through are under attack
  // TODO
}

void KingMoveGenerator::add_king_castling(std::vector<Move>& moves, Square from, Color side, const Board& board) {
  // Kingside castling
  if (can_castle_kingside(board, side)) {
    Square to = (side == Color::WHITE) ? Squares::G1 : Squares::G8;
    moves.emplace_back(from, to, std::nullopt, false, false, true);
  }

  // Queenside castling
  if (can_castle_queenside(board, side)) {
    Square to = (side == Color::WHITE) ? Squares::C1 : Squares::C8;
    moves.emplace_back(from, to, std::nullopt, false, false, true);
  }
}

bool KingMoveGenerator::can_castle_kingside(const Board& board, Color side) {
  if (!board.has_kingside_castling_rights(side)) {
    return false;
  }

  Square king_sq = (side == Color::WHITE) ? Squares::E1 : Squares::E8;
  Square rook_sq = (side == Color::WHITE) ? Squares::H1 : Squares::H8;
  Square f_sq = (side == Color::WHITE) ? Squares::F1 : Squares::F8;
  Square g_sq = (side == Color::WHITE) ? Squares::G1 : Squares::G8;

  // Check if king is on the starting square
  if (!board.king(side).test(king_sq)) {
    return false;
  }

  // Check if rook is on the starting square
  if (!board.rooks(side).test(rook_sq)) {
    return false;
  }

  // Check if squares between king and rook are empty
  const Bitboard occupied = board.occupied();
  return !occupied.test(f_sq) && !occupied.test(g_sq);
}

bool KingMoveGenerator::can_castle_queenside(const Board& board, Color side) {
  if (!board.has_queenside_castling_rights(side)) {
    return false;
  }

  Square king_sq = (side == Color::WHITE) ? Squares::E1 : Squares::E8;
  Square rook_sq = (side == Color::WHITE) ? Squares::A1 : Squares::A8;
  Square b_sq = (side == Color::WHITE) ? Squares::B1 : Squares::B8;
  Square c_sq = (side == Color::WHITE) ? Squares::C1 : Squares::C8;
  Square d_sq = (side == Color::WHITE) ? Squares::D1 : Squares::D8;

  // Check that the king is on the starting square
  if (!board.king(side).test(king_sq)) {
    return false;
  }

  // Check if the rook is on the starting square
  if (!board.rooks(side).test(rook_sq)) {
    return false;
  }

  // Check if the squares between king and rook are empty
  const Bitboard occupied = board.occupied();
  return !occupied.test(b_sq) && !occupied.test(c_sq) && !occupied.test(d_sq);
}
