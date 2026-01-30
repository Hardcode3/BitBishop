#pragma once

#include <array>
#include <bitbishop/bitboard.hpp>
#include <bitbishop/bitmasks.hpp>
#include <bitbishop/config.hpp>
#include <bitbishop/constants.hpp>
#include <cstdint>

namespace Lookups {

/**
 * @brief Returns the full northeast diagonal ray from a square.
 *
 * Computes all squares reachable by a bishop moving northeast from the given
 * square, ignoring board occupancy. The ray extends until the edge of the board.
 *
 * This function is pure geometry and is intended for compile-time lookup
 * generation.
 *
 * @param square The starting square index (0–63)
 * @return Bitboard of all squares on the northeast diagonal from the square
 */
CX_FN uint64_t bishop_northeast_ray(int square) {
  using namespace Const;

  uint64_t ray = 0ULL;
  int rank = square / BOARD_WIDTH;
  int file = square % BOARD_WIDTH;
  int index = 1;

  while ((rank + index <= RANK_8_IND) && (file + index <= FILE_H_IND)) {
    ray |= (1ULL << ((file + index) + (rank + index) * BOARD_WIDTH));
    ++index;
  }

  return ray;
}

/**
 * @brief Returns the full northwest diagonal ray from a square.
 *
 * Computes all squares reachable by a bishop moving northwest from the given
 * square, ignoring board occupancy.
 *
 * @param square The starting square index (0–63)
 * @return Bitboard of all squares on the northwest diagonal from the square
 */
CX_FN uint64_t bishop_northwest_ray(int square) {
  using namespace Const;

  uint64_t ray = 0ULL;
  int rank = square / BOARD_WIDTH;
  int file = square % BOARD_WIDTH;
  int index = 1;

  while ((rank + index <= RANK_8_IND) && (file - index >= FILE_A_IND)) {
    ray |= (1ULL << ((file - index) + (rank + index) * BOARD_WIDTH));
    ++index;
  }

  return ray;
}

/**
 * @brief Returns the full southeast diagonal ray from a square.
 *
 * Computes all squares reachable by a bishop moving southeast from the given
 * square, ignoring board occupancy.
 *
 * @param square The starting square index (0–63)
 * @return Bitboard of all squares on the southeast diagonal from the square
 */
CX_FN uint64_t bishop_southeast_ray(int square) {
  using namespace Const;

  uint64_t ray = 0ULL;
  int rank = square / BOARD_WIDTH;
  int file = square % BOARD_WIDTH;
  int index = 1;

  while ((rank - index >= RANK_1_IND) && (file + index <= FILE_H_IND)) {
    ray |= (1ULL << ((file + index) + (rank - index) * BOARD_WIDTH));
    ++index;
  }

  return ray;
}

/**
 * @brief Returns the full southwest diagonal ray from a square.
 *
 * Computes all squares reachable by a bishop moving southwest from the given
 * square, ignoring board occupancy.
 *
 * @param square The starting square index (0–63)
 * @return Bitboard of all squares on the southwest diagonal from the square
 */
CX_FN uint64_t bishop_southwest_ray(int square) {
  using namespace Const;

  uint64_t ray = 0ULL;
  int rank = square / BOARD_WIDTH;
  int file = square % BOARD_WIDTH;
  int index = 1;

  while ((rank - index >= RANK_1_IND) && (file - index >= FILE_A_IND)) {
    ray |= (1ULL << ((file - index) + (rank - index) * BOARD_WIDTH));
    ++index;
  }

  return ray;
}

/**
 * @brief Returns the union of all diagonal rays from a square.
 *
 * Combines the four diagonal rays (NE, NW, SE, SW) reachable by a bishop from
 * the given square, ignoring board occupancy.
 *
 * @param square The starting square index (0–63)
 * @return Bitboard of all diagonal ray squares from the square
 */
CX_FN uint64_t bishop_rays_for_square(int square) {
  return bishop_northeast_ray(square) | bishop_northwest_ray(square) | bishop_southeast_ray(square) |
         bishop_southwest_ray(square);
}

/**
 * @brief Precomputed lookup table of bishop diagonal rays.
 *
 * For each square, stores the union of all diagonal rays reachable by a bishop
 * from that square, ignoring board occupancy.
 *
 * Indexed by square (0–63).
 */
CX_FN std::array<Bitboard, Const::BOARD_SIZE> BISHOP_RAYS = []() CX_EXEC {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(bishop_rays_for_square(sq));
  }
  return table;
}();

/**
 * @brief Precomputed lookup table of bishop northeast diagonal rays.
 *
 * Indexed by square (0–63). Board occupancy is ignored.
 */
CX_FN std::array<Bitboard, Const::BOARD_SIZE> BISHOP_NORTHEAST_RAYS = []() CX_EXEC {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(bishop_northeast_ray(sq));
  }
  return table;
}();

/**
 * @brief Precomputed lookup table of bishop northwest diagonal rays.
 *
 * Indexed by square (0–63). Board occupancy is ignored.
 */
CX_FN std::array<Bitboard, Const::BOARD_SIZE> BISHOP_NORTHWEST_RAYS = []() CX_EXEC {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(bishop_northwest_ray(sq));
  }
  return table;
}();

/**
 * @brief Precomputed lookup table of bishop southeast diagonal rays.
 *
 * Indexed by square (0–63). Board occupancy is ignored.
 */
CX_FN std::array<Bitboard, Const::BOARD_SIZE> BISHOP_SOUTHEAST_RAYS = []() CX_EXEC {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(bishop_southeast_ray(sq));
  }
  return table;
}();

/**
 * @brief Precomputed lookup table of bishop southwest diagonal rays.
 *
 * Indexed by square (0–63). Board occupancy is ignored.
 */
CX_FN std::array<Bitboard, Const::BOARD_SIZE> BISHOP_SOUTHWEST_RAYS = []() CX_EXEC {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(bishop_southwest_ray(sq));
  }
  return table;
}();

/**
 * @brief Precomputed bitboards of bishop attackers (ray-based).
 *
 * For each target square, this table contains a bitboard of all squares
 * from which a bishop could attack that square, assuming an empty board.
 *
 * Bishop attack geometry:
 * - Attacks are ray-based (diagonal directions)
 * - Directional and not symmetric
 * - Independent of board occupancy at this level
 *
 * For sliding pieces, the set of potential attackers to a square is
 * exactly the set of squares lying on the corresponding rays.
 *
 * Indexed by target square (0–63).
 */
CX_FN std::array<Bitboard, Const::BOARD_SIZE> BISHOP_ATTACKER_RAYS = BISHOP_RAYS;

}  // namespace Lookups
