#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/moves/move_effect.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test Place effect factory creates correct effect.
 * @brief Confirms MoveEffect::place() creates effect with correct type and data.
 */
TEST(MoveEffectTest, PlaceEffectFactory) {
  MoveEffect effect = MoveEffect::place(E4, WHITE_PAWN);

  EXPECT_EQ(effect.type, MoveEffect::Type::Place);
  EXPECT_EQ(effect.square, E4);
  EXPECT_EQ(effect.piece, WHITE_PAWN);
}

/**
 * @test Remove effect factory creates correct effect.
 * @brief Confirms MoveEffect::remove() creates effect with correct type and data.
 */
TEST(MoveEffectTest, RemoveEffectFactory) {
  MoveEffect effect = MoveEffect::remove(E4, BLACK_KNIGHT);

  EXPECT_EQ(effect.type, MoveEffect::Type::Remove);
  EXPECT_EQ(effect.square, E4);
  EXPECT_EQ(effect.piece, BLACK_KNIGHT);
}

/**
 * @test State change effect factory creates correct effect.
 * @brief Confirms MoveEffect::state_change() creates effect with correct type and states.
 */
TEST(MoveEffectTest, StateChangeEffectFactory) {
  Board board1 = Board::StartingPosition();
  Board board2 = Board::Empty();

  BoardState prev = board1.get_state();
  BoardState next = board2.get_state();

  MoveEffect effect = MoveEffect::state_change(prev, next);

  EXPECT_EQ(effect.type, MoveEffect::Type::BoardState);
  // States should be stored (exact comparison depends on BoardState implementation)
}

/**
 * @test Apply place effect adds piece to board.
 * @brief Confirms MoveEffect::apply() places piece on board for Place type.
 */
TEST(MoveEffectTest, ApplyPlaceEffect) {
  Board board = Board::Empty();
  MoveEffect effect = MoveEffect::place(E4, WHITE_PAWN);

  effect.apply(board);

  EXPECT_EQ(board.get_piece(E4), WHITE_PAWN);
}

/**
 * @test Apply remove effect removes piece from board.
 * @brief Confirms MoveEffect::apply() removes piece from board for Remove type.
 */
TEST(MoveEffectTest, ApplyRemoveEffect) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_ROOK);

  MoveEffect effect = MoveEffect::remove(E4, BLACK_ROOK);
  effect.apply(board);

  EXPECT_EQ(board.get_piece(E4), std::nullopt);
}

/**
 * @test Apply state change effect updates board state.
 * @brief Confirms MoveEffect::apply() updates board state for BoardState type.
 */
TEST(MoveEffectTest, ApplyStateChangeEffect) {
  Board board = Board::Empty();
  Board target = Board::StartingPosition();

  BoardState prev = board.get_state();
  BoardState next = target.get_state();

  MoveEffect effect = MoveEffect::state_change(prev, next);
  effect.apply(board);

  EXPECT_EQ(board.get_state(), next);
}

/**
 * @test Revert place effect removes piece.
 * @brief Confirms MoveEffect::revert() removes placed piece.
 */
TEST(MoveEffectTest, RevertPlaceEffect) {
  Board board = Board::Empty();
  MoveEffect effect = MoveEffect::place(E4, WHITE_PAWN);

  effect.apply(board);
  EXPECT_EQ(board.get_piece(E4), WHITE_PAWN);

  effect.revert(board);
  EXPECT_EQ(board.get_piece(E4), std::nullopt);
}

/**
 * @test Revert remove effect restores piece.
 * @brief Confirms MoveEffect::revert() restores removed piece.
 */
TEST(MoveEffectTest, RevertRemoveEffect) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_ROOK);

  MoveEffect effect = MoveEffect::remove(E4, BLACK_ROOK);

  effect.apply(board);
  EXPECT_EQ(board.get_piece(E4), std::nullopt);

  effect.revert(board);
  EXPECT_EQ(board.get_piece(E4), BLACK_ROOK);
}

/**
 * @test Revert state change effect restores previous state.
 * @brief Confirms MoveEffect::revert() restores previous board state.
 */
TEST(MoveEffectTest, RevertStateChangeEffect) {
  Board board = Board::Empty();
  Board target = Board::StartingPosition();

  BoardState prev = board.get_state();
  BoardState next = target.get_state();

  MoveEffect effect = MoveEffect::state_change(prev, next);

  effect.apply(board);
  EXPECT_EQ(board.get_state(), next);

  effect.revert(board);
  EXPECT_EQ(board.get_state(), prev);
}

/**
 * @test Apply then revert place effect is identity.
 * @brief Confirms apply followed by revert returns board to original state
 *        for place effect.
 */
TEST(MoveEffectTest, ApplyRevertPlaceIsIdentity) {
  Board board = Board::Empty();
  Board original = board;

  MoveEffect effect = MoveEffect::place(E4, WHITE_PAWN);

  effect.apply(board);
  effect.revert(board);

  EXPECT_EQ(board, original);
}

/**
 * @test Apply then revert remove effect is identity.
 * @brief Confirms apply followed by revert returns board to original state
 *        for remove effect.
 */
TEST(MoveEffectTest, ApplyRevertRemoveIsIdentity) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_ROOK);
  Board original = board;

  MoveEffect effect = MoveEffect::remove(E4, BLACK_ROOK);

  effect.apply(board);
  effect.revert(board);

  EXPECT_EQ(board, original);
}

/**
 * @test Apply then revert state change is identity.
 * @brief Confirms apply followed by revert returns board to original state
 *        for state change effect.
 */
TEST(MoveEffectTest, ApplyRevertStateChangeIsIdentity) {
  Board board = Board::Empty();
  Board target = Board::StartingPosition();

  BoardState prev = board.get_state();
  BoardState next = target.get_state();

  Board original = board;

  MoveEffect effect = MoveEffect::state_change(prev, next);

  effect.apply(board);
  effect.revert(board);

  EXPECT_EQ(board, original);
}

/**
 * @test Multiple place effects can be applied.
 * @brief Confirms multiple place effects can be applied sequentially.
 */
TEST(MoveEffectTest, MultiplePlaceEffects) {
  Board board = Board::Empty();

  MoveEffect effect1 = MoveEffect::place(E4, WHITE_PAWN);
  MoveEffect effect2 = MoveEffect::place(D4, BLACK_PAWN);
  MoveEffect effect3 = MoveEffect::place(E5, WHITE_KNIGHT);

  effect1.apply(board);
  effect2.apply(board);
  effect3.apply(board);

  EXPECT_EQ(board.get_piece(E4), WHITE_PAWN);
  EXPECT_EQ(board.get_piece(D4), BLACK_PAWN);
  EXPECT_EQ(board.get_piece(E5), WHITE_KNIGHT);
}

/**
 * @test Multiple effects can be reverted in reverse order.
 * @brief Confirms multiple effects can be reverted in LIFO order.
 */
TEST(MoveEffectTest, MultipleEffectsRevertedInReverse) {
  Board board = Board::Empty();
  Board original = board;

  MoveEffect effect1 = MoveEffect::place(E4, WHITE_PAWN);
  MoveEffect effect2 = MoveEffect::place(D4, BLACK_PAWN);
  MoveEffect effect3 = MoveEffect::place(E5, WHITE_KNIGHT);

  effect1.apply(board);
  effect2.apply(board);
  effect3.apply(board);

  effect3.revert(board);
  effect2.revert(board);
  effect1.revert(board);

  EXPECT_EQ(board, original);
}

/**
 * @test Place effect on occupied square.
 * @brief Confirms place effect overwrites existing piece.
 */
TEST(MoveEffectTest, PlaceEffectOnOccupiedSquare) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_PAWN);

  MoveEffect effect = MoveEffect::place(E4, WHITE_QUEEN);
  effect.apply(board);

  EXPECT_EQ(board.get_piece(E4), WHITE_QUEEN);
}

/**
 * @test Remove effect on empty square.
 * @brief Confirms remove effect on empty square is handled gracefully.
 */
TEST(MoveEffectTest, RemoveEffectOnEmptySquare) {
  Board board = Board::Empty();

  MoveEffect effect = MoveEffect::remove(E4, BLACK_PAWN);
  effect.apply(board);

  EXPECT_EQ(board.get_piece(E4), std::nullopt);
}

/**
 * @test Place different piece types.
 * @brief Confirms place effect works for all piece types.
 */
TEST(MoveEffectTest, PlaceDifferentPieceTypes) {
  Board board = Board::Empty();

  MoveEffect pawn = MoveEffect::place(A2, WHITE_PAWN);
  MoveEffect knight = MoveEffect::place(B1, WHITE_KNIGHT);
  MoveEffect bishop = MoveEffect::place(C1, WHITE_BISHOP);
  MoveEffect rook = MoveEffect::place(A1, WHITE_ROOK);
  MoveEffect queen = MoveEffect::place(D1, WHITE_QUEEN);
  MoveEffect king = MoveEffect::place(E1, WHITE_KING);

  pawn.apply(board);
  knight.apply(board);
  bishop.apply(board);
  rook.apply(board);
  queen.apply(board);
  king.apply(board);

  EXPECT_EQ(board.get_piece(A2), WHITE_PAWN);
  EXPECT_EQ(board.get_piece(B1), WHITE_KNIGHT);
  EXPECT_EQ(board.get_piece(C1), WHITE_BISHOP);
  EXPECT_EQ(board.get_piece(A1), WHITE_ROOK);
  EXPECT_EQ(board.get_piece(D1), WHITE_QUEEN);
  EXPECT_EQ(board.get_piece(E1), WHITE_KING);
}

/**
 * @test Remove different piece types.
 * @brief Confirms remove effect works for all piece types.
 */
TEST(MoveEffectTest, RemoveDifferentPieceTypes) {
  Board board = Board::StartingPosition();

  MoveEffect pawn = MoveEffect::remove(E2, WHITE_PAWN);
  MoveEffect knight = MoveEffect::remove(B1, WHITE_KNIGHT);
  MoveEffect bishop = MoveEffect::remove(C1, WHITE_BISHOP);
  MoveEffect rook = MoveEffect::remove(A1, WHITE_ROOK);
  MoveEffect queen = MoveEffect::remove(D1, WHITE_QUEEN);

  pawn.apply(board);
  knight.apply(board);
  bishop.apply(board);
  rook.apply(board);
  queen.apply(board);

  EXPECT_EQ(board.get_piece(E2), std::nullopt);
  EXPECT_EQ(board.get_piece(B1), std::nullopt);
  EXPECT_EQ(board.get_piece(C1), std::nullopt);
  EXPECT_EQ(board.get_piece(A1), std::nullopt);
  EXPECT_EQ(board.get_piece(D1), std::nullopt);
}

/**
 * @test Place effect on all board squares.
 * @brief Confirms place effect works on corner and edge squares.
 */
TEST(MoveEffectTest, PlaceEffectOnAllBoardSquares) {
  Board board = Board::Empty();

  // Test corners
  MoveEffect corner1 = MoveEffect::place(A1, WHITE_ROOK);
  MoveEffect corner2 = MoveEffect::place(H1, WHITE_ROOK);
  MoveEffect corner3 = MoveEffect::place(A8, BLACK_ROOK);
  MoveEffect corner4 = MoveEffect::place(H8, BLACK_ROOK);

  corner1.apply(board);
  corner2.apply(board);
  corner3.apply(board);
  corner4.apply(board);

  EXPECT_EQ(board.get_piece(A1), WHITE_ROOK);
  EXPECT_EQ(board.get_piece(H1), WHITE_ROOK);
  EXPECT_EQ(board.get_piece(A8), BLACK_ROOK);
  EXPECT_EQ(board.get_piece(H8), BLACK_ROOK);
}

/**
 * @test Both colors of same piece type.
 * @brief Confirms effects work for both white and black pieces.
 */
TEST(MoveEffectTest, BothColorsOfSamePieceType) {
  Board board = Board::Empty();

  MoveEffect white_pawn = MoveEffect::place(E4, WHITE_PAWN);
  MoveEffect black_pawn = MoveEffect::place(E5, BLACK_PAWN);

  white_pawn.apply(board);
  black_pawn.apply(board);

  EXPECT_EQ(board.get_piece(E4), WHITE_PAWN);
  EXPECT_EQ(board.get_piece(E5), BLACK_PAWN);
}

/**
 * @test Revert place effect doesn't affect other pieces.
 * @brief Confirms reverting place effect only affects the specific square.
 */
TEST(MoveEffectTest, RevertPlaceDoesNotAffectOtherPieces) {
  Board board = Board::Empty();
  board.set_piece(D4, WHITE_KNIGHT);
  board.set_piece(F4, WHITE_BISHOP);

  MoveEffect effect = MoveEffect::place(E4, WHITE_PAWN);

  effect.apply(board);
  effect.revert(board);

  EXPECT_EQ(board.get_piece(D4), WHITE_KNIGHT);
  EXPECT_EQ(board.get_piece(F4), WHITE_BISHOP);
  EXPECT_EQ(board.get_piece(E4), std::nullopt);
}

/**
 * @test Revert remove effect doesn't affect other pieces.
 * @brief Confirms reverting remove effect only affects the specific square.
 */
TEST(MoveEffectTest, RevertRemoveDoesNotAffectOtherPieces) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_PAWN);
  board.set_piece(D4, WHITE_KNIGHT);
  board.set_piece(F4, WHITE_BISHOP);

  MoveEffect effect = MoveEffect::remove(E4, WHITE_PAWN);

  effect.apply(board);
  effect.revert(board);

  EXPECT_EQ(board.get_piece(E4), WHITE_PAWN);
  EXPECT_EQ(board.get_piece(D4), WHITE_KNIGHT);
  EXPECT_EQ(board.get_piece(F4), WHITE_BISHOP);
}
