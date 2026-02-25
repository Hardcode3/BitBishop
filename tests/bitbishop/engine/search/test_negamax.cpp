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

  EXPECT_NO_THROW(std::ignore = negamax(board, 1, ALPHA_INIT, BETA_INIT, 0));
}

TEST(NegaMaxTest, FindsScolarsMateInOne) {
  // White to move, Queen can take on f7 for mate
  Board board = Board("r1bqkbnr/pppp1ppp/2n5/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR w KQkq - 0 1");

  BestMove best = negamax(board, 2, ALPHA_INIT, BETA_INIT, 0);

  EXPECT_GT(best.score, Eval::MATE_THRESHOLD);
  EXPECT_TRUE(best.move.has_value());
  EXPECT_EQ(best.move->from, F3);
  EXPECT_EQ(best.move->to, F7);
}

TEST(NegaMaxTest, FindsCornerMateInOne) {
  Board board("7k/5K2/6Q1/8/8/8/8/8 w - - 0 1");

  BestMove best = negamax(board, 2, ALPHA_INIT, BETA_INIT, 0);

  EXPECT_GT(best.score, Eval::MATE_THRESHOLD);
  EXPECT_TRUE(best.move.has_value());
  EXPECT_EQ(best.move->from, G6);
  EXPECT_EQ(best.move->to, H5);
}

TEST(NegaMaxTest, FindsStaleMateByWhiteQueenInCorner) {
  Board board("K7/8/8/8/8/8/5Q2/7k b - - 0 1");

  BestMove best = negamax(board, 2, ALPHA_INIT, BETA_INIT, 0);

  EXPECT_EQ(best.score, 0);
  EXPECT_FALSE(best.move.has_value());
}

TEST(NegaMaxTest, FindsStaleMateByWhiteAllBlackPiecesBlocked) {
  Board board("k7/7R/8/7p/b4p1P/5N2/8/RQ5K b - - 0 1");

  BestMove best = negamax(board, 2, ALPHA_INIT, BETA_INIT, 0);

  EXPECT_EQ(best.score, 0);
  EXPECT_FALSE(best.move.has_value());
}

TEST(NegaMaxTest, KingVsKingIsInsufficientMaterialDraw) {
  Board board("8/8/8/8/8/8/8/Kk6 w - - 0 1");

  BestMove best = negamax(board, 2, ALPHA_INIT, BETA_INIT, 0);

  EXPECT_EQ(best.score, 0);
  EXPECT_FALSE(best.move.has_value());
}

TEST(NegaMaxTest, KingVsKingAndBishopIsInsufficientMaterialDraw) {
  Board board("8/8/8/8/8/8/8/KBk5 w - - 0 1");

  BestMove best = negamax(board, 2, ALPHA_INIT, BETA_INIT, 0);

  EXPECT_EQ(best.score, 0);
  EXPECT_FALSE(best.move.has_value());
}

TEST(NegaMaxTest, KingVsKingAndKnightIsInsufficientMaterialDraw) {
  Board board("8/8/8/8/8/8/8/KNk5 w - - 0 1");

  BestMove best = negamax(board, 2, ALPHA_INIT, BETA_INIT, 0);

  EXPECT_EQ(best.score, 0);
  EXPECT_FALSE(best.move.has_value());
}

TEST(NegaMaxTest, KingAndBishopVsKingAndSameColorBishopIsInsufficientMaterialDraw) {
  Board board("8/8/8/3b4/8/3B4/8/K1k5 w - - 0 1");

  BestMove best = negamax(board, 2, ALPHA_INIT, BETA_INIT, 0);

  EXPECT_EQ(best.score, 0);
  EXPECT_FALSE(best.move.has_value());
}
