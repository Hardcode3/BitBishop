#include <gtest/gtest.h>

#include <bitbishop/helpers/async.hpp>
#include <bitbishop/interface/search_session.hpp>
#include <chrono>
#include <functional>
#include <sstream>
#include <thread>

using namespace std::chrono_literals;

bool wait_until_idle(Uci::SearchSession& session, std::chrono::milliseconds timeout = 1000ms) {
  return wait_for(
      [&session] {
        session.poll();
        return session.is_idle();
      },
      timeout);
}

TEST(SearchSessionTest, StartsIdleAndPollIsNoopWhenNoSearchIsRunning) {
  std::stringstream output;
  Uci::SearchSession session(output);

  EXPECT_TRUE(session.is_idle());
  session.poll();
  EXPECT_TRUE(session.is_idle());
  EXPECT_TRUE(output.str().empty());
}

TEST(SearchSessionTest, StartGoDepthOneProducesBestMoveAndBecomesIdle) {
  std::stringstream output;
  Uci::SearchSession session(output);

  Uci::SearchLimits limits{.depth = 1};

  session.start_go(Board::StartingPosition(), limits);

  ASSERT_TRUE(wait_until_idle(session));

  const std::string result = output.str();
  EXPECT_NE(result.find("bestmove "), std::string::npos);
}

TEST(SearchSessionTest, StartBenchDepthOneProducesBenchSummaryAndBecomesIdle) {
  std::stringstream output;
  Uci::SearchSession session(output);

  Uci::SearchLimits limits{.depth = 1};

  session.start_bench(Board::StartingPosition(), limits);

  ASSERT_TRUE(wait_until_idle(session));

  const std::string result = output.str();
  EXPECT_NE(result.find("bench nodes "), std::string::npos);
  EXPECT_NE(result.find("negamax_nodes "), std::string::npos);
  EXPECT_NE(result.find("quiescence_nodes "), std::string::npos);
  EXPECT_NE(result.find("nps "), std::string::npos);
}

TEST(SearchSessionTest, RequestStopInterruptsInfiniteSearchAndStillFinalizes) {
  std::stringstream output;
  Uci::SearchSession session(output);

  Uci::SearchLimits limits{.infinite = true};

  session.start_go(Board::StartingPosition(), limits);
  std::this_thread::sleep_for(20ms);
  session.request_stop();

  ASSERT_TRUE(wait_until_idle(session, 1500ms));
  EXPECT_NE(output.str().find("bestmove "), std::string::npos);
}

TEST(SearchSessionTest, StartBenchWithInfiniteLimitCompletesWithoutManualStop) {
  std::stringstream output;
  Uci::SearchSession session(output);

  // Stalemate position keeps the converted fixed-depth bench path very fast.
  Board board("K7/8/8/8/8/8/5Q2/7k b - - 0 1");

  Uci::SearchLimits limits{.infinite = true};

  session.start_bench(board, limits);

  ASSERT_TRUE(wait_until_idle(session, 1500ms));
  EXPECT_NE(output.str().find("bench nodes "), std::string::npos);
}
