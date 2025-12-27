#include <array>
#include <bitbishop/bitboard.hpp>
#include <bitbishop/constants.hpp>

namespace Lookups {

/**
 * @brief Computes the directional step between two aligned squares.
 *
 * Returns the signed square-index increment required to move from the
 * source square toward the destination square along a shared rank, file,
 * or diagonal.
 *
 * The returned value corresponds to the underlying square indexing:
 * - +-1  for horizontal movement (same rank)
 * - +-8  for vertical movement (same file)
 * - +-9  for NE–SW diagonal movement
 * - +-7  for NW–SE diagonal movement
 * - 0    if source and destination square are identical
 *
 * This function assumes that the two squares are aligned. Its primary use
 * is internal traversal of rays when building geometric lookup tables such
 * as BETWEEN.
 *
 * @param from The starting square.
 * @param to   The target square.
 * @return The signed step value to advance one square toward @p to.
 */
constexpr int direction(Square from, Square to) {
  if (from == to) {
    return 0;
  }
  if (from.same_rank(to)) {
    return (to.file() > from.file()) ? +1 : -1;
  }
  if (from.same_file(to)) {
    return (to.rank() > from.rank()) ? +Const::BOARD_WIDTH : -Const::BOARD_WIDTH;
  }
  if (from.same_ne_sw_diag(to)) {
    return (to.rank() > from.rank()) ? +(Const::BOARD_WIDTH + 1) : -(Const::BOARD_WIDTH + 1);
  }
  if (from.same_nw_se_diag(to)) {
    return (to.rank() > from.rank()) ? +(Const::BOARD_WIDTH - 1) : -(Const::BOARD_WIDTH - 1);
  }

  return 0;
}

/**
 * @brief Computes the bitboard of squares strictly between two aligned squares.
 *
 * Returns a bitboard containing all squares located strictly between the
 * source and destination squares, excluding both endpoints.
 *
 * If the squares are not aligned along the same rank, file, or diagonal,
 * or if both squares are identical, an empty bitboard is returned.
 *
 * This function is purely geometric and does not depend on board occupancy.
 * It is intended for use in compile-time construction of lookup tables and
 * in runtime legality checks such as pins and check interposition.
 *
 * @param from The starting square.
 * @param to   The target square.
 * @return A bitboard of squares strictly between @p from and @p to.
 */
constexpr Bitboard ray_between(Square from, Square to) {
  if (from == to) {
    return Bitboard::Zeros();
  }

  if (!(from.same_file(to) || from.same_rank(to) || from.same_diag(to))) {
    return Bitboard::Zeros();
  }

  const int step = direction(from, to);
  Bitboard ray = Bitboard::Zeros();

  for (std::uint8_t square = from.value() + step; square != to.value(); square += step) {
    ray.set(square);
  }

  return ray;
}

/**
 * @brief Precomputed lookup table of squares lying strictly between any two squares.
 *
 * For each pair of squares (from, to), this table contains a bitboard of all
 * squares strictly between them along a shared rank, file, or diagonal.
 *
 * If the squares are not aligned, the corresponding entry is an empty bitboard.
 *
 * This table is computed entirely at compile time and is used extensively
 * in move generation, including:
 * - pin detection
 * - check resolution
 * - slider attack filtering
 *
 * The table is indexed as BETWEEN[from][to].
 */
constexpr std::array<std::array<Bitboard, Const::BOARD_SIZE>, Const::BOARD_SIZE> BETWEEN = []() constexpr {
  using namespace Const;

  std::array<std::array<Bitboard, BOARD_SIZE>, BOARD_SIZE> table{};

  for (int from = 0; from < BOARD_SIZE; ++from) {
    for (int to = 0; to < BOARD_SIZE; ++to) {
      table[from][to] = ray_between(Square(from, std::in_place), Square(to, std::in_place));
    }
  }

  return table;
}();

}  // namespace Lookups
