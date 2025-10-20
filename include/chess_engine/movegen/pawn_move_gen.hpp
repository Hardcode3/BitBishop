#pragma once

#include <chess_engine/attacks/pawn.hpp>
#include <chess_engine/movegen/piece_move_gen.hpp>

class PawnMoveGenerator : public PieceMoveGenerator {
 public:
  std::vector<Move> generate_pseudo_legal_moves(const Board& board, Color side) const override;
  std::vector<Move> generate_legal_moves(const Board& board, Color side) const override;

 private:
  // NOT TESTED
  static void add_pawn_promotions(std::vector<Move>& moves, Square from, Square to, Color side, bool is_capture);

  // NOT TESTED
  static constexpr bool is_starting_pawn_rank(Square sq, Color c) {
    return c == Color::WHITE ? (sq.rank() == 1) : (sq.rank() == 6);
  }

  // NOT TESTED
  static constexpr bool is_promotion_rank(Square sq, Color c) {
    return c == Color::WHITE ? (sq.rank() == 7) : (sq.rank() == 0);
  }

  // NOT TESTED
  static std::array<Bitboard, 64> single_push(Color side) {
    switch (side) {
      case Color::WHITE:
        return Attacks::WHITE_PAWN_SINGLE_PUSH;
      case Color::BLACK:
        return Attacks::BLACK_PAWN_SINGLE_PUSH;
      default:
        throw std::runtime_error("Invalid color");
    }
  }

  // NOT TESTED
  static std::array<Bitboard, 64> double_push(Color side) {
    switch (side) {
      case Color::WHITE:
        return Attacks::WHITE_PAWN_DOUBLE_PUSH;
      case Color::BLACK:
        return Attacks::BLACK_PAWN_DOUBLE_PUSH;
      default:
        throw std::runtime_error("Invalid color");
    }
  }

  // NOT TESTED
  static std::array<Bitboard, 64> attacks(Color side) {
    switch (side) {
      case Color::WHITE:
        return Attacks::WHITE_PAWN_ATTACKS;
      case Color::BLACK:
        return Attacks::BLACK_PAWN_ATTACKS;
      default:
        throw std::runtime_error("Invalid color");
    }
  }

  static constexpr bool can_capture_en_passant(Square from, Square epsq, Color side) noexcept {
    int df = std::abs(int(from.file()) - int(epsq.file()));
    if (df != 1) return false;
    if (side == Color::WHITE && from.rank() == 4) return true;
    if (side == Color::BLACK && from.rank() == 3) return true;
    return false;
  }
};
