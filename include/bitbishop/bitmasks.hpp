#pragma once
#include <cstdint>

/**
 * @namespace Bitmasks
 * @brief Defines precomputed bitmasks that can be applied during move generation.
 *
 * @example How are these file bitmasks defined?
 *
 * Take file C for instance.
 * Each group of 8 bits defines a row.
 * The third bit corresponds to file C.
 *
 * In binary representation:
 *      0000 0100 0000 0100 ... 0000 0100 0000 0100
 *
 * In hexadecimal representation (0x):
 *      0    4    0    4    ... 0    4    0    4
 *
 * Aka. FILE_C = 0x0404040404040404
 *
 * The suffix ULL (Unsigned Long Long is added to ensure the compiler interprets the
 * hexadecimal value as an unsigned integer of length 64 bits).
 *
 * @example How are these rank bitmasks defined?
 *
 * Take rank 1 for instance.
 * Each group of 8 bits defines a row.
 * The first (leftmost) 8 bits correspond to rank 1:
 * 0000 0000 0000 0000 ... 0000 0000 1111 1111
 *
 * 1111 = 2^3 + 2^2 + 2^1 + 2^0 = 15 = 0xF
 *
 * Hence: 0000 0000 0000 0000 ... 0000 0000 1111 1111 = 0x00000000000000FF
 */
namespace Bitmasks {

/**
 * uint64_t mask with ones on file A.
 */
constexpr uint64_t FILE_A = 0x0101010101010101ULL;

/**
 * uint64_t mask with ones on file B.
 */
constexpr uint64_t FILE_B = 0x0202020202020202ULL;

/**
 * uint64_t mask with ones on file C.
 */
constexpr uint64_t FILE_C = 0x0404040404040404ULL;

/**
 * uint64_t mask with ones on file D.
 */
constexpr uint64_t FILE_D = 0x0808080808080808ULL;

/**
 * uint64_t mask with ones on file E.
 */
constexpr uint64_t FILE_E = 0x1010101010101010ULL;

/**
 * uint64_t mask with ones on file F.
 */
constexpr uint64_t FILE_F = 0x2020202020202020ULL;

/**
 * uint64_t mask with ones on file G.
 */
constexpr uint64_t FILE_G = 0x4040404040404040ULL;

/**
 * uint64_t mask with ones on file H.
 */
constexpr uint64_t FILE_H = 0x8080808080808080ULL;

/**
 * uint64_t mask with ones on rank 1.
 */
constexpr uint64_t RANK_1 = 0x00000000000000FFULL;

/**
 * uint64_t mask with ones on rank 2.
 */
constexpr uint64_t RANK_2 = 0x000000000000FF00ULL;

/**
 * uint64_t mask with ones on rank 3.
 */
constexpr uint64_t RANK_3 = 0x0000000000FF0000ULL;

/**
 * uint64_t mask with ones on rank 4.
 */
constexpr uint64_t RANK_4 = 0x00000000FF000000ULL;

/**
 * uint64_t mask with ones on rank 5.
 */
constexpr uint64_t RANK_5 = 0x000000FF00000000ULL;

/**
 * uint64_t mask with ones on rank 6.
 */
constexpr uint64_t RANK_6 = 0x0000FF0000000000ULL;

/**
 * uint64_t mask with ones on rank 7.
 */
constexpr uint64_t RANK_7 = 0x00FF000000000000ULL;

/**
 * uint64_t mask with ones on rank 8.
 */
constexpr uint64_t RANK_8 = 0xFF00000000000000ULL;

}  // namespace Bitmasks
