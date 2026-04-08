#include <gtest/gtest.h>

#include <bitbishop/board.hpp>

TEST(BoardTestOStreamOverload, EmptyBoardProvidesCorrectFormatting) {
  std::ostringstream oss;

  Board board = Board::Empty();

  oss << board;

  std::string expected =
      "     a   b   c   d   e   f   g   h  \n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 8 |   |   |   |   |   |   |   |   | 8\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 7 |   |   |   |   |   |   |   |   | 7\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 6 |   |   |   |   |   |   |   |   | 6\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 5 |   |   |   |   |   |   |   |   | 5\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 4 |   |   |   |   |   |   |   |   | 4\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 3 |   |   |   |   |   |   |   |   | 3\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 2 |   |   |   |   |   |   |   |   | 2\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 1 |   |   |   |   |   |   |   |   | 1\n"
      "   +---+---+---+---+---+---+---+---+\n"
      "     a   b   c   d   e   f   g   h  \n";

  ASSERT_EQ(oss.str(), expected);
}

TEST(BoardTestOStreamOverload, StartingPositionBoardProvidesCorrectFormatting) {
  std::ostringstream oss;

  Board board = Board::StartingPosition();

  oss << board;

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

  ASSERT_EQ(oss.str(), expected);
}

TEST(BoardTestOStreamOverload, RandomPositionBoardProvidesCorrectFormatting) {
  std::ostringstream oss;

  Board board("rk2n1qr/ppppp1pp/1bn2b2/2NQp2P/8/2B4B/PPPPPP1P/RK1N3R b - - 0 1");

  oss << board;

  std::string expected =
      "     a   b   c   d   e   f   g   h  \n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 8 | r | k |   |   | n |   | q | r | 8\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 7 | p | p | p | p | p |   | p | p | 7\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 6 |   | b | n |   |   | b |   |   | 6\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 5 |   |   | N | Q | p |   |   | P | 5\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 4 |   |   |   |   |   |   |   |   | 4\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 3 |   |   | B |   |   |   |   | B | 3\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 2 | P | P | P | P | P | P |   | P | 2\n"
      "   +---+---+---+---+---+---+---+---+\n"
      " 1 | R | K |   | N |   |   |   | R | 1\n"
      "   +---+---+---+---+---+---+---+---+\n"
      "     a   b   c   d   e   f   g   h  \n";

  ASSERT_EQ(oss.str(), expected);
}
