#pragma once

#include <bitbishop/attacks/queen_attacks.hpp>
#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/movegen/pins.hpp>
#include <vector>

/**
 * @brief Generate all legal queen moves for the side to move.
 *
 * This function enumerates every queen belonging to the given side and
 * generates all legal destination squares, taking into account:
 *
 * - Sliding attacks along ranks, files, and diagonals
 * - Friendly piece blocking
 * - Enemy captures
 * - Active check constraints via @p check_mask
 * - Absolute pins via @p pins
 *
 * The resulting moves are appended to @p moves. Existing contents are
 * preserved.
 *
 * @param[out] moves
 *   Vector to which generated queen moves will be appended.
 *
 * @param[in] board
 *   Current board position.
 *
 * @param[in] us
 *   Side to generate queen moves for.
 *
 * @param[in] check_mask
 *   Bitboard restricting legal move destinations when the king is in check.
 *   Only moves intersecting this mask are generated.
 *
 * @param[in] pins
 *   Precomputed pin information. If a queen is absolutely pinned, its legal
 *   moves are restricted to the corresponding pin ray.
 *
 * @note
 * - This function assumes that @p check_mask and @p pins have already been
 *   computed for the current position.
 * - Promotions, en passant, and castling are not applicable to queen moves.
 */
void generate_queen_legal_moves(std::vector<Move>& moves, const Board& board, Color us, const Bitboard& check_mask,
                                const PinResult& pins) {
  const Bitboard own = board.friendly(us);
  const Bitboard enemy = board.enemy(us);
  const Bitboard occupied = board.occupied();
  Bitboard queens = board.queens(us);

  // warning: this loop is destructive on bitboard queens
  while (auto from_opt = queens.pop_lsb()) {
    Square from = from_opt.value();

    Bitboard candidates = queen_attacks(from, occupied);
    candidates &= ~own;
    candidates &= check_mask;
    if (pins.pinned.test(from)) {
      candidates &= pins.pin_ray[from.value()];
    }

    for (Square to : candidates) {
      const bool is_capture = enemy.test(to);
      moves.emplace_back(Move::make(from, to, is_capture));
    }
  }
}
