#include <gtest/gtest.h>

#include <bitbishop/interface/search_controller.hpp>
#include <sstream>

TEST(SearchControllerTest, StartEmitsBestmove) {
  Board board = Board::StartingPosition();
  Position position(board);
  std::stringstream out;

  {
    Uci::SearchController controller;
    Uci::SearchLimits limits;
    limits.depth = 1;
    controller.start(position, limits, out);
  }

  const std::string res = out.str();
  EXPECT_NE(res.find("bestmove "), std::string::npos);
}
