#include <chess_engine/attacks/pawn.hpp>
#include <chess_engine/bitboard.hpp>
#include <chess_engine/color.hpp>
#include <chess_engine/movegen/pawn_move_gen.hpp>
#include <optional>

std::vector<Move> PawnMoveGenerator::generate_pseudo_legal_moves(const Board& b, Color side) const {
  // NOTE: Possible optimization here: there are faster alternatives to an iterator over the Bitboard object

  std::vector<Move> moves;

  Bitboard pawns = b.pawns(side);
  Bitboard empty = b.empty();
  Bitboard enemy = b.enemy(side);

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
    double_pushes &= single_push(side)[from.value()];
    for (Square to : double_pushes) {
      moves.emplace_back(from, to, std::nullopt, false, false, false);
    }

    // Classic capture
    Bitboard captures = attacks(side)[from.value()];
    captures &= enemy;
    for (Square to : captures) {
      if (is_promotion_rank(to, side)) {
        add_pawn_promotions(moves, from, to, side, true);
      } else {
        moves.emplace_back(from, to, std::nullopt, true, false, false);
      }
    }

    // En passant capture
    if (b.en_passant_square() && can_capture_en_passant(from, *b.en_passant_square(), side)) {
      moves.emplace_back(from, *b.en_passant_square(), std::nullopt, true, true, false);
    }
  }

  return moves;
}

void PawnMoveGenerator::add_pawn_promotions(std::vector<Move>& moves, Square from, Square to, Color side,
                                            bool capture) {
  std::array<Piece, 4> promotion_pieces;
  if (side == Color::WHITE) {
    promotion_pieces = {Piece(Piece::Q), Piece(Piece::R), Piece(Piece::B), Piece(Piece::N)};
  } else {
    promotion_pieces = {Piece(Piece::q), Piece(Piece::r), Piece(Piece::b), Piece(Piece::n)};
  }

  for (auto piece : promotion_pieces) {
    moves.emplace_back(from, to, piece, capture, false, false);
  }
}

std::vector<Move> PawnMoveGenerator::generate_legal_moves(const Board& board, Color side) const {
  std::vector<Move> moves;
  // TODO
  return moves;
}
