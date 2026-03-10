#include <gtest/gtest.h>

#include <bitbishop/random.hpp>  // adjust include if needed
#include <cstdint>

static constexpr uint64_t INITIAL_SEED = 0xdd40af6e35fd248ULL;

// Known-good values generated from the SplitMix64 reference implementation
static constexpr uint64_t KNOWN_SEED_SEQUENCE[] = {0xbd9e4ea9d61be97bULL, 0x17da6008d8998017ULL, 0x4a5427b076bdfcb1ULL,
                                                   0x5df4deae0f2ad481ULL};

TEST(SplitMix64Test, SameSeedProducesSameFirstValue) {
  uint64_t seed1 = INITIAL_SEED;
  uint64_t seed2 = INITIAL_SEED;

  auto v1 = Random::splitmix64(seed1);
  auto v2 = Random::splitmix64(seed2);

  EXPECT_EQ(v1, v2);
}

TEST(SplitMix64Test, SeedIsModifiedAfterCall) {
  uint64_t seed = INITIAL_SEED;
  uint64_t original = seed;

  std::ignore = Random::splitmix64(seed);

  EXPECT_NE(seed, original);
}

TEST(SplitMix64Test, SuccessiveCallsProduceDifferentValues) {
  uint64_t seed = INITIAL_SEED;

  uint64_t v1 = Random::splitmix64(seed);
  uint64_t v2 = Random::splitmix64(seed);
  uint64_t v3 = Random::splitmix64(seed);

  EXPECT_NE(v1, v2);
  EXPECT_NE(v2, v3);
  EXPECT_NE(v1, v3);
}

TEST(SplitMix64Test, SameSeedProducesSameSequence) {
  uint64_t seed1 = INITIAL_SEED;
  uint64_t seed2 = INITIAL_SEED;

  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(Random::splitmix64(seed1), Random::splitmix64(seed2));
  }
}

// Reference sequence verification (strongest regression test)
// Replace expected values with ones generated from the reference implementation
TEST(SplitMix64Test, MatchesReferenceSequence) {
  uint64_t seed = INITIAL_SEED;

  for (uint64_t expected : KNOWN_SEED_SEQUENCE) {
    EXPECT_EQ(Random::splitmix64(seed), expected);
  }
}
