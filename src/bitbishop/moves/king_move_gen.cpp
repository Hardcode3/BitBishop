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
  if (board.can_castle_kingside(side)) {
    Square to = (side == Color::WHITE) ? Squares::G1 : Squares::G8;
    moves.emplace_back(from, to, std::nullopt, false, false, true);
  }

  // Queenside castling
  if (board.can_castle_queenside(side)) {
    Square to = (side == Color::WHITE) ? Squares::C1 : Squares::C8;
    moves.emplace_back(from, to, std::nullopt, false, false, true);
  }
}
