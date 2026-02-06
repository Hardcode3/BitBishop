#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/engine/evaluation.hpp>

using namespace Eval;
using namespace Squares;
using namespace Pieces;

TEST(MaterialEvaluationTest, WhitesHavePositiveMaterialScore) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E5, WHITE_PAWN);

  int score = evaluate_material(board, Color::WHITE);

  EXPECT_GT(score, 0);
}

TEST(MaterialEvaluationTest, BlacksHavePositiveMaterialScore) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_KING);
  board.set_piece(E5, BLACK_PAWN);

  int score = evaluate_material(board, Color::BLACK);

  EXPECT_GT(score, 0);
}

TEST(MaterialEvaluationTest, NoPieceButKingEvaluatesToZero) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);

  int score = evaluate_material(board, Color::WHITE);

  EXPECT_EQ(score, 0);
}

TEST(MaterialEvaluationTest, NoPieceEvaluatesToZeroMaterial) {
  Board board = Board::Empty();

  int score = evaluate_material(board, Color::WHITE);

  EXPECT_EQ(score, 0);
}

TEST(MaterialEvaluationTest, NoPieceButKingAndPawn) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E5, WHITE_PAWN);

  int score = evaluate_material(board, Color::WHITE);

  EXPECT_EQ(score, MaterialValue::PAWN);
}

TEST(MaterialEvaluationTest, NoPieceButKingAndKnight) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E5, WHITE_KNIGHT);

  int score = evaluate_material(board, Color::WHITE);

  EXPECT_EQ(score, MaterialValue::KNIGHT);
}

TEST(MaterialEvaluationTest, NoPieceButKingAndBishop) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E5, WHITE_BISHOP);

  int score = evaluate_material(board, Color::WHITE);

  EXPECT_EQ(score, MaterialValue::BISHOP);
}
TEST(MaterialEvaluationTest, NoPieceButKingAndRook) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E5, WHITE_ROOK);

  int score = evaluate_material(board, Color::WHITE);

  EXPECT_EQ(score, MaterialValue::ROOK);
}

TEST(MaterialEvaluationTest, NoPieceButKingAndQueen) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(E5, WHITE_QUEEN);

  int score = evaluate_material(board, Color::WHITE);

  EXPECT_EQ(score, MaterialValue::QUEEN);
}

TEST(MaterialEvaluationTest, MixedMaterialCorrectlyAccumulates) {
  Board board("8/8/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  std::size_t nb_queens = 1;
  std::size_t nb_rooks = 2;
  std::size_t nb_bishops = 2;
  std::size_t nb_knights = 2;
  std::size_t nb_pawns = 8;
  std::size_t nb_king = 1;

  int score = evaluate_material(board, Color::WHITE);

  int expected_score = nb_queens * MaterialValue::QUEEN + nb_rooks * MaterialValue::ROOK +
                       nb_bishops * MaterialValue::BISHOP + nb_knights * MaterialValue::KNIGHT +
                       nb_pawns * MaterialValue::PAWN;

  EXPECT_EQ(score, expected_score);
}

TEST(MaterialEvaluationTest, MixedMaterialColorAreIsolated) {
  Board board("nbrkqnbr/pppppppp/8/8/8/2KQRBNP/PPPPPPPP/NBRKQNBR b - - 0 1");

  int score_white = evaluate_material(board, Color::WHITE);
  int score_black = evaluate_material(board, Color::BLACK);

  EXPECT_GT(score_white, score_black);
}

TEST(MaterialEvaluationTest, SymmetricColorSetupProvidesSameScore) {
  Board board = Board::StartingPosition();

  int score_white = evaluate_material(board, Color::WHITE);
  int score_black = evaluate_material(board, Color::BLACK);

  EXPECT_EQ(score_white, score_black);
}
