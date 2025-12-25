#include <bitbishop/attacks/queen_attacks.hpp>
#include <bitbishop/moves/queen_move_gen.hpp>
#include <optional>

void QueenMoveGenerator::generate_pseudo_legal_moves(std::vector<Move>& moves, const Board& board, Color side) {
  // Multiple queens may be present on the board after pawn promotion
  Bitboard queens = board.queens(side);
  Bitboard empty = board.unoccupied();
  Bitboard enemy = board.enemy(side);
  Bitboard occupied = board.occupied();

  // warning: this loop is destructive on Bitboard queens
  while (auto from_opt = queens.pop_lsb()) {
    Square from = from_opt.value();

    Bitboard attacks = queen_attacks(from, occupied);

    // Queen sliding moves
    Bitboard queen_moves = attacks & empty;
    for (auto to : queen_moves) {
      moves.emplace_back(from, to, std::nullopt, false, false, false);
    }

    // Queen captures
    Bitboard queen_captures = attacks & enemy;
    for (auto to : queen_captures) {
      moves.emplace_back(from, to, std::nullopt, true, false, false);
    }
  }
}
