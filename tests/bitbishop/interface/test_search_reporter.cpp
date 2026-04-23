#include <gtest/gtest.h>

#include <bitbishop/interface/search_reporter.hpp>
#include <sstream>
#include <thread>

using namespace Search;

class SearchReporterTest : public ::testing::Test {
 protected:
  Move fake_move = Move::from_uci("e2e4");

  BestMove best_move{.move = fake_move};
  BestMove empty_move{.move = std::nullopt};

  SearchStats stats{.negamax_nodes = 120, .quiescence_nodes = 80};

  std::ostringstream out;
};

TEST_F(SearchReporterTest, UciOutputsBestMoveWhenPresent) {
  UciReporter reporter(out);

  reporter.on_finish(best_move, stats);

  EXPECT_EQ(out.str(), "bestmove " + best_move.move->to_uci() + "\n");
}

TEST_F(SearchReporterTest, UciOutputsNullMoveWhenMissing) {
  UciReporter reporter(out);

  reporter.on_finish(empty_move, stats);

  EXPECT_EQ(out.str(), "bestmove 0000\n");
}

TEST_F(SearchReporterTest, BenchOutputsCorrectNodeAggregation) {
  BenchReporter reporter(out);

  reporter.on_finish(best_move, stats);

  const std::string result = out.str();

  const uint64_t total_nodes = stats.negamax_nodes + stats.quiescence_nodes;

  EXPECT_NE(result.find("bench nodes " + std::to_string(total_nodes)), std::string::npos);

  EXPECT_NE(result.find("negamax_nodes " + std::to_string(stats.negamax_nodes)), std::string::npos);

  EXPECT_NE(result.find("quiescence_nodes " + std::to_string(stats.quiescence_nodes)), std::string::npos);

  EXPECT_NE(result.find("time(s)"), std::string::npos);
  EXPECT_NE(result.find("nps"), std::string::npos);
}

TEST_F(SearchReporterTest, BenchHandlesZeroTimeGracefully) {
  const auto start_time = std::chrono::steady_clock::now();

  BenchReporter reporter(out, [start_time]() { return start_time; });
  reporter.on_finish(best_move, stats);

  const std::string result = out.str();

  EXPECT_NE(result.find("bench nodes "), std::string::npos);
  EXPECT_NE(result.find("nps "), std::string::npos);
}
