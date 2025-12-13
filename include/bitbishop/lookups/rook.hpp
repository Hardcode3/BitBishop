#pragma once
#include <array>
#include <bitbishop/bitboard.hpp>
#include <bitbishop/bitmasks.hpp>
#include <bitbishop/constants.hpp>
#include <cstdint>

namespace Lookups {

/**
 * @brief Generates a bitboard of all squares a rook can attack moving north from the given square.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a rook moving north.
 */
constexpr uint64_t rook_north_attacks(int square) {
  using namespace Const;

  uint64_t attacks = 0ULL;
  int rank = square / BOARD_WIDTH;
  int file = square % BOARD_WIDTH;
  for (int i = rank + 1; i < BOARD_WIDTH; ++i) {
    attacks |= (1ULL << (file + i * BOARD_WIDTH));
  }
  return attacks;
}

/**
 * @brief Generates a bitboard of all squares a rook can attack moving south from the given square.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a rook moving south.
 */
constexpr uint64_t rook_south_attacks(int square) {
  using namespace Const;

  uint64_t attacks = 0ULL;
  int rank = square / BOARD_WIDTH;
  int file = square % BOARD_WIDTH;
  for (int i = rank - 1; i >= 0; --i) {
    attacks |= (1ULL << (file + i * BOARD_WIDTH));
  }
  return attacks;
}

/**
 * @brief Generates a bitboard of all squares a rook can attack moving east from the given square.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a rook moving east.
 */
constexpr uint64_t rook_east_attacks(int square) {
  using namespace Const;

  uint64_t attacks = 0ULL;
  int rank = square / BOARD_WIDTH;
  int file = square % BOARD_WIDTH;
  for (int i = file + 1; i < BOARD_WIDTH; ++i) {
    attacks |= (1ULL << (i + rank * BOARD_WIDTH));
  }
  return attacks;
}

/**
 * @brief Generates a bitboard of all squares a rook can attack moving west from the given square.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a rook moving west.
 */
constexpr uint64_t rook_west_attacks(int square) {
  using namespace Const;

  uint64_t attacks = 0ULL;
  int rank = square / BOARD_WIDTH;
  int file = square % BOARD_WIDTH;
  for (int i = file - 1; i >= 0; --i) {
    attacks |= (1ULL << (i + rank * BOARD_WIDTH));
  }
  return attacks;
}

/**
 * @brief Generates a bitboard of all squares a rook can attack from the given square in all directions.
 * @param sq The square index (0-63).
 * @return Bitboard of all squares attacked by a rook from the given square.
 */
constexpr uint64_t rook_attacks_for_square(int square) {
  return rook_north_attacks(square) | rook_south_attacks(square) | rook_east_attacks(square) |
         rook_west_attacks(square);
}

/**
 * @brief Precomputed lookup table of rook attacks for every square on the board.
 *
 * Each entry is a bitboard of all squares a rook can attack from the corresponding square.
 * Indexed by square (0-63).
 */
constexpr std::array<Bitboard, Const::BOARD_SIZE> ROOK_ATTACKS = []() constexpr {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    table[sq] = Bitboard(rook_attacks_for_square(sq));
  }
  return table;
}();

}  // namespace Lookups
