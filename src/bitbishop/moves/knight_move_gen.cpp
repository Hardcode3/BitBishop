#include <bitbishop/lookups/knight_attacks.hpp>
#include <bitbishop/moves/knight_move_gen.hpp>

void KnightMoveGenerator::generate_pseudo_legal_moves(std::vector<Move>& moves, const Board& board, Color side) {
  Bitboard knights = board.knights(side);
  Bitboard empty = board.unoccupied();
  Bitboard enemy = board.enemy(side);

  // warning: this loop is destructive on Bitboard knights
  while (auto from_opt = knights.pop_lsb()) {
    Square from = from_opt.value();
    const Bitboard& knight_lookup = Lookups::KNIGHT_ATTACKS[from.value()];

    // Classic knight moves
    Bitboard knight_moves = knight_lookup & empty;
    for (Square to : knight_moves) {
      moves.emplace_back(from, to, std::nullopt, false, false, false);
    }

    // Knight captures
    Bitboard knight_captures = knight_lookup & enemy;
    for (Square to : knight_captures) {
      moves.emplace_back(from, to, std::nullopt, true, false, false);
    }
  }
}
