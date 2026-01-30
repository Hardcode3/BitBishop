#pragma once

#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/config.hpp>
#include <bitbishop/lookups/bishop_rays.hpp>
#include <bitbishop/lookups/king_attacks.hpp>
#include <bitbishop/lookups/knight_attacks.hpp>
#include <bitbishop/lookups/pawn_attacks.hpp>
#include <bitbishop/lookups/queen_rays.hpp>
#include <bitbishop/lookups/rook_rays.hpp>
#include <bitbishop/square.hpp>

/**
 * @brief Computes the geometric attackers to a target square for a given color.
 *
 * Returns a bitboard containing all squares from which a piece of the given
 * color could attack the target square, assuming an empty board for sliding
 * pieces.
 *
 * This function performs a purely geometric attack computation:
 * - King, knight, and pawn attacks are exact.
 * - Rook, bishop, and queen attacks are based on precomputed rays and do not
 *   account for blockers.
 *
 * The resulting bitboard therefore represents *potential* attackers and must
 * be filtered against board occupancy to determine actual attacks.
 *
 * This function is intended for use in:
 * - check detection
 * - pin detection
 * - attack-based legality filtering
 *
 * @param target The square being attacked.
 * @param color  The color of the attacking side.
 * @return A bitboard of squares that could attack @p target.
 *
 * @note This function does not consider board occupancy.
 * @note The returned bitboard may include squares that are blocked in the
 *       current position.
 */
CX_FN Bitboard attackers_to(Square target, Color color) {
  using namespace Lookups;
  const int square_index = target.value();

  Bitboard attackers;
  attackers |= KING_ATTACKERS[square_index];
  attackers |= KNIGHT_ATTACKERS[square_index];

  if (color == Color::WHITE) {
    attackers |= WHITE_PAWN_ATTACKERS[square_index];
  } else {
    attackers |= BLACK_PAWN_ATTACKERS[square_index];
  }

  attackers |= ROOK_ATTACKER_RAYS[square_index];
  attackers |= BISHOP_ATTACKER_RAYS[square_index];
  attackers |= QUEEN_ATTACKER_RAYS[square_index];

  return attackers;
}

/**
 * @brief Precomputed lookup table of geometric attackers for all squares and colors.
 *
 * For each color and target square, this table contains a bitboard of all
 * squares from which a piece of that color could potentially attack the target
 * square, ignoring board occupancy.
 *
 * The table is indexed as:
 * @code
 * ATTACKERS_TO[color_index][square_index]
 * @endcode
 *
 * This table is computed entirely at compile time and serves as a fast,
 * constant-time replacement for repeated calls to attackers_to().
 *
 * Typical use cases include:
 * - fast king-in-check detection
 * - identifying candidate checking pieces
 * - pin and x-ray attack analysis
 *
 * @note Sliding piece attacks in this table are unblocked rays and must be
 *       masked with board occupancy when determining actual attacks.
 */
CX_INLINE std::array<std::array<Bitboard, Const::BOARD_SIZE>, ColorUtil::SIZE> ATTACKERS_TO = []() CX_EXEC {
  using namespace Const;

  std::array<std::array<Bitboard, BOARD_SIZE>, ColorUtil::SIZE> table{};

  for (Color col : ColorUtil::ALL) {
    const std::size_t coli = ColorUtil::to_index(col);
    for (int sq = 0; sq < BOARD_SIZE; ++sq) {
      table[coli][sq] = Bitboard(attackers_to(Square(sq, std::in_place), col));
    }
  }

  return table;
}();
