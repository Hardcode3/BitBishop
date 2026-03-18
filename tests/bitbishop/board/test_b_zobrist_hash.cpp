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
 * @test set_piece updates the key.
 * @brief Confirms placing a piece is reflected in the internal Zobrist key.
 */
TEST(BoardZobristTest, SetPieceUpdatesHash) {
  Board board = Board::Empty();
  const Zobrist::Key reference_hash = board.get_zobrist_hash();

  board.set_piece(E4, WHITE_PAWN);
  Zobrist::Key place_delta = Zobrist::NULL_HASH;
  Zobrist::mutate_piece(E4, WHITE_PAWN, place_delta);
  EXPECT_EQ(board.get_zobrist_hash(), reference_hash ^ place_delta);
  EXPECT_EQ(board.get_zobrist_hash(), Zobrist::compute_hash(board));
}

/**
 * @test remove_piece updates the key.
 * @brief Confirms removing a piece is reflected in the internal Zobrist key.
 */
TEST(BoardZobristTest, RemovePieceUpdatesHash) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_PAWN);

  const Zobrist::Key reference_hash = board.get_zobrist_hash();

  board.remove_piece(E4);
  Zobrist::Key remove_delta = Zobrist::NULL_HASH;
  Zobrist::mutate_piece(E4, WHITE_PAWN, remove_delta);
  EXPECT_EQ(board.get_zobrist_hash(), reference_hash ^ remove_delta);
  EXPECT_EQ(board.get_zobrist_hash(), Zobrist::compute_hash(board));
}

/**
 * @test remove_piece is a no-op on empty squares.
 * @brief Confirms removing a non-existent piece does not change the hash.
 */
TEST(BoardZobristTest, RemovePieceNoopDoesNotChangeHash) {
  Board board = Board::Empty();
  const Zobrist::Key reference_hash = board.get_zobrist_hash();

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
 * @brief Confirms moving a piece to an empty square updates the internal Zobrist key.
 */
TEST(BoardZobristTest, MovePieceToEmptySquareUpdatesHash) {
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

/**
 * @test move_piece updates the key on capture.
 * @brief Confirms capturing a piece updates the internal Zobrist key.
 */
TEST(BoardZobristTest, MovePieceCaptureUpdatesHash) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_PAWN);
  board.set_piece(D5, BLACK_PAWN);

  const Zobrist::Key reference_hash = board.get_zobrist_hash();

  // e4xd5
  board.move_piece(E4, D5);

  Zobrist::Key capture_delta = Zobrist::NULL_HASH;
  Zobrist::mutate_piece(D5, BLACK_PAWN, capture_delta);  // captured
  Zobrist::mutate_piece(E4, WHITE_PAWN, capture_delta);  // moved-from
  Zobrist::mutate_piece(D5, WHITE_PAWN, capture_delta);  // moved-to
  EXPECT_EQ(board.get_zobrist_hash(), reference_hash ^ capture_delta);
  EXPECT_EQ(board.get_zobrist_hash(), Zobrist::compute_hash(board));
}

/**
 * @test set_state updates the key for castling rights changes.
 * @brief Confirms castling rights are reflected in the internal Zobrist key.
 */
TEST(BoardZobristTest, SetStateCastlingRightsUpdatesHash) {
  Board board = Board::StartingPosition();
  const Zobrist::Key reference_hash = board.get_zobrist_hash();
  const BoardState prev = board.get_state();

  BoardState next = prev;
  next.m_white_castle_kingside = false;

  Zobrist::Key expected_delta = Zobrist::NULL_HASH;
  Zobrist::mutate_castling_rights(prev, expected_delta);
  Zobrist::mutate_castling_rights(next, expected_delta);

  board.set_state(next);
  EXPECT_EQ(board.get_zobrist_hash(), reference_hash ^ expected_delta);
  EXPECT_EQ(board.get_zobrist_hash(), Zobrist::compute_hash(board));
}

/**
 * @test set_state updates the key for en-passant changes.
 * @brief Confirms en-passant square changes are reflected in the internal Zobrist key.
 */
TEST(BoardZobristTest, SetStateEnPassantUpdatesHash) {
  Board board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
  const Zobrist::Key reference_hash = board.get_zobrist_hash();
  const BoardState prev = board.get_state();

  BoardState next = prev;
  next.m_en_passant_sq = std::nullopt;

  Zobrist::Key expected_delta = Zobrist::NULL_HASH;
  Zobrist::mutate_en_passant_square(*prev.m_en_passant_sq, expected_delta);

  board.set_state(next);
  EXPECT_EQ(board.get_zobrist_hash(), reference_hash ^ expected_delta);
  EXPECT_EQ(board.get_zobrist_hash(), Zobrist::compute_hash(board));
}

/**
 * @test set_state updates the key for side-to-move changes.
 * @brief Confirms side-to-move changes are reflected in the internal Zobrist key.
 */
TEST(BoardZobristTest, SetStateSideToMoveUpdatesHash) {
  Board board = Board::StartingPosition();
  const Zobrist::Key reference_hash = board.get_zobrist_hash();
  const BoardState prev = board.get_state();

  BoardState next = prev;
  next.m_is_white_turn = false;

  Zobrist::Key expected_delta = Zobrist::NULL_HASH;
  Zobrist::mutate_side_to_move(expected_delta);

  board.set_state(next);
  EXPECT_EQ(board.get_zobrist_hash(), reference_hash ^ expected_delta);
  EXPECT_EQ(board.get_zobrist_hash(), Zobrist::compute_hash(board));
}

/**
 * @test set_state ignores history counters.
 * @brief Confirms halfmove/fullmove counters do not affect the Zobrist key.
 */
TEST(BoardZobristTest, SetStateHistoryCountersDoNotAffectHash) {
  Board board = Board::StartingPosition();

  BoardState next = board.get_state();
  next.m_halfmove_clock += 1;
  next.m_fullmove_number += 1;

  const Zobrist::Key reference_hash = board.get_zobrist_hash();

  board.set_state(next);
  EXPECT_EQ(board.get_zobrist_hash(), reference_hash);
  EXPECT_EQ(board.get_zobrist_hash(), Zobrist::compute_hash(board));
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
