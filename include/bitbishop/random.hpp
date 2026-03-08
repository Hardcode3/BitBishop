#pragma once

#include <bitbishop/config.hpp>
#include <cstdint>

namespace Random {
/**
 * @brief Generates a pseudo-random 64-bit integer using the SplitMix64 algorithm.
 *
 * @param seed Reference to the generator state. Modified in-place on each call;
 *             pass the same variable across successive calls to advance the sequence.
 *
 * @return A pseudo-random @c uint64_t derived from the updated seed.
 *
 * @note The output is deterministic: identical seeds produce identical sequences.
 * @note Not cryptographically secure.
 *
 * @see https://rosettacode.org/wiki/Pseudo-random_numbers/Splitmix64
 * @see https://prng.di.unimi.it/splitmix64.c
 *
 * @par Example
 * @code
 *   uint64_t seed = 0xdd40af6e35fd248ULL;
 *   uint64_t k1 = Random::splitmix64(seed);
 *   uint64_t k2 = Random::splitmix64(seed); // next value in sequence
 * @endcode
 */
[[nodiscard]] CX_FN uint64_t splitmix64(uint64_t& seed) noexcept {
  uint64_t z = (seed += 0x9E3779B97F4A7C15ULL);
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
  return z ^ (z >> 31);
}

}  // namespace Random
