#include <gtest/gtest.h>

#include <bitbishop/interface/search_controller.hpp>

TEST(SearchLimitsTest, DefaultsAreEmptyAndNotInfinite) {
  Uci::SearchLimits limits;

  EXPECT_FALSE(limits.depth.has_value());
  EXPECT_FALSE(limits.movetime.has_value());
  EXPECT_FALSE(limits.wtime.has_value());
  EXPECT_FALSE(limits.btime.has_value());
  EXPECT_FALSE(limits.winc.has_value());
  EXPECT_FALSE(limits.binc.has_value());
  EXPECT_FALSE(limits.infinite);
}
