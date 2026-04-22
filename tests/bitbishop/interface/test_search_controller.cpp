#include <gtest/gtest.h>

#include <bitbishop/interface/search_controller.hpp>
#include <algorithm>
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
  EXPECT_TRUE(std::any_of(reports.begin(), reports.end(),
                          [](const Uci::SearchReport& report) { return report.kind == Uci::SearchReportKind::Iteration; }));
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
  EXPECT_TRUE(std::any_of(reports.begin(), reports.end(),
                          [](const Uci::SearchReport& report) { return report.kind == Uci::SearchReportKind::Iteration; }));
}
