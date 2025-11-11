#include <gtest/gtest.h>

#include <bitbishop/color.hpp>
#include <format>
#include <string>

/**
 * @brief Test fixture for Color enum formatting tests.
 */
class ColorFormatterTest : public ::testing::Test {};

/**
 * @brief Tests that Color::BLACK formats to "black".
 */
TEST_F(ColorFormatterTest, FormatBlackColor) {
  Color c = Color::BLACK;
  const std::string msg = std::format("{}", c);
  EXPECT_EQ(msg, "black");
}

/**
 * @brief Tests that Color::WHITE formats to "white".
 */
TEST_F(ColorFormatterTest, FormatWhiteColor) {
  Color c = Color::WHITE;
  const std::string msg = std::format("{}", c);
  EXPECT_EQ(msg, "white");
}

/**
 * @brief Tests Color formatting in a complete sentence.
 */
TEST_F(ColorFormatterTest, FormatInSentence) {
  Color player = Color::WHITE;
  const std::string msg = std::format("Current player: {}", player);
  EXPECT_EQ(msg, "Current player: white");
}

/**
 * @brief Tests Color formatting with multiple placeholders.
 */
TEST_F(ColorFormatterTest, FormatMultipleColors) {
  Color c1 = Color::BLACK;
  Color c2 = Color::WHITE;
  const std::string msg = std::format("{} vs {}", c1, c2);
  EXPECT_EQ(msg, "black vs white");
}

/**
 * @brief Tests Color formatting with std::format_to.
 */
TEST_F(ColorFormatterTest, FormatToOutputIterator) {
  Color c = Color::BLACK;
  std::string result;
  std::format_to(std::back_inserter(result), "Color is: {}", c);
  EXPECT_EQ(result, "Color is: black");
}
