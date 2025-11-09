#include <chess_engine/attacks/king.hpp>
#include <chess_engine/movegen/king_move_gen.hpp>
#include <format>
#include <optional>

std::vector<Move> KingMoveGenerator::generate_pseudo_legal_moves(const Board& b, Color side) const {
  std::vector<Move> moves;

  Bitboard king = b.king(side);
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
  Bitboard king_moves = Attacks::KING_ATTACKS[from.value()];

  Bitboard empty = b.unoccupied();
  Bitboard enemy = b.enemy(side);

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

  return moves;
}

std::vector<Move> KingMoveGenerator::generate_legal_moves(const Board& board, Color side) const {
  std::vector<Move> moves;
  // TODO
  return moves;
}
