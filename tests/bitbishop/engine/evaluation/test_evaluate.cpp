#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/engine/evaluation.hpp>

using namespace Eval;
using namespace Squares;
using namespace Pieces;

TEST(TestScoreEvaluation, StartingPosWhiteHasPositiveScore) {
  Board board = Board::StartingPosition();

  int score = evaluate(board, Color::WHITE);

  EXPECT_GT(score, 0);
}

TEST(TestScoreEvaluation, StartingPosBlackHasNegativeScore) {
  Board board = Board::StartingPosition();

  int score = evaluate(board, Color::BLACK);

  EXPECT_LT(score, 0);
}

TEST(TestScoreEvaluation, EmptyBoardEvaluatesToZero) {
  Board board = Board::Empty();

  int white_score = evaluate(board, Color::WHITE);
  int black_score = evaluate(board, Color::BLACK);

  EXPECT_EQ(white_score, 0);
  EXPECT_EQ(black_score, 0);
}
