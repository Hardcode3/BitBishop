#include <gtest/gtest.h>

#include <bitbishop/interface/uci_engine.hpp>

TEST(UciSplitTest, BasicTokens) {
  const auto tokens = Uci::split("position startpos moves e2e4 e7e5");

  ASSERT_EQ(tokens.size(), 5u);
  EXPECT_EQ(tokens[0], "position");
  EXPECT_EQ(tokens[1], "startpos");
  EXPECT_EQ(tokens[2], "moves");
  EXPECT_EQ(tokens[3], "e2e4");
  EXPECT_EQ(tokens[4], "e7e5");
}

TEST(UciSplitTest, CollapsesWhitespace) {
  const auto tokens = Uci::split("  go\t  depth   3  ");

  ASSERT_EQ(tokens.size(), 3u);
  EXPECT_EQ(tokens[0], "go");
  EXPECT_EQ(tokens[1], "depth");
  EXPECT_EQ(tokens[2], "3");
}
