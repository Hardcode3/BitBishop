#pragma once
#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/lookups/bishop_rays.hpp>
#include <bitbishop/lookups/king_attacks.hpp>
#include <bitbishop/lookups/knight_attacks.hpp>
#include <bitbishop/lookups/pawn_attacks.hpp>
#include <bitbishop/lookups/queen_rays.hpp>
#include <bitbishop/lookups/rook_rays.hpp>
#include <bitbishop/square.hpp>

constexpr Bitboard attackers_to(Square target, Color color) {
  using namespace Lookups;
  const int square_index = target.value();

  Bitboard attackers;
  attackers |= KING_ATTACKERS[square_index];
  attackers |= KNIGHT_ATTACKERS[square_index];

  if (color == Color::WHITE) {
    attackers |= WHITE_PAWN_ATTACKERS[square_index];
  } else {
    attackers |= BLACK_PAWN_ATTACKERS[square_index];
  }

  attackers |= ROOK_ATTACKER_RAYS[square_index];
  attackers |= BISHOP_ATTACKER_RAYS[square_index];
  attackers |= QUEEN_ATTACKER_RAYS[square_index];

  return attackers;
}

constexpr std::array<std::array<Bitboard, Const::BOARD_SIZE>, ColorUtil::size()> ATTACKERS_TO = []() constexpr {
  using namespace Const;

  std::array<std::array<Bitboard, BOARD_SIZE>, ColorUtil::size()> table{};
  for (Color col : ColorUtil::all()) {
    const std::size_t coli = ColorUtil::to_index(col);
    for (int sq = 0; sq < BOARD_SIZE; ++sq) {
      table[coli][sq] = Bitboard(attackers_to(Square(sq, std::in_place), col));
    }
  }
  return table;
}();
