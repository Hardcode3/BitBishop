#pragma once

#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/lookups/knight_attacks.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/movegen/pins.hpp>
#include <utility>
#include <vector>

// pinned knights cannot move at all due to knight's l-shaped move geometry
void generate_knight_legal_moves(std::vector<Move>& moves, const Board& board, Color us, const Bitboard& check_mask,
                                 const PinResult& pins) {
  const Bitboard own = board.friendly(us);
  const Bitboard enemy = board.enemy(us);
  Bitboard knights = board.knights(us);
  knights &= ~pins.pinned;

  // warning: this loop is destructive on Bitboard knights
  while (auto from_opt = knights.pop_lsb()) {
    Square from = from_opt.value();

    Bitboard candidates = Lookups::KNIGHT_ATTACKS[from.value()];
    candidates &= ~own;
    candidates &= check_mask;

    for (Square to : candidates) {
      const bool is_capture = enemy.test(to);
      moves.emplace_back(from, to, std::nullopt, is_capture, false, false);
    }
  }
}
