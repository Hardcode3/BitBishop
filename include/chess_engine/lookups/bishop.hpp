#pragma once
#include <array>
#include <chess_engine/bitboard.hpp>
#include <chess_engine/bitmasks.hpp>
#include <cstdint>

namespace Attacks {

/**
 * @brief Generates a bitboard of all squares a bishop can attack moving northeast from the given square.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a bishop moving northeast.
 */
constexpr uint64_t bishop_northeast_attacks(int sq) {
  uint64_t attacks = 0ULL;
  int r = sq / 8;
  int f = sq % 8;
  int i = 1;
  while ((r + i < 8) && (f + i < 8)) {
    attacks |= (1ULL << ((f + i) + (r + i) * 8));
    ++i;
  }
  return attacks;
}

/**
 * @brief Generates a bitboard of all squares a bishop can attack moving northwest from the given square.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a bishop moving northwest.
 */
constexpr uint64_t bishop_northwest_attacks(int sq) {
  uint64_t attacks = 0ULL;
  int r = sq / 8;
  int f = sq % 8;
  int i = 1;
  while ((r + i < 8) && (f - i >= 0)) {
    attacks |= (1ULL << ((f - i) + (r + i) * 8));
    ++i;
  }
  return attacks;
}

/**
 * @brief Generates a bitboard of all squares a bishop can attack moving southeast from the given square.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a bishop moving southeast.
 */
constexpr uint64_t bishop_southeast_attacks(int sq) {
  uint64_t attacks = 0ULL;
  int r = sq / 8;
  int f = sq % 8;
  int i = 1;
  while ((r - i >= 0) && (f + i < 8)) {
    attacks |= (1ULL << ((f + i) + (r - i) * 8));
    ++i;
  }
  return attacks;
}

/**
 * @brief Generates a bitboard of all squares a bishop can attack moving southwest from the given square.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a bishop moving southwest.
 */
constexpr uint64_t bishop_southwest_attacks(int sq) {
  uint64_t attacks = 0ULL;
  int r = sq / 8;
  int f = sq % 8;
  int i = 1;
  while ((r - i >= 0) && (f - i >= 0)) {
    attacks |= (1ULL << ((f - i) + (r - i) * 8));
    ++i;
  }
  return attacks;
}

/**
 * @brief Generates a bitboard of all squares a bishop can attack from the given square in all directions.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a bishop from the given square.
 */
constexpr uint64_t bishop_attacks_for_square(int sq) {
  return bishop_northeast_attacks(sq) | bishop_northwest_attacks(sq) | bishop_southeast_attacks(sq) |
         bishop_southwest_attacks(sq);
}

/**
 * @brief Precomputed lookup table of bishop attacks for every square on the board.
 *
 * Each entry is a bitboard of all squares a bishop can attack from the corresponding square.
 * Indexed by square (0-63).
 */
constexpr std::array<Bitboard, 64> BISHOP_ATTACKS = []() constexpr {
  std::array<Bitboard, 64> table{};
  for (int sq = 0; sq < 64; ++sq) {
    table[sq] = Bitboard(bishop_attacks_for_square(sq));
  }
  return table;
}();

}  // namespace Attacks