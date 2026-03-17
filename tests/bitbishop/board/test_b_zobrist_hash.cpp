#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/square.hpp>
#include <bitbishop/zobrist.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test Stored Zobrist key matches full recomputation.
 * @brief Confirms Board keeps its internal key consistent on construction.
 */
TEST(BoardZobristTest, StoredHashMatchesFullRecompute) {
  Board empty = Board::Empty();
  EXPECT_EQ(empty.get_zobrist_hash(), Zobrist::compute_hash(empty));

  Board start = Board::StartingPosition();
  EXPECT_EQ(start.get_zobrist_hash(), Zobrist::compute_hash(start));

  Board fen("rnbqkbnr/pppp1ppp/8/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 1");
  EXPECT_EQ(fen.get_zobrist_hash(), Zobrist::compute_hash(fen));
}

/**
 * @test set_piece/remove_piece update the key.
 * @brief Confirms piece mutations are reflected in the internal Zobrist key.
 */
TEST(BoardZobristTest, SetAndRemovePieceUpdateHash) {
  Board board = Board::Empty();
  const Zobrist::Key reference_hash = board.get_zobrist_hash();

  board.set_piece(E4, WHITE_PAWN);
  Zobrist::Key place_delta = Zobrist::NULL_HASH;
  Zobrist::mutate_piece(E4, WHITE_PAWN, place_delta);
  EXPECT_EQ(board.get_zobrist_hash(), reference_hash ^ place_delta);
  EXPECT_EQ(board.get_zobrist_hash(), Zobrist::compute_hash(board));

  board.remove_piece(E4);
  EXPECT_EQ(board.get_zobrist_hash(), reference_hash);
  EXPECT_EQ(board.get_zobrist_hash(), Zobrist::compute_hash(board));

  // No-op removal should not change the key.
  board.remove_piece(E4);
  EXPECT_EQ(board.get_zobrist_hash(), reference_hash);
}

/**
 * @test Replacing a piece updates the key.
 * @brief Confirms overwriting a square toggles out the previous piece and toggles in the new piece.
 */
TEST(BoardZobristTest, ReplacePieceUpdatesHash) {
  Board board = Board::Empty();
  const Zobrist::Key reference_hash = board.get_zobrist_hash();

  board.set_piece(E4, WHITE_PAWN);
  board.set_piece(E4, BLACK_QUEEN);

  Zobrist::Key queen_delta = Zobrist::NULL_HASH;
  Zobrist::mutate_piece(E4, BLACK_QUEEN, queen_delta);
  EXPECT_EQ(board.get_zobrist_hash(), reference_hash ^ queen_delta);
  EXPECT_EQ(board.get_zobrist_hash(), Zobrist::compute_hash(board));
}

/**
 * @test move_piece updates the key.
 * @brief Confirms moving (and capturing) pieces updates the internal Zobrist key.
 */
TEST(BoardZobristTest, MovePieceUpdatesHash) {
  {
    Board board = Board::Empty();
    board.set_piece(E2, WHITE_PAWN);
    const Zobrist::Key reference_hash = board.get_zobrist_hash();

    board.move_piece(E2, E4);

    Zobrist::Key move_delta = Zobrist::NULL_HASH;
    Zobrist::mutate_piece(E2, WHITE_PAWN, move_delta);
    Zobrist::mutate_piece(E4, WHITE_PAWN, move_delta);
    EXPECT_EQ(board.get_zobrist_hash(), reference_hash ^ move_delta);
    EXPECT_EQ(board.get_zobrist_hash(), Zobrist::compute_hash(board));
  }

  {
    Board board = Board::Empty();
    board.set_piece(E2, WHITE_PAWN);
    board.set_piece(E4, BLACK_PAWN);
    const Zobrist::Key reference_hash = board.get_zobrist_hash();

    board.move_piece(E2, E4);

    Zobrist::Key capture_delta = Zobrist::NULL_HASH;
    Zobrist::mutate_piece(E4, BLACK_PAWN, capture_delta);  // captured
    Zobrist::mutate_piece(E2, WHITE_PAWN, capture_delta);  // moved-from
    Zobrist::mutate_piece(E4, WHITE_PAWN, capture_delta);  // moved-to
    EXPECT_EQ(board.get_zobrist_hash(), reference_hash ^ capture_delta);
    EXPECT_EQ(board.get_zobrist_hash(), Zobrist::compute_hash(board));
  }
}

/**
 * @test set_state/set_side_to_move update the key.
 * @brief Confirms changes to side/castling/en-passant are reflected in the internal Zobrist key.
 */
TEST(BoardZobristTest, StateUpdatesHash) {
  Board board = Board::StartingPosition();
  const Zobrist::Key reference_hash = board.get_zobrist_hash();
  const BoardState prev = board.get_state();

  BoardState next = prev;
  next.m_white_castle_kingside = false;
  next.m_en_passant_sq = E3;
  next.m_is_white_turn = false;

  Zobrist::Key expected_delta = Zobrist::NULL_HASH;
  Zobrist::mutate_board_state_diff(prev, next, expected_delta);

  board.set_state(next);
  EXPECT_EQ(board.get_zobrist_hash(), reference_hash ^ expected_delta);
  EXPECT_EQ(board.get_zobrist_hash(), Zobrist::compute_hash(board));

  // Changing only halfmove/fullmove counters should not affect the key.
  BoardState history_only = board.get_state();
  history_only.m_halfmove_clock += 1;
  history_only.m_fullmove_number += 1;
  const Zobrist::Key history_reference_hash = board.get_zobrist_hash();
  board.set_state(history_only);
  EXPECT_EQ(board.get_zobrist_hash(), history_reference_hash);
}

TEST(BoardZobristTest, SetSideToMoveTogglesHash) {
  Board board = Board::StartingPosition();
  const Zobrist::Key reference_hash = board.get_zobrist_hash();

  board.set_side_to_move(Color::BLACK);

  Zobrist::Key expected_delta = Zobrist::NULL_HASH;
  Zobrist::mutate_side_to_move(expected_delta);
  EXPECT_EQ(board.get_zobrist_hash(), reference_hash ^ expected_delta);
  EXPECT_EQ(board.get_zobrist_hash(), Zobrist::compute_hash(board));
}

