#include <gtest/gtest.h>

#include <algorithm>
#include <bitbishop/interface/search_worker.hpp>
#include <chrono>
#include <thread>

TEST(SearchControllerTest, StartPublishesFinishReportWithDepth1) {
  Board board = Board::StartingPosition();
  Uci::SearchLimits limits;
  limits.depth = 1;

  Uci::SearchWorker controller(board, limits);
  controller.start();
  controller.wait();

  const auto reports = controller.drain_reports();
  ASSERT_FALSE(reports.empty());
  EXPECT_EQ(reports.back().kind, Uci::SearchReportKind::Finish);
  EXPECT_TRUE(reports.back().best.move.has_value());
  EXPECT_TRUE(std::any_of(reports.begin(), reports.end(), [](const Uci::SearchReport& report) {
    return report.kind == Uci::SearchReportKind::Iteration;
  }));
}

TEST(SearchControllerTest, StartPublishesFinishReportWithInfiniteSearch) {
  Board board = Board::StartingPosition();
  Uci::SearchLimits limits;
  limits.infinite = true;

  Uci::SearchWorker controller(board, limits);
  controller.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  controller.stop();

  const auto reports = controller.drain_reports();
  ASSERT_FALSE(reports.empty());
  EXPECT_EQ(reports.back().kind, Uci::SearchReportKind::Finish);
  EXPECT_TRUE(reports.back().best.move.has_value());
  EXPECT_TRUE(std::any_of(reports.begin(), reports.end(), [](const Uci::SearchReport& report) {
    return report.kind == Uci::SearchReportKind::Iteration;
  }));
}

TEST(SearchControllerTest, MovetimeStopsSearchAutomatically) {
  Board board = Board::StartingPosition();
  Uci::SearchLimits limits;
  limits.movetime = 50;

  Uci::SearchWorker controller(board, limits);
  const auto start = std::chrono::steady_clock::now();
  controller.start();
  controller.wait();
  const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);

  const auto reports = controller.drain_reports();
  ASSERT_FALSE(reports.empty());
  EXPECT_EQ(reports.back().kind, Uci::SearchReportKind::Finish);
  EXPECT_TRUE(reports.back().best.move.has_value());
  EXPECT_GE(elapsed.count(), *limits.movetime);
  EXPECT_LT(elapsed.count(), *limits.movetime * 4);  // very permissive for slow ci builds and tests
  EXPECT_TRUE(std::any_of(reports.begin(), reports.end(), [](const Uci::SearchReport& report) {
    return report.kind == Uci::SearchReportKind::Iteration;
  }));
}
