#pragma once
#include <array>
#include <bitbishop/bitboard.hpp>
#include <bitbishop/bitmasks.hpp>
#include <bitbishop/constants.hpp>
#include <cstdint>

namespace Lookups {

/**
 * @brief Precomputed bitboards for white pawn single pushes (1 square forward).
 *
 * For each square, contains a bitboard with the destination square if a white pawn moves one square forward.
 * Only valid for squares on ranks 1 to 7 (0-based), as pawns on rank 8 cannot move forward.
 */
constexpr std::array<Bitboard, Const::BOARD_SIZE> WHITE_PAWN_SINGLE_PUSH = []() constexpr {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    // ranks 1..7 can push north (+8) for white pawns
    if (sq / BOARD_WIDTH < RANK_8_IND) {
      table[sq] = Bitboard(1ULL << (sq + BOARD_WIDTH));
    }
  }
  return table;
}();

/**
 * @brief Precomputed bitboards for black pawn single pushes (1 square forward).
 *
 * For each square, contains a bitboard with the destination square if a black pawn moves one square forward.
 * Only valid for squares on ranks 2 to 8 (0-based), as pawns on rank 1 cannot move forward.
 */
constexpr std::array<Bitboard, Const::BOARD_SIZE> BLACK_PAWN_SINGLE_PUSH = []() constexpr {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    // ranks 2..8 can push south for black pawns
    if (sq / BOARD_WIDTH > RANK_1_IND) {
      table[sq] = Bitboard(1ULL << (sq - BOARD_WIDTH));
    }
  }
  return table;
}();

/**
 * @brief Precomputed bitboards for white pawn double pushes (2 squares forward).
 *
 * For each square, contains a bitboard with the destination square if a white pawn moves two squares forward.
 * Only valid for squares on rank 2 (0-based), as only pawns on their starting rank can double push.
 */
constexpr std::array<Bitboard, Const::BOARD_SIZE> WHITE_PAWN_DOUBLE_PUSH = []() constexpr {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    // white can only double push from rank 2
    if (sq / BOARD_WIDTH == RANK_2_IND) {
      table[sq] = Bitboard(1ULL << (sq + 2 * BOARD_WIDTH));
    }
  }
  return table;
}();

/**
 * @brief Precomputed bitboards for black pawn double pushes (2 squares forward).
 *
 * For each square, contains a bitboard with the destination square if a black pawn moves two squares forward.
 * Only valid for squares on rank 7 (0-based), as only pawns on their starting rank can double push.
 */
constexpr std::array<Bitboard, Const::BOARD_SIZE> BLACK_PAWN_DOUBLE_PUSH = []() constexpr {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};
  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    // black can only double push from rank 7
    if (sq / BOARD_WIDTH == RANK_7_IND) {
      table[sq] = Bitboard(1ULL << (sq - 2 * BOARD_WIDTH));
    }
  }
  return table;
}();

/**
 * @brief Precomputed bitboards for white pawn attacks (diagonal captures).
 *
 * For each square, contains a bitboard with the destination squares if a white pawn captures diagonally (NW and NE).
 * Only valid for squares on ranks 1 to 7 (0-based), as pawns on rank 8 cannot attack forward.
 */
constexpr std::array<Bitboard, Const::BOARD_SIZE> WHITE_PAWN_ATTACKS = []() constexpr {
  using namespace Const;
  using namespace Bitmasks;

  std::array<Bitboard, BOARD_SIZE> table{};

  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    uint64_t bitboard = 1ULL << sq;
    uint64_t attacks = 0;
    if (sq / BOARD_WIDTH < RANK_8_IND) {           // rank 1..7
      attacks |= (bitboard << (BOARD_WIDTH - 1));  // NW
      attacks |= (bitboard << (BOARD_WIDTH + 1));  // NE
    }
    table[sq] = Bitboard(attacks);
  }
  return table;
}();

/**
 * @brief Precomputed bitboards for black pawn attacks (diagonal captures).
 *
 * For each square, contains a bitboard with the destination squares if a black pawn captures diagonally (SW and SE).
 * Only valid for squares on ranks 2 to 8 (0-based), as pawns on rank 1 cannot attack forward.
 */
constexpr std::array<Bitboard, Const::BOARD_SIZE> BLACK_PAWN_ATTACKS = []() constexpr {
  using namespace Const;
  using namespace Bitmasks;

  std::array<Bitboard, BOARD_SIZE> table{};

  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    uint64_t bitboard = 1ULL << sq;
    uint64_t attacks = 0;
    if (sq / BOARD_WIDTH > RANK_1_IND) {           // rank 2..8
      attacks |= (bitboard >> (BOARD_WIDTH + 1));  // SW
      attacks |= (bitboard >> (BOARD_WIDTH - 1));  // SE
    }
    table[sq] = Bitboard(attacks);
  }
  return table;
}();

}  // namespace Lookups