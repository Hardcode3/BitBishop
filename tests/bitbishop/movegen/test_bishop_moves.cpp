#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/movegen/bishop_moves.hpp>
#include <bitbishop/movegen/pins.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test Single bishop on empty board generates all moves.
 * @brief Confirms generate_bishop_legal_moves() produces 13 moves when
 *        bishop is in center with no restrictions.
 */
TEST(GenerateBishopLegalMovesTest, CenterBishopEmptyBoard) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_BISHOP);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 13);

  // Verify moves along NE diagonal
  EXPECT_TRUE(contains_move(moves, {E4, F5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, G6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, H7, std::nullopt, false, false, false}));

  // Verify moves along NW diagonal
  EXPECT_TRUE(contains_move(moves, {E4, D5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, C6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, B7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, A8, std::nullopt, false, false, false}));

  // Verify moves along SE diagonal
  EXPECT_TRUE(contains_move(moves, {E4, F3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, G2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, H1, std::nullopt, false, false, false}));

  // Verify moves along SW diagonal
  EXPECT_TRUE(contains_move(moves, {E4, D3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, C2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, B1, std::nullopt, false, false, false}));
}

/**
 * @test Bishop on corner generates moves.
 * @brief Confirms generate_bishop_legal_moves() produces 7 moves when
 *        bishop is on corner square.
 */
TEST(GenerateBishopLegalMovesTest, CornerBishopMoves) {
  Board board = Board::Empty();
  board.set_piece(A1, WHITE_BISHOP);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 7);

  // Verify moves along diagonal from A1 to H8
  EXPECT_TRUE(contains_move(moves, {A1, B2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A1, C3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A1, D4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A1, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A1, F6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A1, G7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A1, H8, std::nullopt, false, false, false}));
}

/**
 * @test Bishop blocked by friendly pieces.
 * @brief Confirms generate_bishop_legal_moves() stops at friendly pieces
 *        and excludes their squares.
 */
TEST(GenerateBishopLegalMovesTest, FriendlyPiecesBlock) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_BISHOP);
  board.set_piece(G6, WHITE_PAWN);    // Blocks NE
  board.set_piece(C6, WHITE_KNIGHT);  // Blocks NW
  board.set_piece(G2, WHITE_ROOK);    // Blocks SE
  board.set_piece(C2, WHITE_QUEEN);   // Blocks SW

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 4);

  // Verify moves stop before friendly pieces
  EXPECT_TRUE(contains_move(moves, {E4, F5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, D5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, D3, std::nullopt, false, false, false}));

  // Verify blocked squares not in moves
  EXPECT_FALSE(contains_move(moves, {E4, G6, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, C6, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, G2, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, C2, std::nullopt, false, false, false}));
}

/**
 * @test Bishop captures enemy pieces.
 * @brief Confirms generate_bishop_legal_moves() generates capture moves
 *        and stops at enemy pieces.
 */
TEST(GenerateBishopLegalMovesTest, BishopCapturesEnemyPieces) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_BISHOP);
  board.set_piece(G6, BLACK_PAWN);
  board.set_piece(C6, BLACK_KNIGHT);
  board.set_piece(G2, BLACK_ROOK);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Should include captures plus moves before blockers
  EXPECT_GT(moves.size(), 3);

  // Verify capture moves
  EXPECT_TRUE(contains_move(moves, {E4, G6, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, C6, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, G2, std::nullopt, true, false, false}));

  // Verify moves stop at enemy pieces (no x-ray)
  EXPECT_FALSE(contains_move(moves, {E4, H7, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, B7, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, H1, std::nullopt, false, false, false}));
}

/**
 * @test Check mask restricts bishop moves.
 * @brief Confirms generate_bishop_legal_moves() only generates moves within
 *        the check mask.
 */
TEST(GenerateBishopLegalMovesTest, CheckMaskRestriction) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_BISHOP);

  Bitboard check_mask = Bitboard::Zeros();
  check_mask.set(F5);
  check_mask.set(G6);
  check_mask.set(H7);

  std::vector<Move> moves;
  PinResult pins;

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 3);

  EXPECT_TRUE(contains_move(moves, {E4, F5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, G6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, H7, std::nullopt, false, false, false}));
}

/**
 * @test Empty check mask generates no moves.
 * @brief Confirms generate_bishop_legal_moves() generates no moves when
 *        check mask is empty (double check).
 */
TEST(GenerateBishopLegalMovesTest, EmptyCheckMaskNoMoves) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_BISHOP);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Zeros();
  PinResult pins;

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Pinned bishop moves along pin ray only.
 * @brief Confirms generate_bishop_legal_moves() restricts pinned bishop to
 *        moves along the pin ray.
 */
TEST(GenerateBishopLegalMovesTest, PinnedBishopAlongRay) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(F2, WHITE_BISHOP);
  board.set_piece(H4, BLACK_BISHOP);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Bishop can only move along NE-SW diagonal (pin ray)
  EXPECT_EQ(moves.size(), 2);

  EXPECT_TRUE(contains_move(moves, {F2, G3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {F2, H4, std::nullopt, true, false, false}));

  // Cannot move along other diagonal
  EXPECT_FALSE(contains_move(moves, {F2, E3, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {F2, G1, std::nullopt, false, false, false}));
}

/**
 * @test Pinned bishop can capture attacker.
 * @brief Confirms generate_bishop_legal_moves() allows pinned bishop to
 *        capture the pinning piece.
 */
TEST(GenerateBishopLegalMovesTest, PinnedBishopCapturesAttacker) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(F2, WHITE_BISHOP);
  board.set_piece(H4, BLACK_BISHOP);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Verify capture of pinning piece is allowed
  EXPECT_TRUE(contains_move(moves, {F2, H4, std::nullopt, true, false, false}));
}

/**
 * @test Orthogonally pinned bishop has no moves.
 * @brief Confirms generate_bishop_legal_moves() generates no moves when
 *        bishop is pinned orthogonally (perpendicular to its movement).
 */
TEST(GenerateBishopLegalMovesTest, OrthogonallyPinnedBishopNoMoves) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, WHITE_BISHOP);
  board.set_piece(E8, BLACK_ROOK);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Bishop pinned orthogonally cannot move (moves are diagonal)
  EXPECT_EQ(moves.size(), 0);
  EXPECT_TRUE(pins.pinned.test(E4));
}

/**
 * @test Multiple bishops generate moves independently.
 * @brief Confirms generate_bishop_legal_moves() generates moves from all
 *        bishops.
 */
TEST(GenerateBishopLegalMovesTest, MultipleBishops) {
  Board board = Board::Empty();
  board.set_piece(A1, WHITE_BISHOP);
  board.set_piece(H8, WHITE_BISHOP);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // A1 bishop: 6 moves along diagonal
  // H8 bishop: 6 moves along diagonal
  EXPECT_EQ(moves.size(), 12);

  // Verify moves from A1
  EXPECT_TRUE(contains_move(moves, {A1, B2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A1, C3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A1, D4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A1, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A1, F6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A1, G7, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {A1, H8, std::nullopt, false, false, false}));

  // Verify moves from H8
  EXPECT_TRUE(contains_move(moves, {H8, G7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {H8, F6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {H8, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {H8, D4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {H8, C3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {H8, B2, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {H8, A1, std::nullopt, false, false, false}));
}

/**
 * @test One bishop pinned, one bishop free.
 * @brief Confirms generate_bishop_legal_moves() handles mixed pinned and
 *        unpinned bishops correctly.
 */
TEST(GenerateBishopLegalMovesTest, OnePinnedOneFree) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(F2, WHITE_BISHOP);  // Pinned
  board.set_piece(D1, WHITE_BISHOP);  // Free
  board.set_piece(H4, BLACK_BISHOP);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // F2 bishop: 2 moves along pin ray
  // D1 bishop: 7 moves
  EXPECT_EQ(moves.size(), 9);

  // Verify pinned bishop restricted to diagonal
  EXPECT_TRUE(contains_move(moves, {F2, G3, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {F2, E3, std::nullopt, false, false, false}));

  // Verify free bishop moves normally
  EXPECT_TRUE(contains_move(moves, {D1, A4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {D1, H5, std::nullopt, false, false, false}));
}

/**
 * @test Black bishops generate moves correctly.
 * @brief Confirms generate_bishop_legal_moves() works correctly for
 *        black bishops.
 */
TEST(GenerateBishopLegalMovesTest, BlackBishopMoves) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_BISHOP);
  board.set_piece(D5, BLACK_PAWN);  // Friendly
  board.set_piece(F5, WHITE_PAWN);  // Enemy

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_bishop_legal_moves(moves, board, Color::BLACK, check_mask, pins);

  // Verify D5 is blocked (friendly)
  EXPECT_FALSE(contains_move(moves, {E4, D5, std::nullopt, false, false, false}));

  // Verify F5 is a capture
  EXPECT_TRUE(contains_move(moves, {E4, F5, std::nullopt, true, false, false}));

  // Verify other diagonals are available
  EXPECT_TRUE(contains_move(moves, {E4, D3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F3, std::nullopt, false, false, false}));
}

/**
 * @test Check mask allows capturing checker.
 * @brief Confirms generate_bishop_legal_moves() allows capturing checking
 *        piece when it's in the check mask.
 */
TEST(GenerateBishopLegalMovesTest, CaptureChecker) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_BISHOP);
  board.set_piece(H7, BLACK_QUEEN);  // Checking piece

  Bitboard check_mask = Bitboard::Zeros();
  check_mask.set(F5);
  check_mask.set(G6);
  check_mask.set(H7);  // Can capture checker

  std::vector<Move> moves;
  PinResult pins;

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 3);

  EXPECT_TRUE(contains_move(moves, {E4, H7, std::nullopt, true, false, false}));
}

/**
 * @test No bishops on board.
 * @brief Confirms generate_bishop_legal_moves() handles empty bishop
 *        bitboard gracefully.
 */
TEST(GenerateBishopLegalMovesTest, NoBishopsOnBoard) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Moves vector accumulates correctly.
 * @brief Confirms generate_bishop_legal_moves() appends to existing moves
 *        vector without clearing it.
 */
TEST(GenerateBishopLegalMovesTest, MovesVectorAccumulates) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_BISHOP);

  std::vector<Move> moves;
  // Add a dummy move first
  moves.emplace_back(A1, A2, std::nullopt, false, false, false);

  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Should have 1 dummy + 13 bishop moves = 14 total
  EXPECT_EQ(moves.size(), 14);
  EXPECT_TRUE(contains_move(moves, {A1, A2, std::nullopt, false, false, false}));
}

/**
 * @test Combined restrictions.
 * @brief Confirms generate_bishop_legal_moves() correctly applies pins,
 *        blockers, and check mask simultaneously.
 */
TEST(GenerateBishopLegalMovesTest, CombinedRestrictions) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(F2, WHITE_BISHOP);  // Pinned diagonally
  board.set_piece(H4, BLACK_BISHOP);  // Pins F2 bishop along diagonal
  board.set_piece(E8, BLACK_ROOK);    // Checking the king from E8
  board.set_piece(B5, WHITE_BISHOP);  // Threatens checking black rook

  // Check mask: must block or capture checker on E8
  // The check is coming from E8 along the E-file
  Bitboard check_mask = Bitboard::Zeros();
  check_mask.set(E2);
  check_mask.set(E3);
  check_mask.set(E4);
  check_mask.set(E5);
  check_mask.set(E6);
  check_mask.set(E7);
  check_mask.set(E8);

  std::vector<Move> moves;
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // F2 bishop is pinned along F2-G3-H4 diagonal
  // B5 bishop threatens the checking black rook on E8 and is able to defend from check in E2
  // Check mask requires moves to block/capture on E-file (E2-E8)
  // Pin ray is F2-G3-H4 (NE diagonal)
  // Intersection of check mask (E-file) and pin ray (diagonal) is empty
  // Expects a single capture move for B5 bishop, taking E8 black rook
  // Expects a single move without capture for B5 bishop to E2 square
  EXPECT_EQ(moves.size(), 2);
  EXPECT_TRUE(contains_move(moves, {B5, E2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {B5, E8, std::nullopt, true, false, false}));
}

/**
 * @test Move properties are correct.
 * @brief Confirms generate_bishop_legal_moves() sets all move properties
 *        correctly (no promotion, en passant, or castling).
 */
TEST(GenerateBishopLegalMovesTest, MovePropertiesCorrect) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_BISHOP);
  board.set_piece(G6, BLACK_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  for (const Move& move : moves) {
    EXPECT_EQ(move.from, E4);
    EXPECT_FALSE(move.promotion.has_value());
    EXPECT_FALSE(move.is_en_passant);
    EXPECT_FALSE(move.is_castling);
  }

  // Verify capture flag specifically
  EXPECT_TRUE(contains_move(moves, {E4, G6, std::nullopt, true, false, false}));
}

/**
 * @test Bishop blocked in all directions.
 * @brief Confirms generate_bishop_legal_moves() generates no moves when
 *        bishop is completely surrounded.
 */
TEST(GenerateBishopLegalMovesTest, BishopCompletelyBlocked) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_BISHOP);
  board.set_piece(F5, WHITE_PAWN);
  board.set_piece(D5, WHITE_PAWN);
  board.set_piece(F3, WHITE_PAWN);
  board.set_piece(D3, WHITE_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Bishop on edge of board.
 * @brief Confirms generate_bishop_legal_moves() correctly handles bishops
 *        on board edges with varying move counts.
 */
TEST(GenerateBishopLegalMovesTest, BishopOnEdge) {
  Board board = Board::Empty();
  board.set_piece(A4, WHITE_BISHOP);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_bishop_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Bishop on A4 has 7 moves (one diagonal only)
  EXPECT_EQ(moves.size(), 7);

  EXPECT_TRUE(contains_move(moves, {A4, B5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A4, C6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A4, D7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A4, B3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A4, C2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A4, D1, std::nullopt, false, false, false}));
}
