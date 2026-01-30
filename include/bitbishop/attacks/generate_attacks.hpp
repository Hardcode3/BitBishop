#pragma once

#include <bitbishop/attacks/bishop_attacks.hpp>
#include <bitbishop/attacks/queen_attacks.hpp>
#include <bitbishop/attacks/rook_attacks.hpp>
#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/config.hpp>
#include <bitbishop/lookups/king_attacks.hpp>
#include <bitbishop/lookups/knight_attacks.hpp>
#include <bitbishop/lookups/pawn_attacks.hpp>

/**
 * @brief Computes the set of squares attacked by all pieces of a given side,
 *        assuming the opposing king is not present on the board.
 *
 * This attack map is suitable for validating king moves and castling legality.
 * Sliding attacks (rook, bishop, queen) are generated with the opposing king
 * removed from occupancy to correctly model x-ray attacks.
 *
 * The result represents geometric attacks only and does not account for
 * move legality, pins, or whether attacking pieces are defended.
 *
 * @param board The current board position.
 * @param enemy The side whose attacks are to be generated.
 * @return A bitboard of all squares attacked by the given side.
 */
CX_FN Bitboard generate_attacks(const Board& board, Color enemy) {
  Bitboard attacks = Bitboard::Zeros();

  Bitboard occupied_no_king = board.occupied() ^ board.king(ColorUtil::opposite(enemy));

  Bitboard king = board.king(enemy);
  if (king.any()) {
    Square sq = king.pop_lsb().value();
    attacks |= Lookups::KING_ATTACKS[sq.value()];
  }

  Bitboard knights = board.knights(enemy);
  while (knights) {
    Square sq = knights.pop_lsb().value();
    attacks |= Lookups::KNIGHT_ATTACKS[sq.value()];
  }

  Bitboard pawns = board.pawns(enemy);
  if (enemy == Color::WHITE) {
    while (pawns) {
      Square sq = pawns.pop_lsb().value();
      attacks |= Lookups::WHITE_PAWN_ATTACKS[sq.value()];
    }
  } else {
    while (pawns) {
      Square sq = pawns.pop_lsb().value();
      attacks |= Lookups::BLACK_PAWN_ATTACKS[sq.value()];
    }
  }

  Bitboard rooks = board.rooks(enemy);
  while (rooks) {
    Square sq = rooks.pop_lsb().value();
    attacks |= rook_attacks(sq, occupied_no_king);
  }

  Bitboard bishops = board.bishops(enemy);
  while (bishops) {
    Square sq = bishops.pop_lsb().value();
    attacks |= bishop_attacks(sq, occupied_no_king);
  }

  Bitboard queens = board.queens(enemy);
  while (queens) {
    Square sq = queens.pop_lsb().value();
    attacks |= queen_attacks(sq, occupied_no_king);
  }

  return attacks;
}
