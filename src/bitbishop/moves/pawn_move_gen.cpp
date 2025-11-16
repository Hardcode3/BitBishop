#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/lookups/pawn.hpp>
#include <bitbishop/moves/pawn_move_gen.hpp>
#include <optional>

void PawnMoveGenerator::generate_pseudo_legal_moves(std::vector<Move>& moves, const Board& board, Color side) {
  // NOTE: Possible optimization here: there are faster alternatives to an iterator over the Bitboard object

  Bitboard pawns = board.pawns(side);
  Bitboard empty = board.unoccupied();
  Bitboard enemy = board.enemy(side);

  // warning: this loop is destructive on Bitboard pawns
  while (auto from_opt = pawns.pop_lsb()) {
    Square from = from_opt.value();

    // Single pushes
    Bitboard single_pushes = single_push(side)[from.value()];
    single_pushes &= empty;
    for (Square to : single_pushes) {
      if (is_promotion_rank(to, side)) {
        add_pawn_promotions(moves, from, to, side, false);
      } else {
        moves.emplace_back(from, to, std::nullopt, false, false, false);
      }
    }

    // Double push
    Bitboard double_pushes = double_push(side)[from.value()];
    double_pushes &= empty;
    if (single_pushes.any()) {
      for (Square to : double_pushes) {
        moves.emplace_back(from, to, std::nullopt, false, false, false);
      }
    }

    // Classic capture
    Bitboard cap = captures(side)[from.value()];
    cap &= enemy;
    for (Square to : cap) {
      if (is_promotion_rank(to, side)) {
        add_pawn_promotions(moves, from, to, side, true);
      } else {
        moves.emplace_back(from, to, std::nullopt, true, false, false);
      }
    }

    // En passant capture
    if (board.en_passant_square() && can_capture_en_passant(from, *board.en_passant_square(), side)) {
      moves.emplace_back(from, *board.en_passant_square(), std::nullopt, true, true, false);
    }
  }
}

void PawnMoveGenerator::add_pawn_promotions(std::vector<Move>& moves, Square from, Square to, Color side,
                                            bool capture) {
  const auto& promotion_pieces = (side == Color::WHITE) ? WHITE_PROMOTIONS : BLACK_PROMOTIONS;

  for (auto piece : promotion_pieces) {
    moves.emplace_back(from, to, piece, capture, false, false);
  }
}

void PawnMoveGenerator::generate_legal_moves(std::vector<Move>& moves, const Board& board, Color side) {
  // TODO
}
