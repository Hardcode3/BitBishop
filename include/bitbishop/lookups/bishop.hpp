#pragma once
#include <array>
#include <bitbishop/bitboard.hpp>
#include <bitbishop/bitmasks.hpp>
#include <bitbishop/constants.hpp>
#include <cstdint>

namespace Lookups {

/**
 * @brief Generates a bitboard of all squares a bishop can attack moving northeast from the given square.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a bishop moving northeast.
 */
constexpr uint64_t bishop_northeast_attacks(int square) {
  using namespace Const;

  uint64_t attacks = 0ULL;
  int rank = square / BOARD_WIDTH;
  int file = square % BOARD_WIDTH;
  int index = 1;
  while ((rank + index <= RANK_8_IND) && (file + index <= FILE_H_IND)) {
    attacks |= (1ULL << ((file + index) + (rank + index) * BOARD_WIDTH));
    ++index;
  }
  return attacks;
}

/**
 * @brief Generates a bitboard of all squares a bishop can attack moving northwest from the given square.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a bishop moving northwest.
 */
constexpr uint64_t bishop_northwest_attacks(int square) {
  using namespace Const;

  uint64_t attacks = 0ULL;
  int rank = square / BOARD_WIDTH;
  int file = square % BOARD_WIDTH;
  int index = 1;
  while ((rank + index <= RANK_8_IND) && (file - index >= FILE_A_IND)) {
    attacks |= (1ULL << ((file - index) + (rank + index) * BOARD_WIDTH));
    ++index;
  }
  return attacks;
}

/**
 * @brief Generates a bitboard of all squares a bishop can attack moving southeast from the given square.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a bishop moving southeast.
 */
constexpr uint64_t bishop_southeast_attacks(int square) {
  using namespace Const;

  uint64_t attacks = 0ULL;
  int rank = square / BOARD_WIDTH;
  int file = square % BOARD_WIDTH;
  int index = 1;
  while ((rank - index >= RANK_1_IND) && (file + index <= FILE_H_IND)) {
    attacks |= (1ULL << ((file + index) + (rank - index) * BOARD_WIDTH));
    ++index;
  }
  return attacks;
}

/**
 * @brief Generates a bitboard of all squares a bishop can attack moving southwest from the given square.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a bishop moving southwest.
 */
constexpr uint64_t bishop_southwest_attacks(int square) {
  using namespace Const;

  uint64_t attacks = 0ULL;
  int rank = square / BOARD_WIDTH;
  int file = square % BOARD_WIDTH;
  int index = 1;
  while ((rank - index >= RANK_1_IND) && (file - index >= FILE_A_IND)) {
    attacks |= (1ULL << ((file - index) + (rank - index) * BOARD_WIDTH));
    ++index;
  }
  return attacks;
}

/**
 * @brief Generates a bitboard of all squares a bishop can attack from the given square in all directions.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a bishop from the given square.
 */
constexpr uint64_t bishop_attacks_for_square(int square) {
  return bishop_northeast_attacks(square) | bishop_northwest_attacks(square) | bishop_southeast_attacks(square) |
         bishop_southwest_attacks(square);
}

/**
 * @brief Precomputed lookup table of bishop attacks for every square on the board.
 *
 * Each entry is a bitboard of all squares a bishop can attack from the corresponding square.
 * Indexed by square (0-63).
 */
constexpr std::array<Bitboard, Const::BOARD_SIZE> BISHOP_ATTACKS = []() constexpr {
  // TODO: remove this function, not used anymore
  // TODO: adapt the tests to use bishop attacks with directions instead
  // BISHOP_NORTHEAST_ATTACKS, ...
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(bishop_attacks_for_square(sq));
  }
  return table;
}();

/**
 * @brief Precomputed lookup table of bishop northeast attacks for every square.
 * Indexed by square (0-63).
 */
constexpr std::array<Bitboard, Const::BOARD_SIZE> BISHOP_NORTHEAST_ATTACKS = []() constexpr {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(bishop_northeast_attacks(sq));
  }
  return table;
}();

/**
 * @brief Precomputed lookup table of bishop northwest attacks for every square.
 * Indexed by square (0-63).
 */
constexpr std::array<Bitboard, Const::BOARD_SIZE> BISHOP_NORTHWEST_ATTACKS = []() constexpr {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(bishop_northwest_attacks(sq));
  }
  return table;
}();

/**
 * @brief Precomputed lookup table of bishop southeast attacks for every square.
 * Indexed by square (0-63).
 */
constexpr std::array<Bitboard, Const::BOARD_SIZE> BISHOP_SOUTHEAST_ATTACKS = []() constexpr {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(bishop_southeast_attacks(sq));
  }
  return table;
}();

/**
 * @brief Precomputed lookup table of bishop southwest attacks for every square.
 * Indexed by square (0-63).
 */
constexpr std::array<Bitboard, Const::BOARD_SIZE> BISHOP_SOUTHWEST_ATTACKS = []() constexpr {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(bishop_southwest_attacks(sq));
  }
  return table;
}();

}  // namespace Lookups