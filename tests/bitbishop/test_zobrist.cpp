#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>
#include <bitbishop/zobrist.hpp>

using namespace Zobrist;

TEST(ZobristTest, MutatePieceIsReversible) {
  Key key = NULL_HASH;

  Square sq = Squares::E4;
  Piece piece = Pieces::WHITE_KNIGHT;

  Key original = key;

  mutate_piece(sq, piece, key);
  mutate_piece(sq, piece, key);

  EXPECT_EQ(key, original);
}

TEST(ZobristTest, MutateSideToMoveIsReversible) {
  Key key = NULL_HASH;
  Key original = key;

  mutate_side_to_move(key);
  mutate_side_to_move(key);

  EXPECT_EQ(key, original);
}

TEST(ZobristTest, MutateEnPassantIsReversible) {
  Key key = NULL_HASH;

  Square sq = Squares::E6;

  Key original = key;

  mutate_en_passant_square(sq, key);
  mutate_en_passant_square(sq, key);

  EXPECT_EQ(key, original);
}

TEST(ZobristTest, MutateCastlingRightsIsReversible) {
  BoardState state{};

  state.m_white_castle_kingside = true;
  state.m_white_castle_queenside = true;
  state.m_black_castle_kingside = false;
  state.m_black_castle_queenside = true;

  Key key = NULL_HASH;
  Key original = key;

  mutate_castling_rights(state, key);
  mutate_castling_rights(state, key);

  EXPECT_EQ(key, original);
}

TEST(ZobristTest, BoardStateDiffSideToMove) {
  BoardState prev{};
  BoardState next{};

  prev.m_is_white_turn = true;
  next.m_is_white_turn = false;

  Key key = NULL_HASH;

  mutate_board_state_diff(prev, next, key);

  Key expected = 0;
  mutate_side_to_move(expected);

  EXPECT_EQ(key, expected);
}

TEST(ZobristTest, BoardStateDiffCastlingRights) {
  BoardState prev{};
  BoardState next{};

  prev.m_white_castle_kingside = true;
  next.m_white_castle_kingside = false;

  Key key = NULL_HASH;

  mutate_board_state_diff(prev, next, key);

  Key expected = 0;
  mutate_castling_rights(prev, expected);
  mutate_castling_rights(next, expected);

  EXPECT_EQ(key, expected);
}

TEST(ZobristTest, BoardStateDiffEnPassant) {
  BoardState prev{};
  BoardState next{};

  prev.m_en_passant_sq = Squares::E6;
  next.m_en_passant_sq = Squares::D6;

  Key key = NULL_HASH;

  mutate_board_state_diff(prev, next, key);

  Key expected = 0;
  mutate_en_passant_square(*prev.m_en_passant_sq, expected);
  mutate_en_passant_square(*next.m_en_passant_sq, expected);

  EXPECT_EQ(key, expected);
}

TEST(ZobristTest, ComputeHashMatchesIncrementalPlacement) {
  Board board = Board::Empty();

  board.set_piece(Squares::E2, Pieces::WHITE_PAWN);
  board.set_piece(Squares::E7, Pieces::BLACK_PAWN);
  board.set_side_to_move(Color::WHITE);

  Key incremental = 0;

  mutate_piece(Squares::E2, Pieces::WHITE_PAWN, incremental);
  mutate_piece(Squares::E7, Pieces::BLACK_PAWN, incremental);
  mutate_castling_rights(board.get_state(), incremental);

  Key full = compute_hash(board);

  EXPECT_EQ(full, incremental);
}

TEST(ZobristTest, ComputeHashIncludesSideToMove) {
  Board board = Board::Empty();

  board.set_piece(Squares::E4, Pieces::WHITE_KNIGHT);
  board.set_side_to_move(Color::BLACK);

  Key incremental = 0;

  mutate_piece(Squares::E4, Pieces::WHITE_KNIGHT, incremental);
  mutate_side_to_move(incremental);
  mutate_castling_rights(board.get_state(), incremental);

  Key full = compute_hash(board);

  EXPECT_EQ(full, incremental);
}

TEST(ZobristTest, ComputeHashIncludesCastlingRights) {
  Board board = Board::Empty();

  board.set_piece(Squares::E1, Pieces::WHITE_KING);
  board.set_piece(Squares::H1, Pieces::WHITE_ROOK);
  board.set_side_to_move(Color::WHITE);

  BoardState state = board.get_state();
  state.m_white_castle_kingside = true;
  state.m_white_castle_queenside = false;
  state.m_black_castle_kingside = false;
  state.m_black_castle_queenside = false;
  board.set_state(state);

  Key incremental = 0;

  mutate_piece(Squares::E1, Pieces::WHITE_KING, incremental);
  mutate_piece(Squares::H1, Pieces::WHITE_ROOK, incremental);
  mutate_castling_rights(board.get_state(), incremental);

  Key full = compute_hash(board);

  EXPECT_EQ(full, incremental);
}

TEST(ZobristTest, ComputeHashIncludesEnPassant) {
  Board board = Board::Empty();

  board.set_piece(Squares::E5, Pieces::WHITE_PAWN);
  board.set_piece(Squares::D5, Pieces::BLACK_PAWN);
  board.set_side_to_move(Color::WHITE);

  BoardState state = board.get_state();
  state.m_en_passant_sq = Squares::E6;
  board.set_state(state);

  Key incremental = 0;

  mutate_piece(Squares::E5, Pieces::WHITE_PAWN, incremental);
  mutate_piece(Squares::D5, Pieces::BLACK_PAWN, incremental);
  mutate_castling_rights(board.get_state(), incremental);
  mutate_en_passant_square(*board.en_passant_square(), incremental);

  Key full = compute_hash(board);

  EXPECT_EQ(full, incremental);
}

TEST(ZobristTest, ComputeHashDifferentCastlingStatesProduceDifferentHashes) {
  Board board1 = Board::Empty();
  Board board2 = Board::Empty();

  board1.set_piece(Squares::E1, Pieces::WHITE_KING);
  board2.set_piece(Squares::E1, Pieces::WHITE_KING);

  BoardState s1 = board1.get_state();
  BoardState s2 = board2.get_state();

  s1.m_white_castle_kingside = true;
  s2.m_white_castle_kingside = false;

  board1.set_state(s1);
  board2.set_state(s2);

  Key h1 = compute_hash(board1);
  Key h2 = compute_hash(board2);

  EXPECT_NE(h1, h2);
}

TEST(ZobristTest, IncrementalMoveMatchesRecomputedHash) {
  Board board = Board::Empty();

  board.set_piece(Squares::E2, Pieces::WHITE_PAWN);

  Key incremental = compute_hash(board);

  mutate_piece(Squares::E2, Pieces::WHITE_PAWN, incremental);
  mutate_piece(Squares::E4, Pieces::WHITE_PAWN, incremental);

  board.move_piece(Squares::E2, Squares::E4);

  Key recomputed = compute_hash(board);

  EXPECT_EQ(incremental, recomputed);
}

TEST(ZobristTest, ComputeHashDifferentPositionsDifferentHashes) {
  Board board1 = Board::Empty();
  Board board2 = Board::Empty();

  board1.set_piece(Squares::E4, Pieces::WHITE_KNIGHT);
  board2.set_piece(Squares::E5, Pieces::WHITE_KNIGHT);

  Key h1 = compute_hash(board1);
  Key h2 = compute_hash(board2);

  EXPECT_NE(h1, h2);
}

TEST(ZobristTest, ComputeHashSideToMoveMatters) {
  Board board = Board::StartingPosition();

  Key white_hash = compute_hash(board);

  board.set_side_to_move(Color::BLACK);

  Key black_hash = compute_hash(board);

  EXPECT_NE(white_hash, black_hash);
}

TEST(ZobristTest, IncrementalHashMatchesFullRecompute) {
  Board board = Board::StartingPosition();

  Key full = compute_hash(board);
  Key incremental = full;

  Square from = Squares::E2;
  Square to = Squares::E4;

  Piece pawn = Pieces::WHITE_PAWN;

  mutate_piece(from, pawn, incremental);
  mutate_piece(to, pawn, incremental);

  board.move_piece(from, to);

  Key recomputed = compute_hash(board);

  EXPECT_EQ(incremental, recomputed);
}
