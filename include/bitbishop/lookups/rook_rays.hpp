#pragma once

#include <array>
#include <bitbishop/bitboard.hpp>
#include <bitbishop/bitmasks.hpp>
#include <bitbishop/config.hpp>
#include <bitbishop/constants.hpp>
#include <cstdint>

namespace Lookups {

/**
 * @brief Generates the northward rook ray from a given square, independent of board occupancy.
 *
 * The ray includes all squares strictly north of the starting square
 * up to the edge of the board.
 *
 * @param square The starting square index (0–63).
 * @return Bitboard containing all squares on the north ray.
 */
CX_FN uint64_t rook_north_ray(int square) {
  using namespace Const;

  uint64_t ray = 0ULL;
  int rank = square / BOARD_WIDTH;
  int file = square % BOARD_WIDTH;

  for (int r = rank + 1; r < BOARD_WIDTH; ++r) {
    ray |= (1ULL << (file + r * BOARD_WIDTH));
  }
  return ray;
}

/**
 * @brief Generates the southward rook ray from a given square, independent of board occupancy.
 *
 * The ray includes all squares strictly south of the starting square
 * up to the edge of the board.
 *
 * @param square The starting square index (0–63).
 * @return Bitboard containing all squares on the south ray.
 */
CX_FN uint64_t rook_south_ray(int square) {
  using namespace Const;

  uint64_t ray = 0ULL;
  int rank = square / BOARD_WIDTH;
  int file = square % BOARD_WIDTH;

  for (int r = rank - 1; r >= 0; --r) {
    ray |= (1ULL << (file + r * BOARD_WIDTH));
  }
  return ray;
}

/**
 * @brief Generates the eastward rook ray from a given square, independent of board occupancy.
 *
 * The ray includes all squares strictly east of the starting square
 * up to the edge of the board.
 *
 * @param square The starting square index (0–63).
 * @return Bitboard containing all squares on the east ray.
 */
CX_FN uint64_t rook_east_ray(int square) {
  using namespace Const;

  uint64_t ray = 0ULL;
  int rank = square / BOARD_WIDTH;
  int file = square % BOARD_WIDTH;

  for (int f = file + 1; f < BOARD_WIDTH; ++f) {
    ray |= (1ULL << (f + rank * BOARD_WIDTH));
  }
  return ray;
}

/**
 * @brief Generates the westward rook ray from a given square, independent of board occupancy.
 *
 * The ray includes all squares strictly west of the starting square
 * up to the edge of the board.
 *
 * @param square The starting square index (0–63).
 * @return Bitboard containing all squares on the west ray.
 */
CX_FN uint64_t rook_west_ray(int square) {
  using namespace Const;

  uint64_t ray = 0ULL;
  int rank = square / BOARD_WIDTH;
  int file = square % BOARD_WIDTH;

  for (int f = file - 1; f >= 0; --f) {
    ray |= (1ULL << (f + rank * BOARD_WIDTH));
  }
  return ray;
}

/**
 * @brief Generates all rook rays from a given square, independent of board occupancy.
 *
 * This is the union of the north, south, east, and west rays.
 *
 * @param square The starting square index (0–63).
 * @return Bitboard containing all squares reachable by rook rays.
 */
CX_FN uint64_t rook_rays_for_square(int square) {
  return rook_north_ray(square) | rook_south_ray(square) | rook_east_ray(square) | rook_west_ray(square);
}

/**
 * @brief Precomputed lookup table of rook rays for every square, independent of board occupancy.
 *
 * Each entry contains the union of all four directional rook rays
 * originating from the corresponding square.
 *
 * Indexed by square (0–63).
 */
CX_INLINE std::array<Bitboard, Const::BOARD_SIZE> ROOK_RAYS = []() CX_EXEC {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(rook_rays_for_square(sq));
  }
  return table;
}();

/**
 * @brief Precomputed lookup table of northward rook rays for every square,
 * independent of board occupancy.
 *
 * Indexed by square (0–63).
 */
CX_INLINE std::array<Bitboard, Const::BOARD_SIZE> ROOK_NORTH_RAYS = []() CX_EXEC {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(rook_north_ray(sq));
  }
  return table;
}();

/**
 * @brief Precomputed lookup table of southward rook rays for every square,
 * independent of board occupancy.
 *
 * Indexed by square (0–63).
 */
CX_INLINE std::array<Bitboard, Const::BOARD_SIZE> ROOK_SOUTH_RAYS = []() CX_EXEC {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(rook_south_ray(sq));
  }
  return table;
}();

/**
 * @brief Precomputed lookup table of eastward rook rays for every square,
 * independent of board occupancy.
 *
 * Indexed by square (0–63).
 */
CX_INLINE std::array<Bitboard, Const::BOARD_SIZE> ROOK_EAST_RAYS = []() CX_EXEC {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(rook_east_ray(sq));
  }
  return table;
}();

/**
 * @brief Precomputed lookup table of westward rook rays for every square,
 * independent of board occupancy.
 *
 * Indexed by square (0–63).
 */
CX_INLINE std::array<Bitboard, Const::BOARD_SIZE> ROOK_WEST_RAYS = []() CX_EXEC {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(rook_west_ray(sq));
  }
  return table;
}();

/**
 * @brief Precomputed bitboards of rook attackers (ray-based).
 *
 * For each target square, this table contains a bitboard of all squares
 * from which a rook could attack that square, assuming an empty board.
 *
 * Rook attack geometry:
 * - Attacks are ray-based (orthogonal directions)
 * - Directional and not symmetric
 * - Independent of board occupancy at this level
 *
 * For sliding pieces, the set of potential attackers to a square is
 * exactly the set of squares lying on the corresponding rays.
 *
 * Indexed by target square (0–63).
 */
CX_INLINE std::array<Bitboard, Const::BOARD_SIZE> ROOK_ATTACKER_RAYS = ROOK_RAYS;

}  // namespace Lookups
