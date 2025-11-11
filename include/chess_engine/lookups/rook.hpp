#pragma once
#include <array>
#include <chess_engine/bitboard.hpp>
#include <chess_engine/bitmasks.hpp>
#include <cstdint>

namespace Attacks {

/**
 * @brief Generates a bitboard of all squares a rook can attack moving north from the given square.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a rook moving north.
 */
constexpr uint64_t rook_north_attacks(int sq) {
  uint64_t attacks = 0ULL;
  int r = sq / 8;
  int f = sq % 8;
  for (int i = r + 1; i < 8; ++i) {
    attacks |= (1ULL << (f + i * 8));
  }
  return attacks;
}

/**
 * @brief Generates a bitboard of all squares a rook can attack moving south from the given square.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a rook moving south.
 */
constexpr uint64_t rook_south_attacks(int sq) {
  uint64_t attacks = 0ULL;
  int r = sq / 8;
  int f = sq % 8;
  for (int i = r - 1; i >= 0; --i) {
    attacks |= (1ULL << (f + i * 8));
  }
  return attacks;
}

/**
 * @brief Generates a bitboard of all squares a rook can attack moving east from the given square.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a rook moving east.
 */
constexpr uint64_t rook_east_attacks(int sq) {
  uint64_t attacks = 0ULL;
  int r = sq / 8;
  int f = sq % 8;
  for (int i = f + 1; i < 8; ++i) {
    attacks |= (1ULL << (i + r * 8));
  }
  return attacks;
}

/**
 * @brief Generates a bitboard of all squares a rook can attack moving west from the given square.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a rook moving west.
 */
constexpr uint64_t rook_west_attacks(int sq) {
  uint64_t attacks = 0ULL;
  int r = sq / 8;
  int f = sq % 8;
  for (int i = f - 1; i >= 0; --i) {
    attacks |= (1ULL << (i + r * 8));
  }
  return attacks;
}

/**
 * @brief Generates a bitboard of all squares a rook can attack from the given square in all directions.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a rook from the given square.
 */
constexpr uint64_t rook_attacks_for_square(int sq) {
  return rook_north_attacks(sq) | rook_south_attacks(sq) | rook_east_attacks(sq) | rook_west_attacks(sq);
}

/**
 * @brief Precomputed lookup table of rook attacks for every square on the board.
 *
 * Each entry is a bitboard of all squares a rook can attack from the corresponding square.
 * Indexed by square (0-63).
 */
constexpr std::array<Bitboard, 64> ROOK_ATTACKS = []() constexpr {
  std::array<Bitboard, 64> table{};
  for (int sq = 0; sq < 64; ++sq) {
    table[sq] = Bitboard(rook_attacks_for_square(sq));
  }
  return table;
}();

}  // namespace Attacks
