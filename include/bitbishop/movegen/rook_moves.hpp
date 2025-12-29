#pragma once

#include <bitbishop/attacks/rook_attacks.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/movegen/pins.hpp>
#include <utility>
#include <vector>

/**
 * @brief Generate all legal rook moves for the side to move.
 *
 * This function enumerates every rook belonging to the given side and
 * generates all legal destination squares, taking into account:
 *
 * - Occupied squares on the board
 * - Friendly piece blocking
 * - Enemy captures
 * - Active check constraints via @p check_mask
 * - Absolute pins via @p pins
 *
 * The resulting moves are appended to @p moves. Existing contents are
 * preserved.
 *
 * @param[out] moves
 *   Vector to which generated rook moves will be appended.
 *
 * @param[in] board
 *   Current board position.
 *
 * @param[in] us
 *   Side to generate rook moves for.
 *
 * @param[in] check_mask
 *   Bitboard restricting legal move destinations when the king is in check.
 *   Only moves intersecting this mask are generated.
 *
 * @param[in] pins
 *   Precomputed pin information. If a rook is absolutely pinned, its legal
 *   moves are restricted to the corresponding pin ray.
 *
 * @note
 * - This function assumes that @p check_mask and @p pins have already been
 *   computed for the current position.
 * - Promotions, en passant, and castling are not applicable to rook moves.
 */
void generate_rook_legal_moves(std::vector<Move>& moves, const Board& board, Color us, const Bitboard& check_mask,
                               const PinResult& pins) {
  const Bitboard own = board.friendly(us);
  const Bitboard enemy = board.enemy(us);
  const Bitboard occupied = board.occupied();
  Bitboard rooks = board.rooks(us);

  // warning: this loop is destructive on Bitboard rooks
  while (auto from_opt = rooks.pop_lsb()) {
    Square from = from_opt.value();

    Bitboard candidates = rook_attacks(from, occupied);
    candidates &= ~own;
    candidates &= check_mask;
    if (pins.pinned.test(from)) {
      candidates &= pins.pin_ray[from.value()];
    }

    for (Square to : candidates) {
      const bool is_capture = enemy.test(to);
      moves.emplace_back(from, to, std::nullopt, is_capture, false, false);
    }
  }
}
