#include <gtest/gtest.h>

#include <bitbishop/engine/evaluation.hpp>
#include <bitbishop/engine/search.hpp>
#include <bitbishop/moves/position.hpp>
#include <tuple>

using namespace Search;
using namespace Pieces;
using namespace Squares;

TEST(NegaMaxTest, EmptyBoardThrows) {
  Board board = Board::Empty();
  Position pos(board);

  EXPECT_THROW(std::ignore = negamax(pos, 1, ALPHA_INIT, BETA_INIT, 0), std::bad_optional_access);
}

TEST(NegaMaxTest, EmptyBoardWithBothKingsDontThrow) {
  Board board = Board::Empty();
  board.set_piece(E5, WHITE_KING);
  board.set_piece(H8, BLACK_KING);

  board.set_side_to_move(Color::WHITE);
  Position pos(board);

  EXPECT_NO_THROW(std::ignore = negamax(pos, 1, ALPHA_INIT, BETA_INIT, 0));
}

TEST(NegaMaxTest, FindsScolarsMateInOne) {
  // White to move, Queen can take on f7 for mate
  Board board = Board("r1bqkbnr/pppp1ppp/2n5/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR w KQkq - 0 1");
  Position pos(board);

  BestMove best = negamax(pos, 2, ALPHA_INIT, BETA_INIT, 0);

  EXPECT_GT(best.score, Eval::MATE_THRESHOLD);
  EXPECT_TRUE(best.move.has_value());
  EXPECT_EQ(best.move->from, F3);
  EXPECT_EQ(best.move->to, F7);
}

TEST(NegaMaxTest, FindsCornerMateInOne) {
  Board board("7k/5K2/6Q1/8/8/8/8/8 w - - 0 1");
  Position pos(board);

  BestMove best = negamax(pos, 2, ALPHA_INIT, BETA_INIT, 0);

  EXPECT_GT(best.score, Eval::MATE_THRESHOLD);
  EXPECT_TRUE(best.move.has_value());
  EXPECT_EQ(best.move->from, G6);
  EXPECT_EQ(best.move->to, H5);
}

TEST(NegaMaxTest, FindsStaleMateByWhiteQueenInCorner) {
  Board board("K7/8/8/8/8/8/5Q2/7k b - - 0 1");
  Position pos(board);

  BestMove best = negamax(pos, 2, ALPHA_INIT, BETA_INIT, 0);

  EXPECT_EQ(best.score, 0);
  EXPECT_FALSE(best.move.has_value());
}

TEST(NegaMaxTest, FindsStaleMateByWhiteAllBlackPiecesBlocked) {
  Board board("k7/7R/8/7p/b4p1P/5N2/8/RQ5K b - - 0 1");
  Position pos(board);

  BestMove best = negamax(pos, 2, ALPHA_INIT, BETA_INIT, 0);

  EXPECT_EQ(best.score, 0);
  EXPECT_FALSE(best.move.has_value());
}

TEST(NegaMaxTest, KingVsKingIsInsufficientMaterialDraw) {
  Board board("8/8/8/8/8/8/8/K1k5 w - - 0 1");
  Position pos(board);

  BestMove best = negamax(pos, 2, ALPHA_INIT, BETA_INIT, 0);

  EXPECT_EQ(best.score, 0);
  EXPECT_FALSE(best.move.has_value());
}

TEST(NegaMaxTest, KingVsKingAndBishopIsInsufficientMaterialDraw) {
  Board board("8/8/8/8/8/8/8/KBk5 w - - 0 1");
  Position pos(board);

  BestMove best = negamax(pos, 2, ALPHA_INIT, BETA_INIT, 0);

  EXPECT_EQ(best.score, 0);
  EXPECT_FALSE(best.move.has_value());
}

TEST(NegaMaxTest, KingVsKingAndKnightIsInsufficientMaterialDraw) {
  Board board("8/8/8/8/8/8/8/KNk5 w - - 0 1");
  Position pos(board);

  BestMove best = negamax(pos, 2, ALPHA_INIT, BETA_INIT, 0);

  EXPECT_EQ(best.score, 0);
  EXPECT_FALSE(best.move.has_value());
}

TEST(NegaMaxTest, KingAndBishopVsKingAndSameColorBishopIsInsufficientMaterialDraw) {
  Board board("8/8/8/3b4/8/3B4/8/K1k5 w - - 0 1");
  Position pos(board);

  BestMove best = negamax(pos, 2, ALPHA_INIT, BETA_INIT, 0);

  EXPECT_EQ(best.score, 0);
  EXPECT_FALSE(best.move.has_value());
}

namespace {
/**
 * @brief Helper that reaches the same position again after 4 plies.
 *
 * Sequence from the starting position:
 *   1. Ng1-f3  ... Ng8-f6
 *   2. Nf3-g1  ... Nf6-g8
 *
 * This returns to the initial layout with the same side to move, which is the basic
 * building block used by repetition-related search tests below.
 */
void apply_knight_repetition_cycle(Position& pos) {
  // Returns to the initial layout (with the same side-to-move) after 4 plies.
  pos.apply_move(Move::make(G1, F3));
  pos.apply_move(Move::make(G8, F6));
  pos.apply_move(Move::make(F3, G1));
  pos.apply_move(Move::make(F6, G8));
}
}  // namespace

/**
 * @test Search returns a draw score on threefold repetition.
 * @brief Validates the new repetition cut-off in `Search::negamax(Position&, ...)` and `Search::quiesce(Position&,
 * ...)`.
 *
 * We explicitly pre-load the Position history to a threefold repetition, then assert the search returns `0`
 * immediately (draw) and does not propose a move (since it should not search further from a repeated node).
 */
TEST(NegaMaxTest, ThreefoldRepetitionIsDraw) {
  Board board = Board::StartingPosition();
  Position pos(board);

  apply_knight_repetition_cycle(pos);
  apply_knight_repetition_cycle(pos);
  EXPECT_TRUE(pos.is_threefold_repetition());

  BestMove best = negamax(pos, 2, ALPHA_INIT, BETA_INIT, 0);
  EXPECT_EQ(best.score, 0);
  EXPECT_FALSE(best.move.has_value());

  EXPECT_EQ(quiesce(pos, ALPHA_INIT, BETA_INIT), 0);
}

/**
 * @test Twofold repetition does not trigger the repetition draw cut-off.
 * @brief Ensures we don't treat "position seen twice" as an immediate draw.
 */
TEST(NegaMaxTest, TwofoldRepetitionDoesNotAutoDraw) {
  Board board = Board::StartingPosition();
  Position pos(board);

  apply_knight_repetition_cycle(pos);
  EXPECT_FALSE(pos.is_threefold_repetition());

  BestMove best = negamax(pos, 1, ALPHA_INIT, BETA_INIT, 0);
  EXPECT_TRUE(best.move.has_value());
}

/**
 * @test Search returns a draw score on the 50-move rule.
 * @brief Ensures halfmove clock >= 100 is treated as a claimable draw during search.
 */
TEST(NegaMaxTest, FiftyMoveRuleIsDraw) {
  Board board("8/8/8/8/8/8/8/RKk5 w - - 100 1");
  Position pos(board);

  BestMove best = negamax(pos, 2, ALPHA_INIT, BETA_INIT, 0);
  EXPECT_EQ(best.score, 0);
  EXPECT_FALSE(best.move.has_value());

  EXPECT_EQ(quiesce(pos, ALPHA_INIT, BETA_INIT), 0);
}
