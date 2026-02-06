#include <gtest/gtest.h>

#include <bitbishop/engine/evaluation.hpp>
#include <bitbishop/engine/search.hpp>
#include <tuple>

using namespace Search;
using namespace Pieces;
using namespace Squares;

TEST(NegaMaxTest, EmptyBoardThrows) {
  Board board = Board::Empty();

  EXPECT_THROW(std::ignore = negamax(board, 1, ALPHA_INIT, BETA_INIT, 0), std::bad_optional_access);
}

TEST(NegaMaxTest, EmptyBoardWithOnlyWhiteKingThrows) {
  Board board = Board::Empty();
  board.set_piece(E5, WHITE_KING);
  board.set_side_to_move(Color::WHITE);

  EXPECT_THROW(std::ignore = negamax(board, 1, ALPHA_INIT, BETA_INIT, 0), std::bad_optional_access);
}

TEST(NegaMaxTest, EmptyBoardWithOnlyBlackKingThrows) {
  Board board = Board::Empty();
  board.set_piece(E5, BLACK_KING);
  board.set_side_to_move(Color::BLACK);

  EXPECT_THROW(std::ignore = negamax(board, 1, ALPHA_INIT, BETA_INIT, 0), std::bad_optional_access);
}

TEST(NegaMaxTest, EmptyBoardWithBothKingsDontThrow) {
  Board board = Board::Empty();
  board.set_piece(E5, WHITE_KING);
  board.set_piece(H8, BLACK_KING);

  board.set_side_to_move(Color::WHITE);

  BestMove best = negamax(board, 1, ALPHA_INIT, BETA_INIT, 0);
}

TEST(NegaMaxTest, FindsMateInOne) {
  // White to move, Queen can take on f7 for mate
  Board board = Board("r1bqkbnr/pppp1ppp/2n5/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR w KQkq - 0 1");

  BestMove best = negamax(board, 2, ALPHA_INIT, BETA_INIT, 0);

  EXPECT_GT(best.score, Eval::MATE_THRESHOLD);
  EXPECT_TRUE(best.move.has_value());
  EXPECT_EQ(best.move->to, F7);
}
