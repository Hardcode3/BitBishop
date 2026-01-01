#pragma once

#include <bitbishop/attacks/generate_attacks.hpp>
#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/lookups/attackers.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/movegen/bishop_moves.hpp>
#include <bitbishop/movegen/check_mask.hpp>
#include <bitbishop/movegen/king_moves.hpp>
#include <bitbishop/movegen/knight_moves.hpp>
#include <bitbishop/movegen/pawn_moves.hpp>
#include <bitbishop/movegen/pins.hpp>
#include <bitbishop/movegen/queen_moves.hpp>
#include <bitbishop/movegen/rook_moves.hpp>
#include <vector>

void generate_legal_moves(std::vector<Move> moves, const Board& board, Color us) {
  Square king_sq = board.king_square(us).value();
  Color them = ColorUtil::opposite(us);

  Bitboard checkers = attackers_to(king_sq, them);
  Bitboard check_mask = compute_check_mask(king_sq, checkers, board);
  PinResult pins = compute_pins(king_sq, board, us);
  Bitboard enemy_attacks = generate_attacks(board, them);

  generate_legal_king_moves(moves, board, us, king_sq, enemy_attacks, check_mask);

  if (checkers.count() > 1) {
    return;
  }

  generate_knight_legal_moves(moves, board, us, check_mask, pins);
  generate_bishop_legal_moves(moves, board, us, check_mask, pins);
  generate_rook_legal_moves(moves, board, us, check_mask, pins);
  generate_queen_legal_moves(moves, board, us, check_mask, pins);
  generate_pawn_legal_moves(moves, board, us, king_sq, check_mask, pins);

  // TODO generate legal castling moves
}
