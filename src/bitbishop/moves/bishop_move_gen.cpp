#include <bitbishop/attacks/bishop_attacks.hpp>
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

    Bitboard attacks = bishop_attacks(from, occupied);

    // Bishop sliding moves
    Bitboard bishop_moves = attacks & empty;
    for (auto to : bishop_moves) {
      moves.emplace_back(from, to, std::nullopt, false, false, false);
    }

    // Bishop sliding captures
    Bitboard bishop_captures = attacks & enemy;
    for (auto to : bishop_captures) {
      moves.emplace_back(from, to, std::nullopt, true, false, false);
    }
  }
}
