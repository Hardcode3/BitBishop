#pragma once
#include <array>
#include <chess_engine/bitboard.hpp>
#include <chess_engine/bitmasks.hpp>
#include <chess_engine/lookups/bishop.hpp>
#include <chess_engine/lookups/rook.hpp>
#include <cstdint>

namespace Attacks {

/**
 * @brief Generates a bitboard of all squares a queen can attack from the given square in all directions.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a queen from the given square.
 *
 * Note that queen moves is the combination of rook and bishop moves.
 */
constexpr uint64_t queen_attacks_for_square(int sq) {
  return rook_attacks_for_square(sq) | bishop_attacks_for_square(sq);
}

/**
 * @brief Precomputed lookup table of queen attacks for every square on the board.
 *
 * Each entry is a bitboard of all squares a queen can attack from the corresponding square.
 * Indexed by square (0-63).
 */
constexpr std::array<Bitboard, 64> QUEEN_ATTACKS = []() constexpr {
  std::array<Bitboard, 64> table{};
  for (int sq = 0; sq < 64; ++sq) {
    table[sq] = Bitboard(queen_attacks_for_square(sq));
  }
  return table;
}();

}  // namespace Attacks
