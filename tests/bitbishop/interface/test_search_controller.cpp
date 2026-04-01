#include <gtest/gtest.h>

#include <bitbishop/interface/search_controller.hpp>
#include <sstream>

TEST(SearchControllerTest, StartEmitsABestmoveWithDepth1) {
  Board board = Board::StartingPosition();
  Uci::SearchLimits limits;
  limits.depth = 1;
  std::ostringstream out;

  Uci::SearchWorker controller(board, limits, out);
  controller.start();
  controller.wait();

  const std::string res = out.str();
  EXPECT_GT(res.size(), 0);
  EXPECT_NE(res.find("bestmove "), std::string::npos);
}

TEST(SearchControllerTest, StartEmitsABestmoveWithInfiniteSearch) {
  Board board = Board::StartingPosition();
  Uci::SearchLimits limits;
  limits.infinite = true;
  std::ostringstream out;

  Uci::SearchWorker controller(board, limits, out);
  controller.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  controller.stop();

  const std::string res = out.str();
  EXPECT_GT(res.size(), 0);
  EXPECT_NE(res.find("bestmove "), std::string::npos);
}
