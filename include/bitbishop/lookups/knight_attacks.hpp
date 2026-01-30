#pragma once

#include <array>
#include <bitbishop/bitboard.hpp>
#include <bitbishop/bitmasks.hpp>
#include <bitbishop/config.hpp>
#include <cstdint>

namespace Lookups {

/**
 * @brief Computes the attack bitboard for a knight on a given square.
 *
 * Given a square index (0-63), returns a bitboard with all squares attacked by a knight from that square.
 * Takes into account board edges to avoid wrap-around attacks.
 *
 * @param square The square index (0 = a1, 63 = h8).
 * @return Bitboard representing all possible knight attacks from the given square.
 */
CX_FN uint64_t knight_attacks_for_square(int square) {
  const uint64_t bitboard = 1ULL << square;
  uint64_t attacks = 0ULL;

  using namespace Bitmasks;

  // NOLINTBEGIN(readability-magic-numbers)
  attacks |= (bitboard << 17) & ~FILE_A;             // NNE
  attacks |= (bitboard << 15) & ~FILE_H;             // NNW
  attacks |= (bitboard << 10) & ~(FILE_A | FILE_B);  // ENN
  attacks |= (bitboard << 6) & ~(FILE_G | FILE_H);   // WNN
  attacks |= (bitboard >> 17) & ~FILE_H;             // SSE
  attacks |= (bitboard >> 15) & ~FILE_A;             // SSW
  attacks |= (bitboard >> 10) & ~(FILE_G | FILE_H);  // ESS
  attacks |= (bitboard >> 6) & ~(FILE_A | FILE_B);   // WSS
  // NOLINTEND(readability-magic-numbers)

  return attacks;
}

/**
 * @brief Precomputed bitboards for knight attacks from every square.
 *
 * For each square, contains a bitboard with all destination squares a knight can attack from that square.
 * Uses knight_attacks_for_square() to fill the table at compile time.
 */
CX_INLINE std::array<Bitboard, Const::BOARD_SIZE> KNIGHT_ATTACKS = []() CX_EXEC {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(knight_attacks_for_square(sq));
  }
  return table;
}();

/**
 * @brief Precomputed bitboards of knight attackers.
 *
 * For each target square, this table contains a bitboard of all squares
 * from which a knight could attack that square.
 *
 * Knight attack geometry:
 * - Attacks are symmetric (attackers == attacks)
 * - No directionality
 * - Independent of board occupancy
 *
 * As a result, the knight attacker table is identical to the knight attack table.
 *
 * Indexed by target square (0–63).
 */
CX_INLINE std::array<Bitboard, Const::BOARD_SIZE> KNIGHT_ATTACKERS = KNIGHT_ATTACKS;

}  // namespace Lookups