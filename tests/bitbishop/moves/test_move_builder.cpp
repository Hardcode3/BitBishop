#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/moves/move_builder.hpp>
#include <bitbishop/moves/move_effect.hpp>
#include <bitbishop/moves/move_execution.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

TEST(MoveBuilderTest, QuietMoveGeneratesCorrectEffects) {
  Board board = Board::Empty();
  board.set_piece(E2, WHITE_PAWN);

  Move move{E2, E4};

  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();

  ASSERT_EQ(exec.count, 3);  // remove + place + state_change

  // Apply effects and check board change
  exec.apply(board);

  auto p_from = board.get_piece(E2);
  auto p_to = board.get_piece(E4);

  EXPECT_FALSE(p_from.has_value());
  ASSERT_TRUE(p_to.has_value());
  EXPECT_EQ(*p_to, WHITE_PAWN);

  // Revert and check restoration
  exec.revert(board);
  auto pf = board.get_piece(E2);
  EXPECT_TRUE(pf);
  EXPECT_TRUE(*pf == WHITE_PAWN);
  EXPECT_FALSE(board.get_piece(E4).has_value());
}

TEST(MoveBuilderTest, CaptureMoveGeneratesCorrectEffects) {
  Board board = Board::Empty();
  board.set_piece(E5, BLACK_KNIGHT);
  board.set_piece(C3, WHITE_BISHOP);

  Move move{.from = C3, .to = E5, .is_capture = true, .is_en_passant = false, .is_castling = false};

  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();

  // Should remove bishop from C3, remove knight from E5, place bishop on E5, + state change
  ASSERT_EQ(exec.count, 4);  // remove-moving, remove-captured, place-final, state-change

  exec.apply(board);

  EXPECT_FALSE(board.get_piece(C3));
  ASSERT_TRUE(board.get_piece(E5));
  EXPECT_EQ(*board.get_piece(E5), WHITE_BISHOP);

  exec.revert(board);

  EXPECT_TRUE(board.get_piece(C3));
  EXPECT_TRUE(*board.get_piece(C3) == WHITE_BISHOP);
  EXPECT_TRUE(board.get_piece(E5));
  EXPECT_TRUE(*board.get_piece(E5) == BLACK_KNIGHT);
}

TEST(MoveBuilderTest, EnPassantCaptureCreatesCorrectEffect) {
  Board board = Board::Empty();

  board.set_piece(E5, WHITE_PAWN);
  // Black pawn moved from D7 -> D5 to allow en passant at D6
  board.set_piece(D5, BLACK_PAWN);

  BoardState st = board.get_state();
  st.m_en_passant_sq = D6;
  st.m_is_white_turn = true;
  board.set_state(st);

  Move move{E5, D6};
  move.is_en_passant = true;

  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();

  // Expected effects:
  //  remove E5
  //  remove D5 (captured pawn)
  //  place D6 (white pawn)
  //  state change
  ASSERT_EQ(exec.count, 4);

  exec.apply(board);

  EXPECT_FALSE(board.get_piece(E5));
  EXPECT_FALSE(board.get_piece(D5));  // captured
  ASSERT_TRUE(board.get_piece(D6));
  EXPECT_EQ(*board.get_piece(D6), WHITE_PAWN);

  exec.revert(board);

  EXPECT_TRUE(board.get_piece(E5));
  EXPECT_TRUE(*board.get_piece(E5) == WHITE_PAWN);
  EXPECT_TRUE(board.get_piece(D5));
  EXPECT_TRUE(*board.get_piece(D5) == BLACK_PAWN);
  EXPECT_FALSE(board.get_piece(D6));
}

TEST(MoveBuilderTest, EnPassantIsResetIfNotUsed) {
  Board board = Board::Empty();

  board.set_piece(E5, WHITE_PAWN);
  // Black pawn moved from D7 -> D5 to allow en passant at D6
  board.set_piece(D5, BLACK_PAWN);
  board.set_piece(G1, WHITE_KNIGHT);

  BoardState st = board.get_state();
  st.m_en_passant_sq = D6;
  st.m_is_white_turn = true;
  board.set_state(st);

  Move move{G1, F3};
  move.is_en_passant = false;

  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();

  exec.apply(board);

  EXPECT_EQ(board.get_state().m_en_passant_sq, std::nullopt);

  exec.revert(board);

  EXPECT_TRUE(board.get_state().m_en_passant_sq);
  EXPECT_EQ(board.get_state().m_en_passant_sq, D6);
}

TEST(MoveBuilderTest, PromotionCreatesCorrectEffects) {
  Board board = Board::Empty();
  board.set_piece(E7, WHITE_PAWN);

  Move move{E7, E8};
  move.promotion = WHITE_QUEEN;

  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();

  ASSERT_EQ(exec.count, 3);  // remove pawn, place queen, state-change

  exec.apply(board);

  EXPECT_FALSE(board.get_piece(E7));
  ASSERT_TRUE(board.get_piece(E8));
  EXPECT_EQ(*board.get_piece(E8), WHITE_QUEEN);

  exec.revert(board);

  EXPECT_TRUE(board.get_piece(E7));
  EXPECT_TRUE(*board.get_piece(E7) == WHITE_PAWN);
  EXPECT_FALSE(board.get_piece(E8));
}

TEST(MoveBuilderTest, PromotionWithCaptureCreatesCorrectEffects) {
  Board board = Board::Empty();
  board.set_piece(E7, WHITE_PAWN);
  board.set_piece(F8, BLACK_QUEEN);

  Move move{E7, F8};
  move.promotion = WHITE_QUEEN;

  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();

  // remove pawn, remove captured, place queen, state-change
  ASSERT_EQ(exec.count, 4);

  exec.apply(board);

  EXPECT_FALSE(board.get_piece(E7));
  ASSERT_TRUE(board.get_piece(F8));
  EXPECT_EQ(*board.get_piece(F8), WHITE_QUEEN);

  exec.revert(board);

  EXPECT_TRUE(board.get_piece(E7));
  EXPECT_TRUE(*board.get_piece(E7) == WHITE_PAWN);
  EXPECT_TRUE(board.get_piece(F8));
  EXPECT_TRUE(*board.get_piece(F8) == BLACK_QUEEN);
}

TEST(MoveBuilderTest, CastlingKingsideWhitesGeneratesRookMove) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(H1, WHITE_ROOK);

  BoardState st = board.get_state();
  st.m_white_castle_kingside = true;
  st.m_white_castle_queenside = true;
  st.m_is_white_turn = true;
  board.set_state(st);

  Move move{E1, G1};
  move.is_castling = true;

  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();

  // Effects:
  // king remove E1, king place G1
  // rook remove H1, rook place F1
  // state change
  ASSERT_EQ(exec.count, 5);

  exec.apply(board);

  // King moved
  EXPECT_EQ(*board.get_piece(G1), WHITE_KING);
  EXPECT_FALSE(board.get_piece(E1));

  // Rook moved
  EXPECT_EQ(*board.get_piece(F1), WHITE_ROOK);
  EXPECT_FALSE(board.get_piece(H1));

  exec.revert(board);

  EXPECT_EQ(*board.get_piece(E1), WHITE_KING);
  EXPECT_EQ(*board.get_piece(H1), WHITE_ROOK);
}

TEST(MoveBuilderTest, CastlingQueensideWhitesGeneratesRookMove) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(A1, WHITE_ROOK);

  BoardState st = board.get_state();
  st.m_white_castle_kingside = true;
  st.m_white_castle_queenside = true;
  st.m_is_white_turn = true;
  board.set_state(st);

  Move move{E1, C1};
  move.is_castling = true;

  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();

  // Effects:
  // king remove E1, king place C1
  // rook remove A1, rook place D1
  // state change
  ASSERT_EQ(exec.count, 5);

  exec.apply(board);

  // King moved
  EXPECT_EQ(*board.get_piece(C1), WHITE_KING);
  EXPECT_FALSE(board.get_piece(E1));

  // Rook moved
  EXPECT_EQ(*board.get_piece(D1), WHITE_ROOK);
  EXPECT_FALSE(board.get_piece(A1));

  exec.revert(board);

  EXPECT_EQ(*board.get_piece(E1), WHITE_KING);
  EXPECT_EQ(*board.get_piece(A1), WHITE_ROOK);
}

TEST(MoveBuilderTest, CastlingKingsideBlacksGeneratesRookMove) {
  Board board = Board::Empty();
  board.set_piece(E8, BLACK_KING);
  board.set_piece(H8, BLACK_ROOK);

  BoardState st = board.get_state();
  st.m_black_castle_kingside = true;
  st.m_black_castle_queenside = true;
  st.m_is_white_turn = false;
  board.set_state(st);

  Move move{E8, G8};
  move.is_castling = true;

  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();

  // Effects:
  // king remove E8, king place G8
  // rook remove H8, rook place F8
  // state change
  ASSERT_EQ(exec.count, 5);

  exec.apply(board);

  // King moved
  EXPECT_EQ(*board.get_piece(G8), BLACK_KING);
  EXPECT_FALSE(board.get_piece(E8));

  // Rook moved
  EXPECT_EQ(*board.get_piece(F8), BLACK_ROOK);
  EXPECT_FALSE(board.get_piece(H8));

  exec.revert(board);

  EXPECT_EQ(*board.get_piece(E8), BLACK_KING);
  EXPECT_EQ(*board.get_piece(H8), BLACK_ROOK);
}

TEST(MoveBuilderTest, CastlingQueensideBlacksGeneratesRookMove) {
  Board board = Board::Empty();
  board.set_piece(E8, BLACK_KING);
  board.set_piece(A8, BLACK_ROOK);

  BoardState st = board.get_state();
  st.m_black_castle_kingside = true;
  st.m_black_castle_queenside = true;
  st.m_is_white_turn = false;
  board.set_state(st);

  Move move{E8, C8};
  move.is_castling = true;

  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();

  // Effects:
  // king remove E8, king place C8
  // rook remove A8, rook place D8
  // state change
  ASSERT_EQ(exec.count, 5);

  exec.apply(board);

  // King moved
  EXPECT_EQ(*board.get_piece(C8), BLACK_KING);
  EXPECT_FALSE(board.get_piece(E8));

  // Rook moved
  EXPECT_EQ(*board.get_piece(D8), BLACK_ROOK);
  EXPECT_FALSE(board.get_piece(A8));

  exec.revert(board);

  EXPECT_EQ(*board.get_piece(E8), BLACK_KING);
  EXPECT_EQ(*board.get_piece(A8), BLACK_ROOK);
}

TEST(MoveBuilderTest, DoublePawnPushSetsEnPassantSquare) {
  Board board = Board::Empty();
  board.set_piece(E2, WHITE_PAWN);

  Move move{E2, E4};

  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();

  exec.apply(board);

  BoardState st = board.get_state();
  ASSERT_TRUE(st.m_en_passant_sq.has_value());
  EXPECT_EQ(st.m_en_passant_sq.value(), E3);

  exec.revert(board);

  EXPECT_TRUE(board.get_piece(E2));
  EXPECT_EQ(board.get_piece(E2), WHITE_PAWN);
  EXPECT_FALSE(board.get_state().m_en_passant_sq.has_value());
}

TEST(MoveBuilderTest, KingMoveRevokesCastlingRights) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);

  BoardState st = board.get_state();
  st.m_white_castle_kingside = true;
  st.m_white_castle_queenside = true;
  board.set_state(st);

  Move move{E1, E2};

  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();
  exec.apply(board);

  BoardState final_state = board.get_state();

  EXPECT_TRUE(board.get_piece(E2));
  EXPECT_EQ(board.get_piece(E2), WHITE_KING);
  EXPECT_FALSE(board.get_piece(E1));
  EXPECT_FALSE(final_state.m_white_castle_kingside);
  EXPECT_FALSE(final_state.m_white_castle_queenside);

  exec.revert(board);

  BoardState reverted = board.get_state();

  EXPECT_TRUE(board.get_piece(E1));
  EXPECT_EQ(board.get_piece(E1), WHITE_KING);
  EXPECT_FALSE(board.get_piece(E2));
  EXPECT_TRUE(reverted.m_white_castle_kingside);
  EXPECT_TRUE(reverted.m_white_castle_queenside);
}

TEST(MoveBuilderTest, RookMoveRevokesCastlingRights) {
  Board board = Board::Empty();
  board.set_piece(A1, WHITE_ROOK);

  BoardState st = board.get_state();
  st.m_white_castle_queenside = true;
  board.set_state(st);

  Move move{A1, A2};

  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();
  exec.apply(board);

  EXPECT_TRUE(board.get_piece(A2));
  EXPECT_EQ(board.get_piece(A2), WHITE_ROOK);
  EXPECT_FALSE(board.get_piece(A1));
  EXPECT_FALSE(board.get_state().m_white_castle_queenside);

  exec.revert(board);

  EXPECT_TRUE(board.get_piece(A1));
  EXPECT_EQ(board.get_piece(A1), WHITE_ROOK);
  EXPECT_FALSE(board.get_piece(A2));
  EXPECT_TRUE(board.get_state().m_white_castle_queenside);
}

TEST(MoveBuilderTest, HalfMoveClockResetsOnPawnMove) {
  Board board = Board::Empty();
  BoardState st = board.get_state();
  st.m_halfmove_clock = 7;
  board.set_state(st);

  board.set_piece(E2, WHITE_PAWN);

  Move move{E2, E3};

  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();
  exec.apply(board);

  EXPECT_EQ(board.get_state().m_halfmove_clock, 0);
}

TEST(MoveBuilderTest, HalfMoveClockResetsOnCapture) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_ROOK);
  board.set_piece(C4, BLACK_BISHOP);

  BoardState st = board.get_state();
  st.m_halfmove_clock = 7;
  board.set_state(st);

  Move move = Move::make(E4, C4, true);

  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();
  exec.apply(board);

  EXPECT_EQ(board.get_state().m_halfmove_clock, 0);
}

TEST(MoveBuilderTest, HalfMoveClockIncrementsOnQuietMove) {
  Board board = Board::Empty();
  board.set_piece(G1, WHITE_KNIGHT);

  BoardState st = board.get_state();
  st.m_halfmove_clock = 5;
  st.m_is_white_turn = true;
  board.set_state(st);

  Move move = Move::make(G1, F3, false);  // quiet knight move

  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();

  exec.apply(board);

  EXPECT_EQ(board.get_state().m_halfmove_clock, st.m_halfmove_clock + 1);
}

TEST(MoveBuilderTest, HalfMoveClockIncrementsOverTwoQuietMoves) {
  Board board = Board::Empty();
  board.set_piece(G1, WHITE_KNIGHT);
  board.set_piece(G8, BLACK_KNIGHT);

  BoardState st = board.get_state();
  st.m_halfmove_clock = 2;
  st.m_is_white_turn = true;
  board.set_state(st);

  // White quiet move
  Move wmove = Move::make(G1, F3, false);
  MoveBuilder wbuilder(board, wmove);
  MoveExecution wexec = wbuilder.build();
  wexec.apply(board);

  EXPECT_EQ(board.get_state().m_halfmove_clock, st.m_halfmove_clock + 1);

  // Black quiet move
  Move bmove = Move::make(G8, F6, false);
  MoveBuilder bbuilder(board, bmove);
  MoveExecution bexec = bbuilder.build();
  bexec.apply(board);

  EXPECT_EQ(board.get_state().m_halfmove_clock, st.m_halfmove_clock + 2);
}

TEST(MoveExecutionTest, HalfMoveClockRestoredOnRevert) {
  Board board = Board::Empty();
  board.set_piece(G1, WHITE_KNIGHT);

  BoardState before = board.get_state();
  before.m_halfmove_clock = 12;
  before.m_is_white_turn = true;
  board.set_state(before);

  Move move = Move::make(G1, F3, false);
  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();

  exec.apply(board);

  EXPECT_EQ(board.get_state().m_halfmove_clock, before.m_halfmove_clock + 1);

  exec.revert(board);

  BoardState after = board.get_state();
  EXPECT_EQ(after.m_halfmove_clock, before.m_halfmove_clock);
  EXPECT_EQ(after.m_is_white_turn, before.m_is_white_turn);
}

TEST(MoveBuilderTest, HalfMoveClockIncrementThenResetOnCapture) {
  Board board = Board::Empty();
  board.set_piece(G1, WHITE_KNIGHT);
  board.set_piece(E4, BLACK_BISHOP);

  BoardState st = board.get_state();
  st.m_halfmove_clock = 0;
  st.m_is_white_turn = true;
  board.set_state(st);

  // Quiet knight move
  {
    Move move1 = Move::make(G1, F3, false);
    MoveBuilder builder1(board, move1);
    MoveExecution exec1 = builder1.build();
    exec1.apply(board);

    EXPECT_EQ(board.get_state().m_halfmove_clock, st.m_halfmove_clock + 1);
  }

  // Capture bishop
  {
    Move move2 = Move::make(F3, E5, true);
    MoveBuilder builder2(board, move2);
    MoveExecution exec2 = builder2.build();
    exec2.apply(board);

    EXPECT_EQ(board.get_state().m_halfmove_clock, 0);
  }
}

TEST(MoveExecutionTest, SideToMoveFlipsCorrectly) {
  Board board = Board::Empty();
  board.set_piece(E2, WHITE_PAWN);

  BoardState prev_state = board.get_state();
  prev_state.m_is_white_turn = true;
  board.set_state(prev_state);

  Move move{E2, E3};

  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();

  EXPECT_TRUE(board.get_state().m_is_white_turn);

  exec.apply(board);

  EXPECT_FALSE(board.get_state().m_is_white_turn);
}

TEST(MoveBuilderTest, FullMoveIncrementsOnWhiteMove) {
  Board board = Board::Empty();
  board.set_piece(E2, WHITE_PAWN);

  BoardState st = board.get_state();
  st.m_fullmove_number = 10;
  st.m_is_white_turn = true;
  board.set_state(st);

  Move move = Move::make(E2, E4, false);
  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();

  exec.apply(board);

  EXPECT_EQ(board.get_state().m_fullmove_number, st.m_fullmove_number + 1);
}

TEST(MoveBuilderTest, FullMoveDoesNotIncrementOnBlackMove) {
  Board board = Board::Empty();
  board.set_piece(E7, BLACK_PAWN);

  BoardState st = board.get_state();
  st.m_fullmove_number = 10;
  st.m_is_white_turn = false;
  board.set_state(st);

  Move move = Move::make(E7, E5, false);
  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();

  exec.apply(board);

  EXPECT_EQ(board.get_state().m_fullmove_number, st.m_fullmove_number);
}

TEST(MoveBuilderTest, FullMoveNumberSequentialTurns) {
  Board board = Board::Empty();
  board.set_piece(E2, WHITE_PAWN);
  board.set_piece(E7, BLACK_PAWN);

  BoardState st = board.get_state();
  st.m_fullmove_number = 4;
  st.m_is_white_turn = true;
  board.set_state(st);

  // White moves -> should increment to 5
  Move wmove = Move::make(E2, E4, false);
  MoveBuilder wbuild(board, wmove);
  MoveExecution wexec = wbuild.build();
  wexec.apply(board);
  EXPECT_EQ(board.get_state().m_fullmove_number, st.m_fullmove_number + 1);

  // Black moves -> no increment
  Move bmove = Move::make(E7, E5, false);
  MoveBuilder bbuild(board, bmove);
  MoveExecution bexec = bbuild.build();
  bexec.apply(board);
  EXPECT_EQ(board.get_state().m_fullmove_number, st.m_fullmove_number + 1);
}

TEST(MoveExecutionTest, FullMoveNumberRestoredOnRevert) {
  Board board = Board::Empty();
  board.set_piece(E2, WHITE_PAWN);

  BoardState before = board.get_state();
  before.m_fullmove_number = 6;
  before.m_is_white_turn = true;
  board.set_state(before);

  Move move = Move::make(E2, E4, false);
  MoveBuilder builder(board, move);
  MoveExecution exec = builder.build();

  exec.apply(board);
  ASSERT_EQ(board.get_state().m_fullmove_number, before.m_fullmove_number + 1);

  exec.revert(board);

  EXPECT_EQ(board.get_state().m_fullmove_number, before.m_fullmove_number);
  EXPECT_EQ(board.get_state().m_is_white_turn, before.m_is_white_turn);
}
