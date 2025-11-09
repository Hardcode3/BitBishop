#include <chess_engine/attacks/king.hpp>
#include <chess_engine/movegen/king_move_gen.hpp>
#include <format>
#include <optional>

std::vector<Move> KingMoveGenerator::generate_pseudo_legal_moves(const Board& b, Color side) const {
  std::vector<Move> moves;

  Bitboard king = b.king(side);
  Bitboard empty = b.unoccupied();
  Bitboard enemy = b.enemy(side);

  const std::optional<Square> opt_sq = king.pop_lsb();
  const uint8_t nb_kings = king.count();

  if (nb_kings != 1 || !opt_sq.has_value()) {
    // clang-format off
    const std::string msg = std::format(
      "Failed to generate king pseudo-legal moves for {} pieces, expected 1 king, got {}",
      side, nb_kings
    );
    // clang-format on
    throw std::runtime_error(msg);
  }

  const Square from = opt_sq.value();

  Bitboard king_moves = Attacks::KING_ATTACKS[from.value()];

  Bitboard pushes = Bitboard(king_moves);
  pushes &= empty;
  for (Square to : king_moves) {
    moves.emplace_back(from, to, std::nullopt, false, false, false);
  }

  Bitboard captures = Bitboard(king_moves);
  captures &= enemy;
  for (Square to : king_moves) {
    moves.emplace_back(from, to, std::nullopt, false, false, false);
  }

  return moves;
}

std::vector<Move> KingMoveGenerator::generate_legal_moves(const Board& board, Color side) const {
  std::vector<Move> moves;
  // TODO
  return moves;
}
