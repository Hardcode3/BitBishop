#pragma once

#include <chess_engine/movegen/piece_move_gen.hpp>

class PawnMoveGenerator : public PieceMoveGenerator {
 public:
  std::vector<Move> generate_pseudo_legal_moves(const Board& board, Color side) const override;
  std::vector<Move> generate_legal_moves(const Board& board, Color side) const override;

 private:
  void add_pawn_promotions(std::vector<Move>& moves, Square from, Square to, bool is_capture) const;

  constexpr bool is_starting_pawn_rank(Square sq, Color c) {
    return c == Color::WHITE ? (sq.rank() == 1) : (sq.rank() == 6);
  }

  constexpr bool is_promotion_rank(Square sq, Color c) {
    return c == Color::WHITE ? (sq.rank() == 7) : (sq.rank() == 0);
  }
};
