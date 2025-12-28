#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/movegen/knight_moves.hpp>
#include <bitbishop/movegen/pins.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test Single knight on empty board generates all moves.
 * @brief Confirms generate_knight_legal_moves() produces 8 moves when
 *        knight is in center with no restrictions.
 */
TEST(GenerateKnightLegalMovesTest, CenterKnightEmptyBoard) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KNIGHT);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_knight_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 8);

  // Verify all 8 knight moves from E4
  EXPECT_TRUE(contains_move(moves, {E4, D2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, C3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, G3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, C5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, G5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, D6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F6, std::nullopt, false, false, false}));
}

/**
 * @test Knight on corner generates limited moves.
 * @brief Confirms generate_knight_legal_moves() produces only 2 moves when
 *        knight is on corner square.
 */
TEST(GenerateKnightLegalMovesTest, CornerKnightLimitedMoves) {
  Board board = Board::Empty();
  board.set_piece(A1, WHITE_KNIGHT);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_knight_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 2);

  EXPECT_TRUE(contains_move(moves, {A1, B3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A1, C2, std::nullopt, false, false, false}));
}

/**
 * @test Knight on edge generates limited moves.
 * @brief Confirms generate_knight_legal_moves() produces only 4 moves when
 *        knight is on edge square.
 */
TEST(GenerateKnightLegalMovesTest, EdgeKnightLimitedMoves) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KNIGHT);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_knight_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 4);

  EXPECT_TRUE(contains_move(moves, {E1, D3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E1, F3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E1, C2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E1, G2, std::nullopt, false, false, false}));
}

/**
 * @test Friendly pieces block knight moves.
 * @brief Confirms generate_knight_legal_moves() excludes squares occupied
 *        by friendly pieces.
 */
TEST(GenerateKnightLegalMovesTest, FriendlyPiecesBlocked) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KNIGHT);
  board.set_piece(D2, WHITE_PAWN);
  board.set_piece(C3, WHITE_BISHOP);
  board.set_piece(G5, WHITE_ROOK);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_knight_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 5);

  // Verify blocked squares are not in moves
  EXPECT_FALSE(contains_move(moves, {E4, D2, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, C3, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, G5, std::nullopt, false, false, false}));

  // Verify remaining moves are present
  EXPECT_TRUE(contains_move(moves, {E4, F2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, G3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, C5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, D6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F6, std::nullopt, false, false, false}));
}

/**
 * @test Knight captures enemy pieces.
 * @brief Confirms generate_knight_legal_moves() generates capture moves
 *        and marks them correctly.
 */
TEST(GenerateKnightLegalMovesTest, KnightCapturesEnemyPieces) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KNIGHT);
  board.set_piece(D2, BLACK_PAWN);
  board.set_piece(C3, BLACK_BISHOP);
  board.set_piece(G5, BLACK_ROOK);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_knight_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 8);

  // Verify capture moves
  EXPECT_TRUE(contains_move(moves, {E4, D2, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, C3, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, G5, std::nullopt, true, false, false}));

  // Verify non-capture moves
  EXPECT_TRUE(contains_move(moves, {E4, F2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, G3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, C5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, D6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F6, std::nullopt, false, false, false}));
}

/**
 * @test Check mask restricts knight moves.
 * @brief Confirms generate_knight_legal_moves() only generates moves within
 *        the check mask.
 */
TEST(GenerateKnightLegalMovesTest, CheckMaskRestriction) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KNIGHT);

  Bitboard check_mask = Bitboard::Zeros();
  check_mask.set(D2);
  check_mask.set(C3);

  std::vector<Move> moves;
  PinResult pins;

  generate_knight_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 2);

  EXPECT_TRUE(contains_move(moves, {E4, D2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, C3, std::nullopt, false, false, false}));
}

/**
 * @test Empty check mask generates no moves.
 * @brief Confirms generate_knight_legal_moves() generates no moves when
 *        check mask is empty (double check).
 */
TEST(GenerateKnightLegalMovesTest, EmptyCheckMaskNoMoves) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KNIGHT);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Zeros();
  PinResult pins;

  generate_knight_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Pinned knight generates no moves.
 * @brief Confirms generate_knight_legal_moves() excludes pinned knights
 *        (knights cannot move along pin ray).
 */
TEST(GenerateKnightLegalMovesTest, PinnedKnightNoMoves) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E3, WHITE_KNIGHT);
  board.set_piece(E8, BLACK_ROOK);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_knight_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
  EXPECT_TRUE(pins.pinned.test(E3));
}

/**
 * @test Multiple knights generate moves independently.
 * @brief Confirms generate_knight_legal_moves() generates moves from all
 *        unpinned knights.
 */
TEST(GenerateKnightLegalMovesTest, MultipleKnights) {
  Board board = Board::Empty();
  board.set_piece(B1, WHITE_KNIGHT);
  board.set_piece(G1, WHITE_KNIGHT);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_knight_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // B1 knight has 3 moves (A3, C3, D2)
  // G1 knight has 3 moves (E2, F3, H3)
  EXPECT_EQ(moves.size(), 6);

  // Verify moves from B1
  EXPECT_TRUE(contains_move(moves, {B1, A3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {B1, C3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {B1, D2, std::nullopt, false, false, false}));

  // Verify moves from G1
  EXPECT_TRUE(contains_move(moves, {G1, E2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {G1, F3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {G1, H3, std::nullopt, false, false, false}));
}

/**
 * @test One knight pinned, one knight free.
 * @brief Confirms generate_knight_legal_moves() only generates moves from
 *        unpinned knights when some knights are pinned.
 */
TEST(GenerateKnightLegalMovesTest, OnePinnedOneFree) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E3, WHITE_KNIGHT);  // Pinned
  board.set_piece(C3, WHITE_KNIGHT);  // Free
  board.set_piece(E8, BLACK_ROOK);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_knight_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Only C3 knight should generate moves
  EXPECT_GT(moves.size(), 0);

  // Verify no moves from E3
  EXPECT_EQ(count_moves_to(moves, E3), 0);

  // Verify moves from C3
  EXPECT_TRUE(contains_move(moves, {C3, A2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {C3, A4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {C3, B1, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {C3, D1, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {C3, B5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {C3, D5, std::nullopt, false, false, false}));
}

/**
 * @test Black knights generate moves correctly.
 * @brief Confirms generate_knight_legal_moves() works correctly for
 *        black knights.
 */
TEST(GenerateKnightLegalMovesTest, BlackKnightMoves) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_KNIGHT);
  board.set_piece(D2, BLACK_PAWN);  // Friendly
  board.set_piece(C3, WHITE_PAWN);  // Enemy

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_knight_legal_moves(moves, board, Color::BLACK, check_mask, pins);

  EXPECT_EQ(moves.size(), 7);

  // Verify D2 is blocked (friendly)
  EXPECT_FALSE(contains_move(moves, {E4, D2, std::nullopt, false, false, false}));

  // Verify C3 is a capture
  EXPECT_TRUE(contains_move(moves, {E4, C3, std::nullopt, true, false, false}));
}

/**
 * @test Check mask with captures.
 * @brief Confirms generate_knight_legal_moves() allows capturing checker
 *        when it's in the check mask.
 */
TEST(GenerateKnightLegalMovesTest, CheckMaskWithCapture) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KNIGHT);
  board.set_piece(D2, BLACK_QUEEN);  // Checking piece

  Bitboard check_mask = Bitboard::Zeros();
  check_mask.set(D2);  // Must capture or block checker

  std::vector<Move> moves;
  PinResult pins;

  generate_knight_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 1);

  EXPECT_TRUE(contains_move(moves, {E4, D2, std::nullopt, true, false, false}));
}

/**
 * @test No knights on board.
 * @brief Confirms generate_knight_legal_moves() handles empty knight
 *        bitboard gracefully.
 */
TEST(GenerateKnightLegalMovesTest, NoKnightsOnBoard) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_knight_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test All knights pinned.
 * @brief Confirms generate_knight_legal_moves() generates no moves when
 *        all knights are pinned.
 */
TEST(GenerateKnightLegalMovesTest, AllKnightsPinned) {
  Board board = Board::Empty();
  board.set_piece(E3, WHITE_KING);
  board.set_piece(E5, WHITE_KNIGHT);
  board.set_piece(E8, BLACK_ROOK);
  board.set_piece(C3, WHITE_KNIGHT);
  board.set_piece(A3, BLACK_QUEEN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E3, board, Color::WHITE);

  generate_knight_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
  EXPECT_TRUE(pins.pinned.test(C3));
  EXPECT_TRUE(pins.pinned.test(E5));
}

/**
 * @test Moves vector accumulates correctly.
 * @brief Confirms generate_knight_legal_moves() appends to existing moves
 *        vector without clearing it.
 */
TEST(GenerateKnightLegalMovesTest, MovesVectorAccumulates) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KNIGHT);

  std::vector<Move> moves;
  // Add a dummy move first
  moves.emplace_back(A1, A2, std::nullopt, false, false, false);

  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_knight_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Should have 1 dummy + 8 knight moves = 9 total
  EXPECT_EQ(moves.size(), 9);
  EXPECT_TRUE(contains_move(moves, {A1, A2, std::nullopt, false, false, false}));
}

/**
 * @test Combined restrictions.
 * @brief Confirms generate_knight_legal_moves() correctly applies pins,
 *        friendly pieces, and check mask simultaneously.
 */
TEST(GenerateKnightLegalMovesTest, CombinedRestrictions) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E3, WHITE_KNIGHT);  // Pinned by rook on E8
  board.set_piece(D4, WHITE_KNIGHT);  // Free knight
  board.set_piece(E8, BLACK_ROOK);
  board.set_piece(C2, WHITE_PAWN);  // Blocks D4 knight

  Bitboard check_mask = Bitboard::Zeros();
  check_mask.set(E6);  // Must block check
  check_mask.set(E7);
  check_mask.set(E8);

  std::vector<Move> moves;
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_knight_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // E3 is pinned (no moves)
  // D4 can move to E6 (in check mask)
  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {D4, E6, std::nullopt, false, false, false}));
}

/**
 * @test Move properties are correct.
 * @brief Confirms generate_knight_legal_moves() sets all move properties
 *        correctly (no promotion, en passant, or castling).
 */
TEST(GenerateKnightLegalMovesTest, MovePropertiesCorrect) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KNIGHT);
  board.set_piece(D2, BLACK_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_knight_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  for (const Move& move : moves) {
    EXPECT_EQ(move.from, E4);
    EXPECT_FALSE(move.promotion.has_value());
    EXPECT_FALSE(move.is_en_passant);
    EXPECT_FALSE(move.is_castling);
  }

  // Verify capture flag specifically
  EXPECT_TRUE(contains_move(moves, {E4, D2, std::nullopt, true, false, false}));
}

/**
 * @test Knight near board edge.
 * @brief Confirms generate_knight_legal_moves() correctly handles knights
 *        near board edges with varying move counts.
 */
TEST(GenerateKnightLegalMovesTest, KnightNearEdge) {
  Board board = Board::Empty();
  board.set_piece(B2, WHITE_KNIGHT);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_knight_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 4);

  EXPECT_TRUE(contains_move(moves, {B2, A4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {B2, C4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {B2, D3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {B2, D1, std::nullopt, false, false, false}));
}
