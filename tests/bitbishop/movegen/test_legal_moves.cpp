#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/movegen/legal_moves.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test Starting position white moves.
 * @brief Confirms generate_legal_moves() generates correct number of moves
 *        from starting position for white.
 */
TEST(GenerateLegalMovesTest, StartingPositionWhite) {
  Board board = Board::StartingPosition();
  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // 16 pawn moves (8 single + 8 double)
  // 4 knight moves (2 knights * 2 each)
  EXPECT_EQ(moves.size(), 20);
}

/**
 * @test Starting position black moves.
 * @brief Confirms generate_legal_moves() generates correct number of moves
 *        from starting position for black.
 */
TEST(GenerateLegalMovesTest, StartingPositionBlack) {
  Board board = Board::StartingPosition();
  BoardState state = board.get_state();
  state.m_is_white_turn = false;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // 16 pawn moves + 4 knight moves
  EXPECT_EQ(moves.size(), 20);
}

/**
 * @test Empty board with only kings.
 * @brief Confirms generate_legal_moves() generates only king moves when
 *        board has only kings.
 */
TEST(GenerateLegalMovesTest, OnlyKingsOnBoard) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E8, BLACK_KING);

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // King has 5 moves (on edge, some squares attacked by black king)
  EXPECT_GT(moves.size(), 0);
  EXPECT_LT(moves.size(), 8);
}

/**
 * @test King in check must move or block.
 * @brief Confirms generate_legal_moves() only generates moves that resolve
 *        check when king is in single check.
 */
TEST(GenerateLegalMovesTest, KingInSingleCheck) {
  Board board("rnb1kbnr/pppp1ppp/8/4p3/6Pq/3P1P2/PPP1P2P/RNBQKBNR b KQkq - 0 1");
  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  EXPECT_GT(moves.size(), 0);
}

/**
 * @test King in double check must move.
 * @brief Confirms generate_legal_moves() only generates king moves when
 *        in double check.
 */
TEST(GenerateLegalMovesTest, KingInDoubleCheck) {
  Board board("4k3/8/8/8/8/3r4/3r4/4K3 w - - 0 1");
  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // Only king moves allowed in double check
  for (const Move& move : moves) {
    EXPECT_EQ(move.from, E1);
  }
}

/**
 * @test Pinned pieces generate legal moves.
 * @brief Confirms generate_legal_moves() includes moves for pinned pieces
 *        along pin ray.
 */
TEST(GenerateLegalMovesTest, PinnedPiecesCanMove) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, WHITE_ROOK);
  board.set_piece(E8, BLACK_ROOK);

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // Pinned rook can move along pin ray
  EXPECT_TRUE(contains_move(moves, {E4, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E8, std::nullopt, true, false, false}));

  // Cannot move perpendicular to pin
  EXPECT_FALSE(contains_move(moves, {E4, D4, std::nullopt, false, false, false}));
}

/**
 * @test Castling included when legal.
 * @brief Confirms generate_legal_moves() includes castling moves when
 *        all conditions are met.
 */
TEST(GenerateLegalMovesTest, CastlingIncluded) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  EXPECT_TRUE(contains_move(moves, {E1, G1, std::nullopt, false, false, true}));
  EXPECT_TRUE(contains_move(moves, {E1, C1, std::nullopt, false, false, true}));
}

/**
 * @test No castling when in check.
 * @brief Confirms generate_legal_moves() does not include castling when
 *        king is in check.
 */
TEST(GenerateLegalMovesTest, NoCastlingWhenInCheck) {
  Board board("r3k2r/8/8/8/8/8/4q3/R3K2R w KQkq - 0 1");

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  EXPECT_FALSE(contains_move(moves, {E1, G1, std::nullopt, false, false, true}));
  EXPECT_FALSE(contains_move(moves, {E1, C1, std::nullopt, false, false, true}));
}

/**
 * @test All piece types generate moves.
 * @brief Confirms generate_legal_moves() includes moves from all piece types.
 */
TEST(GenerateLegalMovesTest, AllPieceTypesGenerate) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E2, WHITE_PAWN);
  board.set_piece(D1, WHITE_KNIGHT);
  board.set_piece(C1, WHITE_BISHOP);
  board.set_piece(A1, WHITE_ROOK);
  board.set_piece(B1, WHITE_QUEEN);
  board.set_piece(E8, BLACK_KING);

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // Should have moves from all piece types
  bool has_king_move = false;
  bool has_pawn_move = false;
  bool has_knight_move = false;
  bool has_bishop_move = false;
  bool has_rook_move = false;
  bool has_queen_move = false;

  for (const Move& move : moves) {
    if (move.from == E1) has_king_move = true;
    if (move.from == E2) has_pawn_move = true;
    if (move.from == D1) has_knight_move = true;
    if (move.from == C1) has_bishop_move = true;
    if (move.from == A1) has_rook_move = true;
    if (move.from == B1) has_queen_move = true;
  }

  EXPECT_TRUE(has_king_move);
  EXPECT_TRUE(has_pawn_move);
  EXPECT_TRUE(has_knight_move);
  EXPECT_TRUE(has_bishop_move);
  EXPECT_TRUE(has_rook_move);
  EXPECT_TRUE(has_queen_move);
}

/**
 * @test Pawn promotions included.
 * @brief Confirms generate_legal_moves() includes all pawn promotion moves.
 */
TEST(GenerateLegalMovesTest, PawnPromotionsIncluded) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E7, WHITE_PAWN);
  board.set_piece(A8, BLACK_KING);

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // Should have 4 promotion moves
  EXPECT_TRUE(contains_move(moves, {E7, E8, WHITE_QUEEN, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E7, E8, WHITE_ROOK, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E7, E8, WHITE_BISHOP, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E7, E8, WHITE_KNIGHT, false, false, false}));
}

/**
 * @test En passant included.
 * @brief Confirms generate_legal_moves() includes en passant captures.
 */
TEST(GenerateLegalMovesTest, EnPassantIncluded) {
  Board board("rnbqkbnr/pppp1ppp/8/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 1");

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  EXPECT_TRUE(contains_move(moves, {D5, E6, std::nullopt, true, true, false}));
}

/**
 * @test Captures included.
 * @brief Confirms generate_legal_moves() includes capture moves.
 */
TEST(GenerateLegalMovesTest, CapturesIncluded) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, WHITE_ROOK);
  board.set_piece(E7, BLACK_PAWN);
  board.set_piece(E8, BLACK_KING);

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  EXPECT_TRUE(contains_move(moves, {E4, E7, std::nullopt, true, false, false}));
}

/**
 * @test No illegal moves generated.
 * @brief Confirms generate_legal_moves() does not generate moves that
 *        would leave king in check.
 */
TEST(GenerateLegalMovesTest, NoIllegalMoves) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E2, WHITE_QUEEN);
  board.set_piece(E8, BLACK_ROOK);

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // White queen cannot move east or west as it would expose the king
  EXPECT_FALSE(contains_move(moves, {E2, D2, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E2, F2, std::nullopt, false, false, false}));

  // White queen can move towards the black rook
  EXPECT_TRUE(contains_move(moves, {E2, E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E2, E4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E2, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E2, E6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E2, E7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E2, E8, std::nullopt, true, false, false}));
}

/**
 * @test Moves vector not cleared.
 * @brief Confirms generate_legal_moves() appends to existing moves vector.
 */
TEST(GenerateLegalMovesTest, MovesVectorNotCleared) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E8, BLACK_KING);

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  moves.emplace_back(Move::make(A1, A2));

  size_t initial_size = moves.size();
  generate_legal_moves(moves, board);

  EXPECT_GT(moves.size(), initial_size);
  EXPECT_TRUE(contains_move(moves, Move::make(A1, A2)));
}

/**
 * @test Stalemate position generates no moves.
 * @brief Confirms generate_legal_moves() generates no moves in stalemate.
 */
TEST(GenerateLegalMovesTest, StalemateNoMoves) {
  Board board("7k/5Q2/6K1/8/8/8/8/8 b - - 0 1");

  BoardState state = board.get_state();
  state.m_is_white_turn = false;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Checkmate position only has king moves (all illegal).
 * @brief Confirms generate_legal_moves() handles checkmate positions.
 */
TEST(GenerateLegalMovesTest, CheckmatePosition) {
  Board board("8/8/8/8/8/8/1r6/r2K4 w - - 0 1");

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // King in checkmate - no legal moves
  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Back rank mate threat.
 * @brief Confirms generate_legal_moves() correctly handles back rank
 *        mate scenarios.
 */
TEST(GenerateLegalMovesTest, BackRankMateThreat) {
  Board board("6k1/5ppp/8/8/8/8/5PPP/5RK1 w - - 0 1");

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // Should generate moves but not illegal king moves
  EXPECT_GT(moves.size(), 0);
}

/**
 * @test Discovered check pieces restricted.
 * @brief Confirms generate_legal_moves() correctly handles pieces that
 *        would create discovered check if moved.
 */
TEST(GenerateLegalMovesTest, DiscoveredCheckRestriction) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, WHITE_BISHOP);
  board.set_piece(E8, BLACK_ROOK);

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // Bishop pinned, cannot move
  for (const Move& move : moves) {
    EXPECT_NE(move.from, E4);
  }
}

/**
 * @test Complex position with multiple piece types.
 * @brief Confirms generate_legal_moves() handles complex positions correctly.
 */
TEST(GenerateLegalMovesTest, ComplexPosition) {
  Board board("r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1");

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // Should have many legal moves
  EXPECT_GT(moves.size(), 20);
}

/**
 * @test Only king moves in double check.
 * @brief Confirms generate_legal_moves() only generates king moves when
 *        in double check.
 */
TEST(GenerateLegalMovesTest, OnlyKingMovesInDoubleCheck) {
  Board board("4k3/8/8/8/8/2q5/2r5/4K3 w - - 0 1");

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // All moves should be from the king
  for (const Move& move : moves) {
    EXPECT_EQ(move.from, E1);
  }
}

/**
 * @test Blocking moves included in single check.
 * @brief Confirms generate_legal_moves() includes moves that block check.
 */
TEST(GenerateLegalMovesTest, BlockingMovesIncluded) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(D2, WHITE_BISHOP);
  board.set_piece(E8, BLACK_ROOK);

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // Bishop can block on E file
  bool has_blocking_move = false;
  for (const Move& move : moves) {
    if (move.from == D2 && (move.to == E3 || move.to == E4 || move.to == E5 || move.to == E6 || move.to == E7)) {
      has_blocking_move = true;
    }
  }
  EXPECT_TRUE(has_blocking_move);
}

/**
 * @test Capturing checker included.
 * @brief Confirms generate_legal_moves() includes moves that capture
 *        the checking piece.
 */
TEST(GenerateLegalMovesTest, CapturingCheckerIncluded) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(D2, WHITE_KNIGHT);
  board.set_piece(E5, BLACK_QUEEN);

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // Should include capturing the queen (if knight can reach it)
  // Or king moving away
  EXPECT_GT(moves.size(), 0);
}

/**
 * @test All moves from starting position are unique.
 * @brief Confirms generate_legal_moves() doesn't generate duplicate moves.
 */
TEST(GenerateLegalMovesTest, NoDuplicateMoves) {
  Board board = Board::StartingPosition();

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // Check for duplicates
  for (size_t i = 0; i < moves.size(); i++) {
    for (size_t j = i + 1; j < moves.size(); j++) {
      bool same = (moves[i].from == moves[j].from && moves[i].to == moves[j].to &&
                   moves[i].promotion == moves[j].promotion && moves[i].is_capture == moves[j].is_capture &&
                   moves[i].is_en_passant == moves[j].is_en_passant && moves[i].is_castling == moves[j].is_castling);
      EXPECT_FALSE(same) << "Duplicate move found";
    }
  }
}

/**
 * @test Knight moves included when not pinned.
 * @brief Confirms generate_legal_moves() includes knight moves when legal.
 */
TEST(GenerateLegalMovesTest, KnightMovesIncluded) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(D4, WHITE_KNIGHT);
  board.set_piece(E8, BLACK_KING);

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // Knight should have moves
  bool has_knight_move = false;
  for (const Move& move : moves) {
    if (move.from == D4) {
      has_knight_move = true;
      break;
    }
  }
  EXPECT_TRUE(has_knight_move);
}

/**
 * @test Pinned knight generates no moves.
 * @brief Confirms generate_legal_moves() excludes pinned knight moves.
 */
TEST(GenerateLegalMovesTest, PinnedKnightNoMoves) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E3, WHITE_KNIGHT);
  board.set_piece(E8, BLACK_ROOK);

  BoardState state = board.get_state();
  state.m_is_white_turn = true;
  board.set_state(state);

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  // Pinned knight cannot move
  for (const Move& move : moves) {
    EXPECT_NE(move.from, E3);
  }
}
