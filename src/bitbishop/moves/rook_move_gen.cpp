#include <bitbishop/attacks/rook_attacks.hpp>
#include <bitbishop/moves/rook_move_gen.hpp>
#include <optional>

void RookMoveGenerator::generate_pseudo_legal_moves(std::vector<Move>& moves, const Board& board, Color side) {
  Bitboard rooks = board.rooks(side);
  Bitboard empty = board.unoccupied();
  Bitboard enemy = board.enemy(side);
  Bitboard occupied = board.occupied();

  // warning: this loop is destructive on Bitboard rooks
  while (auto from_opt = rooks.pop_lsb()) {
    Square from = from_opt.value();

    Bitboard attacks = rook_attacks(from, occupied);

    // Silent moves
    Bitboard rook_moves = attacks & empty;
    for (auto to : rook_moves) {
      moves.emplace_back(from, to, std::nullopt, false, false, false);
    }

    // Captures
    Bitboard rook_captures = attacks & enemy;
    for (auto to : rook_captures) {
      moves.emplace_back(from, to, std::nullopt, true, false, false);
    }

    // Castling moves (pseudo-legal only checks piece positions, not attacks)
    add_rook_castling(moves, from, side, board);
  }
}

void RookMoveGenerator::add_rook_castling(std::vector<Move>& moves, Square from, Color side, const Board& board) {
  // TODO: These Castling utilities must be moved out of the KingMoveGenerator namespace
  // as it is common code with RookMoveGenerator
  Square kingside_rook = (side == Color::WHITE) ? Squares::H1 : Squares::H8;
  Square queenside_rook = (side == Color::WHITE) ? Squares::A1 : Squares::A8;

  // Kingside castling
  const bool can_castle_kingside = board.can_castle_kingside(side) && (from == kingside_rook);
  if (can_castle_kingside) {
    Square to = (side == Color::WHITE) ? Squares::F1 : Squares::F8;
    moves.emplace_back(from, to, std::nullopt, false, false, true);
  }

  // Queenside castling
  const bool can_castle_queenside = board.can_castle_queenside(side) && (from == queenside_rook);
  if (can_castle_queenside) {
    Square to = (side == Color::WHITE) ? Squares::D1 : Squares::D8;
    moves.emplace_back(from, to, std::nullopt, false, false, true);
  }
}
