#include <chess_engine/color.hpp>
#include <chess_engine/movegen/pawn_move_gen.hpp>

std::vector<Move> PawnMoveGenerator::generate_pseudo_legal_moves(const Board& b, Color side) const {
  std::vector<Move> moves;

  // TODO

  return moves;
}

void PawnMoveGenerator::add_pawn_promotions(std::vector<Move>& moves, Square from, Square to, bool capture) const {
  // TODO
}