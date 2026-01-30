#pragma once

#include <array>
#include <bitbishop/bitboard.hpp>
#include <bitbishop/bitmasks.hpp>
#include <bitbishop/config.hpp>
#include <cstdint>

namespace Lookups {

/**
 * @brief Computes the attack bitboard for a king on a given square.
 *
 * Given a square index (0-63), returns a bitboard with all squares attacked by a king from that square.
 * Takes into account board edges to avoid wrap-around attacks.
 *
 * @param square The square index (0 = a1, 63 = h8).
 * @return Bitboard representing all possible king attacks from the given square.
 */
CX_FN uint64_t king_attacks_for_square(int square) {
  const uint64_t bitboard = 1ULL << square;
  uint64_t attacks = 0ULL;

  using namespace Bitmasks;
  using namespace Const;

  // Cardinal directions
  // No wrap around for N/S moves because the number falls off 64-bit integer
  // aka. <<8 on rank 8 make the N attack square fall off 64-bit integer
  // Hence it's 0, no horizontal wrap around
  // NOLINTBEGIN(readability-magic-numbers)
  attacks |= (bitboard << 8);            // North
  attacks |= (bitboard >> 8);            // South
  attacks |= (bitboard << 1) & ~FILE_A;  // East
  attacks |= (bitboard >> 1) & ~FILE_H;  // West

  // Diagonals
  attacks |= (bitboard << 9) & ~FILE_A;  // North-East
  attacks |= (bitboard << 7) & ~FILE_H;  // North-West
  attacks |= (bitboard >> 7) & ~FILE_A;  // South-East
  attacks |= (bitboard >> 9) & ~FILE_H;  // South-West
  // NOLINTEND(readability-magic-numbers)

  return attacks;
}

/**
 * @brief Precomputed bitboards for king attacks from every square.
 *
 * For each square, contains a bitboard with all destination squares a king can attack from that square.
 * Uses king_attacks_for_square() to fill the table at compile time.
 */
CX_FN std::array<Bitboard, Const::BOARD_SIZE> KING_ATTACKS = []() CX_EXEC {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(king_attacks_for_square(sq));
  }
  return table;
}();

/**
 * @brief Precomputed bitboards of king attackers.
 *
 * For each target square, this table contains a bitboard of all squares
 * from which a king could attack that square.
 *
 * King attack geometry:
 * - Attacks are symmetric (attackers == attacks)
 * - No directionality
 * - Independent of board occupancy
 *
 * As a result, the king attacker table is identical to the king attack table.
 *
 * Indexed by target square (0–63).
 */
CX_FN std::array<Bitboard, Const::BOARD_SIZE> KING_ATTACKERS = KING_ATTACKS;

}  // namespace Lookups