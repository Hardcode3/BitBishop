#pragma once
#include <array>
#include <chess_engine/bitboard.hpp>
#include <chess_engine/bitmasks.hpp>
#include <cstdint>

namespace Attacks {


/**
 * @brief Computes the attack bitboard for a king on a given square.
 *
 * Given a square index (0-63), returns a bitboard with all squares attacked by a king from that square.
 * Takes into account board edges to avoid wrap-around attacks.
 *
 * @param sq The square index (0 = a1, 63 = h8).
 * @return Bitboard representing all possible king attacks from the given square.
 */
constexpr uint64_t king_attacks_for_square(int sq) {
  const uint64_t bb = 1ULL << sq;
  uint64_t attacks = 0ULL;

  using namespace Bitmasks;

  // Cardinal directions
  // No wrap around for N/S moves because the number falls off 64-bit integer
  // aka. <<8 on rank 8 make the N attack square fall off 64-bit integer
  // Hence it's 0, no horizontal wrap around
  attacks |= (bb << 8);            // North
  attacks |= (bb >> 8);            // South
  attacks |= (bb << 1) & ~FILE_A;  // East
  attacks |= (bb >> 1) & ~FILE_H;  // West

  // Diagonals
  attacks |= (bb << 9) & ~FILE_A;  // North-East
  attacks |= (bb << 7) & ~FILE_H;  // North-West
  attacks |= (bb >> 7) & ~FILE_A;  // South-East
  attacks |= (bb >> 9) & ~FILE_H;  // South-West

  return attacks;
}


/**
 * @brief Precomputed bitboards for king attacks from every square.
 *
 * For each square, contains a bitboard with all destination squares a king can attack from that square.
 * Uses king_attacks_for_square() to fill the table at compile time.
 */
constexpr std::array<Bitboard, 64> KING_ATTACKS = []() constexpr {
  std::array<Bitboard, 64> table{};
  for (int sq = 0; sq < 64; ++sq) {
    table[sq] = Bitboard(king_attacks_for_square(sq));
  }
  return table;
}();

}  // namespace Attacks