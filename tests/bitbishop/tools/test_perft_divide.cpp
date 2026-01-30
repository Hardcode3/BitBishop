#include <gtest/gtest.h>

#include <bitbishop/tools/perft.hpp>
#include <ranges>
#include <sstream>

using PerftDivideMap = std::unordered_map<std::string, std::uint64_t>;

/**
 * @brief Test helper to split output of perft divide by lines and check its result.
 */
std::vector<std::string> split_string(const std::string& input, const std::string sep) {
  std::vector<std::string> result;
  for (auto&& part : input | std::ranges::views::split(sep)) {
    result.emplace_back(part.begin(), part.end());
  }
  return result;
}

/**
 * @brief Test helper to convert a perft raw output (without other uci mentions) to an unordered map of moves, count
 */
PerftDivideMap convert_perft_divide_to_map(const std::string& input) {
  PerftDivideMap result;

  for (const auto& line : split_string(input, "\n")) {
    auto parts = split_string(line, ": ");
    if (parts.size() != 2) continue;

    const auto& move = parts[0];
    auto count = std::stoull(parts[1]);

    auto [it, inserted] = result.emplace(move, count);
    if (!inserted) {
      throw std::logic_error("Duplicate move: " + line);
    }
  }

  return result;
}

struct PerftDivideTestCase {
  std::string test_name;
  std::string fen;
  std::size_t depth;
  std::string expected_output;
};

struct PerftDivideParamName {
  template <class ParamType>
  std::string operator()(const testing::TestParamInfo<ParamType>& info) const {
    return info.param.test_name;
  }
};

class PerftDivideSmokeTest : public ::testing::TestWithParam<PerftDivideTestCase> {};

template <typename T>
void RunPerftDivideTest(const PerftDivideTestCase& param) {
  Board board(param.fen);

  std::ostringstream oss;
  auto* old = std::cout.rdbuf(oss.rdbuf());

  Tools::perft_divide(board, param.depth);

  std::cout.rdbuf(old);

  auto mine = convert_perft_divide_to_map(oss.str());
  auto expected = convert_perft_divide_to_map(param.expected_output);

  EXPECT_EQ(mine.size(), expected.size());

  for (const auto& [move, count] : expected) {
    auto it = mine.find(move);
    ASSERT_NE(it, mine.end()) << param.test_name << ": Missing move: " << move;
    EXPECT_EQ(it->second, count) << param.test_name << ": Wrong count for move " << move << ", got " << it->second
                                 << ", expected " << count;
  }
}

TEST_P(PerftDivideSmokeTest, MatchesExpected) { RunPerftDivideTest<PerftDivideSmokeTest>(GetParam()); }

static CX_VALUE const char* STARTING_POS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
static CX_VALUE const char* KIWIPETE_POS = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";

static CX_VALUE std::string_view starting_pos_depth1 = R"(
a2a3: 1
b2b3: 1
c2c3: 1
d2d3: 1
e2e3: 1
f2f3: 1
g2g3: 1
h2h3: 1
a2a4: 1
b2b4: 1
c2c4: 1
d2d4: 1
e2e4: 1
f2f4: 1
g2g4: 1
h2h4: 1
b1a3: 1
b1c3: 1
g1f3: 1
g1h3: 1

Nodes searched: 20
)";

static CX_VALUE std::string_view starting_pos_depth2 = R"(
a2a3: 20
b2b3: 20
c2c3: 20
d2d3: 20
e2e3: 20
f2f3: 20
g2g3: 20
h2h3: 20
a2a4: 20
b2b4: 20
c2c4: 20
d2d4: 20
e2e4: 20
f2f4: 20
g2g4: 20
h2h4: 20
b1a3: 20
b1c3: 20
g1f3: 20
g1h3: 20

Nodes searched: 400
)";

static CX_VALUE std::string_view kiwipete_pos_depth2 = R"(
a2a3: 44
b2b3: 42
g2g3: 42
d5d6: 41
a2a4: 44
g2g4: 42
g2h3: 43
d5e6: 46
c3b1: 42
c3d1: 42
c3a4: 42
c3b5: 39
e5d3: 43
e5c4: 42
e5g4: 44
e5c6: 41
e5g6: 42
e5d7: 45
e5f7: 44
d2c1: 43
d2e3: 43
d2f4: 43
d2g5: 42
d2h6: 41
e2d1: 44
e2f1: 44
e2d3: 42
e2c4: 41
e2b5: 39
e2a6: 36
a1b1: 43
a1c1: 43
a1d1: 43
h1f1: 43
h1g1: 43
f3d3: 42
f3e3: 43
f3g3: 43
f3h3: 43
f3f4: 43
f3g4: 43
f3f5: 45
f3h5: 43
f3f6: 39
e1d1: 43
e1f1: 43
e1g1: 43
e1c1: 43

Nodes searched: 2039
)";

// clang-format off
INSTANTIATE_TEST_SUITE_P(
    Smoke,
    PerftDivideSmokeTest,
    ::testing::Values(
        PerftDivideTestCase{"StartingPos_Depth1", STARTING_POS, 1, std::string(starting_pos_depth1)},
        PerftDivideTestCase{"StartingPos_Depth2", STARTING_POS, 2, std::string(starting_pos_depth2)},
        PerftDivideTestCase{"KiwipetePos_Depth2", KIWIPETE_POS, 2, std::string(kiwipete_pos_depth2)}
    )
);
// clang-format on
