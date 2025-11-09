#include <chess_engine/movegen/king_move_gen.hpp>
#include <format>

std::vector<Move> KingMoveGenerator::generate_pseudo_legal_moves(const Board& b, Color side) const {
  std::vector<Move> moves;

  Bitboard king = b.king(side);
  Bitboard empty = b.unoccupied();
  Bitboard enemy = b.enemy(side);

  const uint8_t nb_kings = king.count();
  if (nb_kings != 1) {
    // clang-format off
    const std::string msg = std::format(
      "Failed to generate king pseudo-legal moves for {} pieces, expected 1 king, got {}",
      side, nb_kings
    );
    // clang-format on
    throw std::runtime_error(msg);
  }

  // warning: this loop is destructive on Bitboard pawns
  while (auto from_opt = king.pop_lsb()) {
  }

  return moves;
}

std::vector<Move> KingMoveGenerator::generate_legal_moves(const Board& board, Color side) const {
  std::vector<Move> moves;
  // TODO
  return moves;
}
