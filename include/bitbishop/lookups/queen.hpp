#pragma once
#include <array>
#include <bitbishop/bitboard.hpp>
#include <bitbishop/bitmasks.hpp>
#include <bitbishop/constants.hpp>
#include <bitbishop/lookups/bishop.hpp>
#include <bitbishop/lookups/rook.hpp>
#include <cstdint>

namespace Lookups {

/**
 * @brief Generates a bitboard of all squares a queen can attack from the given square in all directions.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a queen from the given square.
 *
 * Note that queen moves is the combination of rook and bishop moves.
 */
constexpr uint64_t queen_attacks_for_square(int square) {
  return rook_attacks_for_square(square) | bishop_attacks_for_square(square);
}

/**
 * @brief Precomputed lookup table of queen attacks for every square on the board.
 *
 * Each entry is a bitboard of all squares a queen can attack from the corresponding square.
 * Indexed by square (0-63).
 */
constexpr std::array<Bitboard, Const::BOARD_SIZE> QUEEN_ATTACKS = []() constexpr {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(queen_attacks_for_square(sq));
  }
  return table;
}();

}  // namespace Lookups
