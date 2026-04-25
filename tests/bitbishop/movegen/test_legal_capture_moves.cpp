#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/movegen/legal_moves.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

TEST(GenerateLegalCaptureMovesTest, StartingPositionHasNoCaptures) {
  Board board = Board::StartingPosition();

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_capture_moves(moves, board);

  EXPECT_TRUE(moves.empty());
}

TEST(GenerateLegalCaptureMovesTest, ExcludesQuietMovesAndCastling) {
  Board board("r3k2r/8/8/8/r7/8/8/R3K2R w KQkq - 0 1");

  std::vector<Move> moves;
  generate_legal_capture_moves(moves, board);

  EXPECT_GT(moves.size(), 0);
  EXPECT_TRUE(contains_move(moves, {A1, A4, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {H1, H8, std::nullopt, true, false, false}));

  EXPECT_FALSE(contains_move(moves, {E1, G1, std::nullopt, false, false, true}));
  EXPECT_FALSE(contains_move(moves, {E1, C1, std::nullopt, false, false, true}));
  EXPECT_FALSE(contains_move(moves, {A1, A2, std::nullopt, false, false, false}));

  for (const Move& move : moves) {
    EXPECT_TRUE(move.is_capture);
  }
}

TEST(GenerateLegalCaptureMovesTest, IncludesEnPassantCapture) {
  Board board("rnbqkbnr/pppp1ppp/8/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 1");

  std::vector<Move> moves;
  generate_legal_capture_moves(moves, board);

  EXPECT_TRUE(contains_move(moves, {D5, E6, std::nullopt, true, true, false}));
  EXPECT_EQ(count_en_passant(moves), 1);
  for (const Move& move : moves) {
    EXPECT_TRUE(move.is_capture);
  }
}

TEST(GenerateLegalCaptureMovesTest, IncludesCapturePromotionsOnly) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(A8, BLACK_KING);
  board.set_piece(G7, WHITE_PAWN);
  board.set_piece(H8, BLACK_ROOK);

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_capture_moves(moves, board);

  EXPECT_EQ(moves.size(), 4);
  EXPECT_TRUE(contains_move(moves, {G7, H8, WHITE_QUEEN, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {G7, H8, WHITE_ROOK, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {G7, H8, WHITE_BISHOP, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {G7, H8, WHITE_KNIGHT, true, false, false}));

  EXPECT_FALSE(contains_move(moves, {G7, G8, WHITE_QUEEN, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {G7, G8, WHITE_ROOK, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {G7, G8, WHITE_BISHOP, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {G7, G8, WHITE_KNIGHT, false, false, false}));
}

TEST(GenerateLegalCaptureMovesTest, InCheckOnlyLegalCaptureResponses) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(B5, WHITE_BISHOP);
  board.set_piece(E8, BLACK_ROOK);
  board.set_piece(H8, BLACK_KING);
  board.set_piece(A6, BLACK_PAWN);

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_capture_moves(moves, board);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {B5, E8, std::nullopt, true, false, false}));
  EXPECT_FALSE(contains_move(moves, {B5, A6, std::nullopt, true, false, false}));
}

TEST(GenerateLegalCaptureMovesTest, MovesVectorIsAppendedNotCleared) {
  Board board = Board::StartingPosition();
  std::vector<Move> moves;
  moves.emplace_back(Move::make(A1, A2));

  generate_legal_capture_moves(moves, board);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, Move::make(A1, A2)));
}
