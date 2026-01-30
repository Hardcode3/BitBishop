#pragma once

#include <array>
#include <bitbishop/bitboard.hpp>
#include <bitbishop/bitmasks.hpp>
#include <bitbishop/config.hpp>
#include <bitbishop/constants.hpp>
#include <bitbishop/lookups/bishop_rays.hpp>
#include <bitbishop/lookups/rook_rays.hpp>
#include <cstdint>

namespace Lookups {

/**
 * @brief Generates all queen rays from a given square, independent of board occupancy.
 *
 * Queen rays are defined as the union of rook rays (orthogonal directions)
 * and bishop rays (diagonal directions) originating from the same square.
 *
 * @param square The starting square index (0–63).
 * @return Bitboard containing all squares reachable by queen rays.
 */
CX_FN uint64_t queen_rays_for_square(int square) {
  return rook_rays_for_square(square) | bishop_rays_for_square(square);
}

/**
 * @brief Precomputed lookup table of queen rays for every square, independent of board occupancy.
 *
 * Each entry contains the union of all rook and bishop rays originating
 * from the corresponding square.
 *
 * Indexed by square (0–63).
 */
CX_INLINE std::array<Bitboard, Const::BOARD_SIZE> QUEEN_RAYS = []() CX_EXEC {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(queen_rays_for_square(sq));
  }
  return table;
}();

/**
 * @brief Precomputed bitboards of queen attackers (composite ray-based).
 *
 * For each target square, this table contains a bitboard of all squares
 * from which a queen could attack that square, assuming an empty board.
 *
 * Queen attack geometry:
 * - Attacks are ray-based (diagonal + orthogonal directions)
 * - Directional and not symmetric
 * - Independent of board occupancy at this level
 *
 * Queen attackers are defined as the union of rook attackers and
 * bishop attackers for the same target square.
 *
 * Indexed by target square (0–63).
 */
CX_INLINE std::array<Bitboard, Const::BOARD_SIZE> QUEEN_ATTACKER_RAYS = QUEEN_RAYS;

}  // namespace Lookups
