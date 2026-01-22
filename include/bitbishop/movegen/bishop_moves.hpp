#pragma once

#include <bitbishop/attacks/bishop_attacks.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/movegen/pins.hpp>
#include <utility>
#include <vector>

/**
 * @brief Generate all legal bishop moves for the side to move.
 *
 * This function enumerates every bishop belonging to the given side and
 * generates all legal destination squares, taking into account:
 *
 * - Sliding attacks along diagonals
 * - Friendly piece blocking
 * - Enemy captures
 * - Active check constraints via @p check_mask
 * - Absolute pins via @p pins
 *
 * The resulting moves are appended to @p moves. Existing contents are
 * preserved.
 *
 * @param[out] moves
 *   Vector to which generated bishop moves will be appended.
 *
 * @param[in] board
 *   Current board position.
 *
 * @param[in] us
 *   Side to generate bishop moves for.
 *
 * @param[in] check_mask
 *   Bitboard restricting legal move destinations when the king is in check.
 *   Only moves intersecting this mask are generated.
 *
 * @param[in] pins
 *   Precomputed pin information. If a bishop is absolutely pinned, its legal
 *   moves are restricted to the corresponding pin ray.
 *
 * @note
 * - This function assumes that @p check_mask and @p pins have already been
 *   computed for the current position.
 */
inline void generate_bishop_legal_moves(std::vector<Move>& moves, const Board& board, Color us,
                                        const Bitboard& check_mask, const PinResult& pins) {
  const Bitboard own = board.friendly(us);
  const Bitboard enemy = board.enemy(us);
  const Bitboard occupied = board.occupied();
  Bitboard bishops = board.bishops(us);

  // warning: this loop is destructive on Bitboard bishops
  while (auto from_opt = bishops.pop_lsb()) {
    Square from = from_opt.value();

    Bitboard candidates = bishop_attacks(from, occupied);
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
