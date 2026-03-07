#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/engine/evaluation.hpp>

using namespace Eval;
using namespace Squares;
using namespace Pieces;

TEST(PSqTEvaluationTest, StartingPosWhitesHaveNonZeroEvaluation) {
  Board board = Board::StartingPosition();

  int score = evaluate_psqt(board, Color::WHITE);

  EXPECT_NE(score, 0);
}

TEST(PSqTEvaluationTest, StartingPosBlacksHaveNonZeroEvaluation) {
  Board board = Board::StartingPosition();

  int score = evaluate_psqt(board, Color::BLACK);

  EXPECT_NE(score, 0);
}

TEST(PSqTEvaluationTest, EmptyBoardEvaluatesToZero) {
  Board board = Board::Empty();

  int white_score = evaluate_psqt(board, Color::WHITE);
  int black_score = evaluate_psqt(board, Color::BLACK);

  EXPECT_EQ(white_score, 0);
  EXPECT_EQ(black_score, 0);
}
