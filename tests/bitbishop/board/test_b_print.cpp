#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>
#include <sstream>

/**
 * @test BoardTest.PrintBoard
 * @brief Verifies that the board prints in correct ASCII format.
 */
TEST(BoardTest, PrintBoard) {
  Board board;

  std::ostringstream oss;
  // Redirect std::cout to oss
  std::streambuf* old_buf = std::cout.rdbuf(oss.rdbuf());
  board.print();
  // Restore original buffer
  std::cout.rdbuf(old_buf);
  std::string output = oss.str();

  std::string expected =
      "8 r n b q k b n r \n"
      "7 p p p p p p p p \n"
      "6 . . . . . . . . \n"
      "5 . . . . . . . . \n"
      "4 . . . . . . . . \n"
      "3 . . . . . . . . \n"
      "2 P P P P P P P P \n"
      "1 R N B Q K B N R \n"
      "  a b c d e f g h\n";

  EXPECT_EQ(output, expected);
}