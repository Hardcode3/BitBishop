#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/movegen/pins.hpp>
#include <bitbishop/movegen/rook_moves.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test Single rook on empty board generates all moves.
 * @brief Confirms generate_rook_legal_moves() produces 14 moves when
 *        rook is in center with no restrictions.
 */
TEST(GenerateRookLegalMovesTest, CenterRookEmptyBoard) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_ROOK);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_rook_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 14);

  // Verify moves along file (north and south)
  EXPECT_TRUE(contains_move(moves, {E4, E1, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E8, std::nullopt, false, false, false}));

  // Verify moves along rank (east and west)
  EXPECT_TRUE(contains_move(moves, {E4, A4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, B4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, C4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, D4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, G4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, H4, std::nullopt, false, false, false}));
}

/**
 * @test Rook on corner generates moves.
 * @brief Confirms generate_rook_legal_moves() produces 14 moves when
 *        rook is on corner square.
 */
TEST(GenerateRookLegalMovesTest, CornerRookMoves) {
  Board board = Board::Empty();
  board.set_piece(A1, WHITE_ROOK);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_rook_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 14);

  // Verify moves along file
  EXPECT_TRUE(contains_move(moves, {A1, A8, std::nullopt, false, false, false}));

  // Verify moves along rank
  EXPECT_TRUE(contains_move(moves, {A1, H1, std::nullopt, false, false, false}));
}

/**
 * @test Rook blocked by friendly pieces.
 * @brief Confirms generate_rook_legal_moves() stops at friendly pieces
 *        and excludes their squares.
 */
TEST(GenerateRookLegalMovesTest, FriendlyPiecesBlock) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_ROOK);
  board.set_piece(E6, WHITE_PAWN);    // Blocks north
  board.set_piece(E2, WHITE_PAWN);    // Blocks south
  board.set_piece(G4, WHITE_KNIGHT);  // Blocks east
  board.set_piece(C4, WHITE_BISHOP);  // Blocks west

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_rook_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 4);

  // Verify moves stop before friendly pieces
  EXPECT_TRUE(contains_move(moves, {E4, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, D4, std::nullopt, false, false, false}));

  // Verify blocked squares not in moves
  EXPECT_FALSE(contains_move(moves, {E4, E6, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, E2, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, G4, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, C4, std::nullopt, false, false, false}));
}

/**
 * @test Rook captures enemy pieces.
 * @brief Confirms generate_rook_legal_moves() generates capture moves
 *        and stops at enemy pieces.
 */
TEST(GenerateRookLegalMovesTest, RookCapturesEnemyPieces) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_ROOK);
  board.set_piece(E6, BLACK_PAWN);
  board.set_piece(E2, BLACK_KNIGHT);
  board.set_piece(G4, BLACK_BISHOP);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_rook_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Should include captures plus moves before blockers
  EXPECT_GT(moves.size(), 3);

  // Verify capture moves
  EXPECT_TRUE(contains_move(moves, {E4, E6, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E2, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, G4, std::nullopt, true, false, false}));

  // Verify moves stop at enemy pieces (no x-ray)
  EXPECT_FALSE(contains_move(moves, {E4, E7, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, E1, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, H4, std::nullopt, false, false, false}));
}

/**
 * @test Check mask restricts rook moves.
 * @brief Confirms generate_rook_legal_moves() only generates moves within
 *        the check mask.
 */
TEST(GenerateRookLegalMovesTest, CheckMaskRestriction) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_ROOK);

  Bitboard check_mask = Bitboard::Zeros();
  check_mask.set(E5);
  check_mask.set(E6);
  check_mask.set(E7);

  std::vector<Move> moves;
  PinResult pins;

  generate_rook_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 3);

  EXPECT_TRUE(contains_move(moves, {E4, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E7, std::nullopt, false, false, false}));
}

/**
 * @test Empty check mask generates no moves.
 * @brief Confirms generate_rook_legal_moves() generates no moves when
 *        check mask is empty (double check).
 */
TEST(GenerateRookLegalMovesTest, EmptyCheckMaskNoMoves) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_ROOK);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Zeros();
  PinResult pins;

  generate_rook_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Pinned rook moves along pin ray only.
 * @brief Confirms generate_rook_legal_moves() restricts pinned rook to
 *        moves along the pin ray.
 */
TEST(GenerateRookLegalMovesTest, PinnedRookAlongRay) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, WHITE_ROOK);
  board.set_piece(E8, BLACK_ROOK);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_rook_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Rook can only move along E-file (pin ray)
  EXPECT_EQ(moves.size(), 6);

  EXPECT_TRUE(contains_move(moves, {E4, E2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E8, std::nullopt, true, false, false}));

  // Cannot move along rank
  EXPECT_FALSE(contains_move(moves, {E4, D4, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, F4, std::nullopt, false, false, false}));
}

/**
 * @test Pinned rook can capture attacker.
 * @brief Confirms generate_rook_legal_moves() allows pinned rook to
 *        capture the pinning piece.
 */
TEST(GenerateRookLegalMovesTest, PinnedRookCapturesAttacker) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, WHITE_ROOK);
  board.set_piece(E8, BLACK_ROOK);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_rook_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Verify capture of pinning piece is allowed
  EXPECT_TRUE(contains_move(moves, {E4, E8, std::nullopt, true, false, false}));
}

/**
 * @test Diagonally pinned rook has no moves.
 * @brief Confirms generate_rook_legal_moves() generates no moves when
 *        rook is pinned diagonally (perpendicular to its movement).
 */
TEST(GenerateRookLegalMovesTest, DiagonallyPinnedRookNoMoves) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(F2, WHITE_ROOK);
  board.set_piece(H4, BLACK_BISHOP);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_rook_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Rook pinned diagonally cannot move (moves are orthogonal)
  EXPECT_EQ(moves.size(), 0);
  EXPECT_TRUE(pins.pinned.test(F2));
}

/**
 * @test Multiple rooks generate moves independently.
 * @brief Confirms generate_rook_legal_moves() generates moves from all
 *        rooks.
 */
TEST(GenerateRookLegalMovesTest, MultipleRooks) {
  Board board = Board::Empty();
  board.set_piece(A1, WHITE_ROOK);
  board.set_piece(H8, WHITE_ROOK);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_rook_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Each rook has 14 moves = 28 total
  EXPECT_EQ(moves.size(), 28);

  // Verify moves from A1
  EXPECT_TRUE(contains_move(moves, {A1, A8, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A1, H1, std::nullopt, false, false, false}));

  // Verify moves from H8
  EXPECT_TRUE(contains_move(moves, {H8, H1, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {H8, A8, std::nullopt, false, false, false}));
}

/**
 * @test One rook pinned, one rook free.
 * @brief Confirms generate_rook_legal_moves() handles mixed pinned and
 *        unpinned rooks correctly.
 */
TEST(GenerateRookLegalMovesTest, OnePinnedOneFree) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, WHITE_ROOK);  // Pinned
  board.set_piece(D1, WHITE_ROOK);  // Free
  board.set_piece(E8, BLACK_ROOK);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_rook_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // E4 rook: 6 moves along pin ray (including one capture)
  // D1 rook: 10 moves (blocked by king on E1)
  EXPECT_EQ(moves.size(), 16);

  // Verify pinned rook restricted to file
  EXPECT_TRUE(contains_move(moves, {E4, E2, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, D4, std::nullopt, false, false, false}));

  // Verify pinned rook can move towards the black pinning rook
  // and eventually capture it
  EXPECT_TRUE(contains_move(moves, {E4, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E8, std::nullopt, true, false, false}));

  // Verify free rook moves normally
  EXPECT_TRUE(contains_move(moves, {D1, D8, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {D1, A1, std::nullopt, false, false, false}));
}

/**
 * @test Black rooks generate moves correctly.
 * @brief Confirms generate_rook_legal_moves() works correctly for
 *        black rooks.
 */
TEST(GenerateRookLegalMovesTest, BlackRookMoves) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_ROOK);
  board.set_piece(E2, BLACK_PAWN);  // Friendly
  board.set_piece(E6, WHITE_PAWN);  // Enemy

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_rook_legal_moves(moves, board, Color::BLACK, check_mask, pins);

  // Verify E2 is blocked (friendly)
  EXPECT_FALSE(contains_move(moves, {E4, E2, std::nullopt, false, false, false}));

  // Verify E6 is a capture
  EXPECT_TRUE(contains_move(moves, {E4, E6, std::nullopt, true, false, false}));

  // Verify E3 is available
  EXPECT_TRUE(contains_move(moves, {E4, E3, std::nullopt, false, false, false}));
}

/**
 * @test Check mask allows capturing checker.
 * @brief Confirms generate_rook_legal_moves() allows capturing checking
 *        piece when it's in the check mask.
 */
TEST(GenerateRookLegalMovesTest, CaptureChecker) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_ROOK);
  board.set_piece(E8, BLACK_QUEEN);  // Checking piece

  Bitboard check_mask = Bitboard::Zeros();
  check_mask.set(E5);
  check_mask.set(E6);
  check_mask.set(E7);
  check_mask.set(E8);  // Can capture checker

  std::vector<Move> moves;
  PinResult pins;

  generate_rook_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 4);

  EXPECT_TRUE(contains_move(moves, {E4, E8, std::nullopt, true, false, false}));
}

/**
 * @test No rooks on board.
 * @brief Confirms generate_rook_legal_moves() handles empty rook
 *        bitboard gracefully.
 */
TEST(GenerateRookLegalMovesTest, NoRooksOnBoard) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_rook_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Moves vector accumulates correctly.
 * @brief Confirms generate_rook_legal_moves() appends to existing moves
 *        vector without clearing it.
 */
TEST(GenerateRookLegalMovesTest, MovesVectorAccumulates) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_ROOK);

  std::vector<Move> moves;
  // Add a dummy move first
  moves.emplace_back(Move::make(A1, A2));

  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_rook_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Should have 1 dummy + 14 rook moves = 15 total
  EXPECT_EQ(moves.size(), 15);
  EXPECT_TRUE(contains_move(moves, Move::make(A1, A2)));
}

/**
 * @test Combined restrictions.
 * @brief Confirms generate_rook_legal_moves() correctly applies pins,
 *        blockers, and check mask simultaneously.
 */
TEST(GenerateRookLegalMovesTest, CombinedRestrictions) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, WHITE_ROOK);
  board.set_piece(E3, WHITE_PAWN);  // Blocks south
  board.set_piece(E8, BLACK_ROOK);  // Pins rook

  Bitboard check_mask = Bitboard::Zeros();
  check_mask.set(E5);
  check_mask.set(E6);
  check_mask.set(E7);
  check_mask.set(E8);

  std::vector<Move> moves;
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_rook_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Pinned to E-file, blocked at E3, check mask allows E5-E8
  EXPECT_EQ(moves.size(), 4);

  EXPECT_TRUE(contains_move(moves, {E4, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E8, std::nullopt, true, false, false}));
}

/**
 * @test Move properties are correct.
 * @brief Confirms generate_rook_legal_moves() sets all move properties
 *        correctly (no promotion, en passant, or castling).
 */
TEST(GenerateRookLegalMovesTest, MovePropertiesCorrect) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_ROOK);
  board.set_piece(E6, BLACK_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_rook_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  for (const Move& move : moves) {
    EXPECT_EQ(move.from, E4);
    EXPECT_FALSE(move.promotion.has_value());
    EXPECT_FALSE(move.is_en_passant);
    EXPECT_FALSE(move.is_castling);
  }

  // Verify capture flag specifically
  EXPECT_TRUE(contains_move(moves, {E4, E6, std::nullopt, true, false, false}));
}

/**
 * @test Rook blocked in all directions.
 * @brief Confirms generate_rook_legal_moves() generates no moves when
 *        rook is completely surrounded.
 */
TEST(GenerateRookLegalMovesTest, RookCompletelyBlocked) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_ROOK);
  board.set_piece(E5, WHITE_PAWN);
  board.set_piece(E3, WHITE_PAWN);
  board.set_piece(F4, WHITE_PAWN);
  board.set_piece(D4, WHITE_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_rook_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
}
