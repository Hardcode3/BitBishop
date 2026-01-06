#pragma once

#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/lookups/king_attacks.hpp>
#include <bitbishop/move.hpp>
#include <utility>
#include <vector>

void generate_legal_king_moves(std::vector<Move>& moves, const Board& board, Color us, Square king_sq,
                               const Bitboard& enemy_attacks) {
  const Bitboard own = board.friendly(us);
  const Bitboard enemy = board.enemy(us);

  Bitboard candidates = Lookups::KING_ATTACKS[king_sq.value()];

  candidates &= ~own;
  candidates &= ~enemy_attacks;

  for (Square to : candidates) {
    const bool is_capture = enemy.test(to);
    moves.emplace_back(Move::make(king_sq, to, is_capture));
  }
}
