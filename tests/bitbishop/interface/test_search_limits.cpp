#include <gtest/gtest.h>

#include <bitbishop/interface/search_worker.hpp>

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

struct SearchLimitsFromUciTestCase {
  std::string test_name;
  std::vector<std::string> command_line;
  Uci::SearchLimits expected;
};

struct SearchLimitsParamName {
  template <class ParamType>
  std::string operator()(const testing::TestParamInfo<ParamType>& info) const {
    return info.param.test_name;
  }
};

class SearchLimitsFromUciTest : public ::testing::TestWithParam<SearchLimitsFromUciTestCase> {};

TEST_P(SearchLimitsFromUciTest, ParsesCorrectly) {
  const auto& param = GetParam();
  auto result = Uci::SearchLimits::from_uci_cmd(param.command_line);

  EXPECT_EQ(result.depth, param.expected.depth);
  EXPECT_EQ(result.movetime, param.expected.movetime);
  EXPECT_EQ(result.wtime, param.expected.wtime);
  EXPECT_EQ(result.btime, param.expected.btime);
  EXPECT_EQ(result.winc, param.expected.winc);
  EXPECT_EQ(result.binc, param.expected.binc);
  EXPECT_EQ(result.infinite, param.expected.infinite);
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(
  SearchLimitsFromUci,
  SearchLimitsFromUciTest,
  ::testing::Values(
    // Empty / malformed input -> implicit infinite
    SearchLimitsFromUciTestCase{
      "EmptyCommand",
      {"go"},
      Uci::SearchLimits{
        .depth = std::nullopt,
        .movetime = std::nullopt,
        .wtime = std::nullopt,
        .btime = std::nullopt,
        .winc = std::nullopt,
        .binc = std::nullopt,
        .infinite = true
      }
    },

    // Explicit infinite
    SearchLimitsFromUciTestCase{
      "ExplicitInfinite",
      {"go", "infinite"},
      Uci::SearchLimits{
        .depth = std::nullopt,
        .movetime = std::nullopt,
        .wtime = std::nullopt,
        .btime = std::nullopt,
        .winc = std::nullopt,
        .binc = std::nullopt,
        .infinite = true
      }
    },

    // Depth only -> NOT infinite
    SearchLimitsFromUciTestCase{
      "DepthOnly",
      {"go", "depth", "10"},
      Uci::SearchLimits{
        .depth = 10,
        .movetime = std::nullopt,
        .wtime = std::nullopt,
        .btime = std::nullopt,
        .winc = std::nullopt,
        .binc = std::nullopt,
        .infinite = false
      }
    },

    // Explicit infinite overrides depth
    SearchLimitsFromUciTestCase{
      "DepthAndInfinite",
      {"go", "depth", "8", "infinite"},
      Uci::SearchLimits{
        .depth = 8,
        .movetime = std::nullopt,
        .wtime = std::nullopt,
        .btime = std::nullopt,
        .winc = std::nullopt,
        .binc = std::nullopt,
        .infinite = true
      }
    },

    // Explicit infinite overrides time controls
    SearchLimitsFromUciTestCase{
      "TimeAndInfinite",
      {"go", "wtime", "10000", "btime", "8000", "winc", "100", "binc", "200", "infinite"},
      Uci::SearchLimits{
        .depth = std::nullopt,
        .movetime = std::nullopt,
        .wtime = 10'000,
        .btime = 8'000,
        .winc = 100,
        .binc = 200,
        .infinite = true
      }
    },

    // Movetime only -> NOT infinite
    SearchLimitsFromUciTestCase{
      "MovetimeOnly",
      {"go", "movetime", "5000"},
      Uci::SearchLimits{
        .depth = std::nullopt,
        .movetime = 5'000,
        .wtime = std::nullopt,
        .btime = std::nullopt,
        .winc = std::nullopt,
        .binc = std::nullopt,
        .infinite = false
      }
    },

    // Full time controls -> NOT infinite
    SearchLimitsFromUciTestCase{
      "TimeControls",
      {"go", "wtime", "10000", "btime", "8000", "winc", "100", "binc", "200"},
      Uci::SearchLimits{
        .depth = std::nullopt,
        .movetime = std::nullopt,
        .wtime = 10000,
        .btime = 8000,
        .winc = 100,
        .binc = 200,
        .infinite = false
      }
    },

    // Mixed depth + time → depth wins -> NOT infinite
    SearchLimitsFromUciTestCase{
      "DepthAndTime",
      {"go", "depth", "12", "wtime", "10000"},
      Uci::SearchLimits{
        .depth = 12,
        .movetime = std::nullopt,
        .wtime = 10000,
        .btime = std::nullopt,
        .winc = std::nullopt,
        .binc = std::nullopt,
        .infinite = false
      }
    },

    // Unknown tokens ignored
    SearchLimitsFromUciTestCase{
      "UnknownTokensIgnored",
      {"go", "foo", "bar", "depth", "6"},
      Uci::SearchLimits{
        .depth = 6,
        .movetime = std::nullopt,
        .wtime = std::nullopt,
        .btime = std::nullopt,
        .winc = std::nullopt,
        .binc = std::nullopt,
        .infinite = false
      }
    }
  ),
  SearchLimitsParamName{}
);
// clang-format on
