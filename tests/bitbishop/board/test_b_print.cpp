#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>
#include <sstream>

/**
 * @test BoardTest.PrintBoard
 * @brief Verifies that the board prints to std::cout correctly when called from a starting position.
 *
 * @note Uses operator<< that already test for formatting correctness.
 */
TEST(BoardTest, PrintBoardStartingPos) {
  Board board = Board::StartingPosition();

  std::ostringstream oss;
  // Redirect std::cout to oss
  std::streambuf* old_buf = std::cout.rdbuf(oss.rdbuf());
  board.print();
  // Restore original buffer
  std::cout.rdbuf(old_buf);
  std::string output = oss.str();

  std::string expected =
      "     a   b   c   d   e   f   g   h  \n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 8 | r | n | b | q | k | b | n | r | 8\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 7 | p | p | p | p | p | p | p | p | 7\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 6 |   |   |   |   |   |   |   |   | 6\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 5 |   |   |   |   |   |   |   |   | 5\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 4 |   |   |   |   |   |   |   |   | 4\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 3 |   |   |   |   |   |   |   |   | 3\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 2 | P | P | P | P | P | P | P | P | 2\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 1 | R | N | B | Q | K | B | N | R | 1\n"
      "   +---+---+---+---+---+---+---+---+\n"
      "     a   b   c   d   e   f   g   h  \n";

  EXPECT_EQ(output, expected);
}