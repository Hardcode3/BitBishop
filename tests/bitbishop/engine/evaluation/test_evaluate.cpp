#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/engine/evaluation.hpp>

using namespace Eval;
using namespace Squares;
using namespace Pieces;

TEST(TestScoreEvaluation, StartingPosWhiteHasZeroScore) {
  Board board = Board::StartingPosition();
  board.set_side_to_move(Color::WHITE);

  int score = evaluate(board);

  EXPECT_EQ(score, 0);
}

TEST(TestScoreEvaluation, StartingPosBlackHasZeroScore) {
  Board board = Board::StartingPosition();
  board.set_side_to_move(Color::BLACK);

  int score = evaluate(board);

  EXPECT_EQ(score, 0);
}

TEST(TestScoreEvaluation, EmptyBoardEvaluatesToZero) {
  Board board = Board::Empty();
  board.set_side_to_move(Color::WHITE);

  int white_score = evaluate(board);

  board.set_side_to_move(Color::BLACK);

  int black_score = evaluate(board);

  EXPECT_EQ(white_score, 0);
  EXPECT_EQ(black_score, 0);
}
