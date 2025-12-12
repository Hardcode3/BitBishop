#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/square.hpp>
#include <sstream>

/**
 * @test Print function.
 * @brief Verifies that the bitboard prints as an 8×8 grid with correct layout.
 */
TEST(BitboardTest, PrintOutputsCorrectBoard) {
  Bitboard bb;
  bb.set(Square::A1);
  bb.set(Square::H8);
  bb.set(Square::E4);

  /* Expects:
  . . . . . . . 1 \n    H8
  . . . . . . . . \n
  . . . . . . . . \n
  . . . . . . . . \n
  . . . . 1 . . . \n    E4
  . . . . . . . . \n
  . . . . . . . . \n
  1 . . . . . . . \n" A1
  */

  std::ostringstream oss;
  // Redirect std::cout to oss
  std::streambuf* old_buf = std::cout.rdbuf(oss.rdbuf());
  bb.print();
  // Restore original buffer
  std::cout.rdbuf(old_buf);

  std::string output = oss.str();

  // Check key positions:
  // - H8 (top right corner) should be "1"
  EXPECT_NE(output.find(". . . . . . . 1"), std::string::npos);

  // - E4 should be in the middle
  EXPECT_NE(output.find(". . . . 1 . . ."), std::string::npos);

  // - A1 (bottom left corner) should be "1" in the last row
  EXPECT_NE(output.rfind("1 . . . . . . ."), std::string::npos);

  // Sanity check: must contain 8 lines (ranks 8 → 1)
  int line_count = 0;
  for (char c : output) {
    if (c == '\n') ++line_count;
  }
  EXPECT_EQ(line_count, 8);
}
