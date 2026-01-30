#pragma once

#include <array>
#include <bitbishop/bitboard.hpp>
#include <bitbishop/bitmasks.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/config.hpp>
#include <bitbishop/constants.hpp>
#include <cstdint>

namespace Lookups {

/**
 * @brief Precomputed bitboards for white pawn single pushes (1 square forward).
 *
 * For each square, contains a bitboard with the destination square if a white pawn moves one square forward.
 * Only valid for squares on ranks 1 to 7 (0-based), as pawns on rank 8 cannot move forward.
 */
CX_FN std::array<Bitboard, Const::BOARD_SIZE> WHITE_PAWN_SINGLE_PUSH = []() CX_EXEC {
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
CX_FN std::array<Bitboard, Const::BOARD_SIZE> BLACK_PAWN_SINGLE_PUSH = []() CX_EXEC {
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
CX_FN std::array<Bitboard, Const::BOARD_SIZE> WHITE_PAWN_DOUBLE_PUSH = []() CX_EXEC {
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
CX_FN std::array<Bitboard, Const::BOARD_SIZE> BLACK_PAWN_DOUBLE_PUSH = []() CX_EXEC {
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
CX_FN std::array<Bitboard, Const::BOARD_SIZE> WHITE_PAWN_ATTACKS = []() CX_EXEC {
  using namespace Const;
  using namespace Bitmasks;

  std::array<Bitboard, BOARD_SIZE> table{};

  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    uint64_t bitboard = 1ULL << sq;
    uint64_t attacks = 0;

    const int rank = sq / BOARD_WIDTH;
    const int file = sq % BOARD_WIDTH;

    if (rank < RANK_8_IND) {
      if (file > FILE_A_IND) {
        attacks |= (bitboard << (BOARD_WIDTH - 1));  // NW
      }
      if (file < FILE_H_IND) {
        attacks |= (bitboard << (BOARD_WIDTH + 1));  // NE
      }
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
CX_FN std::array<Bitboard, Const::BOARD_SIZE> BLACK_PAWN_ATTACKS = []() CX_EXEC {
  using namespace Const;

  std::array<Bitboard, BOARD_SIZE> table{};

  for (int sq = 0; sq < BOARD_SIZE; ++sq) {
    uint64_t bb = 1ULL << sq;
    uint64_t attacks = 0;

    const int rank = sq / BOARD_WIDTH;
    const int file = sq % BOARD_WIDTH;

    if (rank > RANK_1_IND) {
      if (file > FILE_A_IND) {
        attacks |= bb >> (BOARD_WIDTH + 1);  // SW
      }
      if (file < FILE_H_IND) {
        attacks |= bb >> (BOARD_WIDTH - 1);  // SE
      }
    }

    table[sq] = Bitboard(attacks);
  }

  return table;
}();

/**
 * @brief Precomputed bitboards of WHITE pawn attackers (reverse pawn attacks).
 *
 * For each target square, this table contains a bitboard of all squares
 * from which a WHITE pawn could attack that square.
 *
 * Important notes about pawn attack geometry:
 * - Pawn attacks are directional and asymmetric
 * - Pawn attackers are NOT equal to pawn attacks from the same square
 * - White pawn attackers are equivalent to black pawn attacks
 *   when viewed from the target square
 *
 * This table is independent of board occupancy and contains only
 * pure geometric information.
 *
 * Indexed by target square (0–63).
 */
CX_FN std::array<Bitboard, Const::BOARD_SIZE> WHITE_PAWN_ATTACKERS = BLACK_PAWN_ATTACKS;

/**
 * @brief Precomputed bitboards of BLACK pawn attackers (reverse pawn attacks).
 *
 * For each target square, this table contains a bitboard of all squares
 * from which a BLACK pawn could attack that square.
 *
 * Important notes about pawn attack geometry:
 * - Pawn attacks are directional and asymmetric
 * - Pawn attackers are NOT equal to pawn attacks from the same square
 * - Black pawn attackers are equivalent to white pawn attacks
 *   when viewed from the target square
 *
 * This table is independent of board occupancy and contains only
 * pure geometric information.
 *
 * Indexed by target square (0–63).
 */
CX_FN std::array<Bitboard, Const::BOARD_SIZE> BLACK_PAWN_ATTACKERS = WHITE_PAWN_ATTACKS;

/**
 * @brief Precomputed single-push destinations for pawns of both colors.
 *
 * For each color and source square, contains a bitboard with the destination
 * square if a pawn moves one square forward:
 * - White pawns move north (+1 rank)
 * - Black pawns move south (-1 rank)
 *
 * This table represents theoretical move destinations only.
 * The caller must ensure:
 * - The destination square is empty
 * - The move satisfies check and pin constraints
 *
 * Indexed as [color][source_square].
 */
CX_FN std::array<std::array<Bitboard, Const::BOARD_SIZE>, ColorUtil::SIZE> PAWN_SINGLE_PUSH = []() CX_EXEC {
  using namespace Const;

  std::array<std::array<Bitboard, BOARD_SIZE>, ColorUtil::SIZE> table{};

  for (Color color : ColorUtil::ALL) {
    const auto idx = ColorUtil::to_index(color);
    switch (color) {
      case Color::WHITE:
        table[idx] = WHITE_PAWN_SINGLE_PUSH;
        break;
      case Color::BLACK:
        table[idx] = BLACK_PAWN_SINGLE_PUSH;
        break;
    }
  }

  return table;
}();

/**
 * @brief Precomputed double-push destinations for pawns of both colors.
 *
 * For each color and source square, contains a bitboard with the destination
 * square if a pawn moves two squares forward from its starting rank:
 * - White: rank 2 → rank 4
 * - Black: rank 7 → rank 5
 *
 * This table represents theoretical move destinations only.
 * The caller must ensure:
 * - The pawn is on its starting rank
 * - Both the intermediate and destination squares are empty
 * - The move satisfies check and pin constraints
 *
 * Indexed as [color][source_square].
 */
CX_FN std::array<std::array<Bitboard, Const::BOARD_SIZE>, ColorUtil::SIZE> PAWN_DOUBLE_PUSH = []() CX_EXEC {
  using namespace Const;

  std::array<std::array<Bitboard, BOARD_SIZE>, ColorUtil::SIZE> table{};

  for (Color color : ColorUtil::ALL) {
    const auto idx = ColorUtil::to_index(color);
    switch (color) {
      case Color::WHITE:
        table[idx] = WHITE_PAWN_DOUBLE_PUSH;
        break;
      case Color::BLACK:
        table[idx] = BLACK_PAWN_DOUBLE_PUSH;
        break;
    }
  }

  return table;
}();

/**
 * @brief Precomputed diagonal capture targets for pawns of both colors.
 *
 * For each color and source square, contains a bitboard with all squares
 * a pawn could capture to diagonally:
 * - White: north-west and north-east
 * - Black: south-west and south-east
 *
 * This table is independent of board occupancy.
 * The caller must verify that:
 * - An enemy piece occupies the target square, or
 * - The square corresponds to a valid en passant target
 *
 * Indexed as [color][source_square].
 */
CX_FN std::array<std::array<Bitboard, Const::BOARD_SIZE>, ColorUtil::SIZE> PAWN_ATTACKS = []() CX_EXEC {
  using namespace Const;

  std::array<std::array<Bitboard, BOARD_SIZE>, ColorUtil::SIZE> table{};

  for (Color color : ColorUtil::ALL) {
    const auto idx = ColorUtil::to_index(color);
    switch (color) {
      case Color::WHITE:
        table[idx] = WHITE_PAWN_ATTACKS;
        break;
      case Color::BLACK:
        table[idx] = BLACK_PAWN_ATTACKS;
        break;
    }
  }

  return table;
}();

}  // namespace Lookups
