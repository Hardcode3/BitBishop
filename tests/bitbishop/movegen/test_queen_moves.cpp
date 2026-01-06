#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/movegen/pins.hpp>
#include <bitbishop/movegen/queen_moves.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test Single queen on empty board generates all moves.
 * @brief Confirms generate_queen_legal_moves() produces 27 moves when
 *        queen is in center with no restrictions.
 */
TEST(GenerateQueenLegalMovesTest, CenterQueenEmptyBoard) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_QUEEN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_queen_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 27);

  // Verify orthogonal moves (rank and file)
  EXPECT_TRUE(contains_move(moves, {E4, E1, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E8, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, A4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, H4, std::nullopt, false, false, false}));

  // Verify diagonal moves
  EXPECT_TRUE(contains_move(moves, {E4, A8, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, H7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, H1, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, B1, std::nullopt, false, false, false}));
}

/**
 * @test Queen on corner generates moves.
 * @brief Confirms generate_queen_legal_moves() produces 21 moves when
 *        queen is on corner square.
 */
TEST(GenerateQueenLegalMovesTest, CornerQueenMoves) {
  Board board = Board::Empty();
  board.set_piece(A1, WHITE_QUEEN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_queen_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 21);

  // Verify orthogonal moves
  EXPECT_TRUE(contains_move(moves, {A1, A8, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A1, H1, std::nullopt, false, false, false}));

  // Verify diagonal moves
  EXPECT_TRUE(contains_move(moves, {A1, H8, std::nullopt, false, false, false}));
}

/**
 * @test Queen blocked by friendly pieces.
 * @brief Confirms generate_queen_legal_moves() stops at friendly pieces
 *        and excludes their squares.
 */
TEST(GenerateQueenLegalMovesTest, FriendlyPiecesBlock) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_QUEEN);
  board.set_piece(E6, WHITE_PAWN);    // Blocks north
  board.set_piece(E2, WHITE_PAWN);    // Blocks south
  board.set_piece(G4, WHITE_KNIGHT);  // Blocks east
  board.set_piece(C4, WHITE_BISHOP);  // Blocks west
  board.set_piece(G6, WHITE_ROOK);    // Blocks NE
  board.set_piece(C6, WHITE_PAWN);    // Blocks NW
  board.set_piece(G2, WHITE_PAWN);    // Blocks SE
  board.set_piece(C2, WHITE_PAWN);    // Blocks SW

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_queen_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 8);

  // Verify moves stop before friendly pieces
  EXPECT_TRUE(contains_move(moves, {E4, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, D4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, D5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, D3, std::nullopt, false, false, false}));

  // Verify blocked squares not in moves
  EXPECT_FALSE(contains_move(moves, {E4, E6, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, G4, std::nullopt, false, false, false}));
}

/**
 * @test Queen captures enemy pieces.
 * @brief Confirms generate_queen_legal_moves() generates capture moves
 *        and stops at enemy pieces.
 */
TEST(GenerateQueenLegalMovesTest, QueenCapturesEnemyPieces) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_QUEEN);
  board.set_piece(E6, BLACK_PAWN);    // Orthogonal capture
  board.set_piece(G4, BLACK_KNIGHT);  // Orthogonal capture
  board.set_piece(G6, BLACK_BISHOP);  // Diagonal capture
  board.set_piece(C2, BLACK_ROOK);    // Diagonal capture

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_queen_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Should include captures plus moves before blockers
  EXPECT_EQ(moves.size(), 22);

  // Verify capture moves
  EXPECT_TRUE(contains_move(moves, {E4, E6, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, G4, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, G6, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, C2, std::nullopt, true, false, false}));

  // Verify moves stop at enemy pieces (no x-ray)
  EXPECT_FALSE(contains_move(moves, {E4, E7, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, H4, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, H7, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, B1, std::nullopt, false, false, false}));
}

/**
 * @test Check mask restricts queen moves.
 * @brief Confirms generate_queen_legal_moves() only generates moves within
 *        the check mask.
 */
TEST(GenerateQueenLegalMovesTest, CheckMaskRestriction) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_QUEEN);

  Bitboard check_mask = Bitboard::Zeros();
  check_mask.set(E5);
  check_mask.set(E6);
  check_mask.set(F5);

  std::vector<Move> moves;
  PinResult pins;

  generate_queen_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 3);

  EXPECT_TRUE(contains_move(moves, {E4, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F5, std::nullopt, false, false, false}));
}

/**
 * @test Empty check mask generates no moves.
 * @brief Confirms generate_queen_legal_moves() generates no moves when
 *        check mask is empty (double check).
 */
TEST(GenerateQueenLegalMovesTest, EmptyCheckMaskNoMoves) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_QUEEN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Zeros();
  PinResult pins;

  generate_queen_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Pinned queen moves along orthogonal pin ray.
 * @brief Confirms generate_queen_legal_moves() restricts pinned queen to
 *        moves along orthogonal pin ray.
 */
TEST(GenerateQueenLegalMovesTest, PinnedQueenOrthogonalRay) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, WHITE_QUEEN);
  board.set_piece(E8, BLACK_ROOK);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_queen_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Queen can only move along E-file (pin ray)
  EXPECT_EQ(moves.size(), 6);

  EXPECT_TRUE(contains_move(moves, {E4, E2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E8, std::nullopt, true, false, false}));

  // Cannot move along rank or diagonals
  EXPECT_FALSE(contains_move(moves, {E4, D4, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, F5, std::nullopt, false, false, false}));
}

/**
 * @test Pinned queen moves along diagonal pin ray.
 * @brief Confirms generate_queen_legal_moves() restricts pinned queen to
 *        moves along diagonal pin ray.
 */
TEST(GenerateQueenLegalMovesTest, PinnedQueenDiagonalRay) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(F2, WHITE_QUEEN);
  board.set_piece(H4, BLACK_BISHOP);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_queen_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Queen can only move along diagonal (pin ray)
  EXPECT_EQ(moves.size(), 2);

  EXPECT_TRUE(contains_move(moves, {F2, G3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {F2, H4, std::nullopt, true, false, false}));

  // Cannot move along orthogonals or other diagonals
  EXPECT_FALSE(contains_move(moves, {F2, F3, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {F2, E2, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {F2, G1, std::nullopt, false, false, false}));
}

/**
 * @test Multiple queens generate moves independently.
 * @brief Confirms generate_queen_legal_moves() generates moves from all
 *        queens.
 */
TEST(GenerateQueenLegalMovesTest, MultipleQueens) {
  Board board = Board::Empty();
  board.set_piece(D1, WHITE_QUEEN);
  board.set_piece(D8, WHITE_QUEEN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_queen_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Each queen has 20 moves
  EXPECT_EQ(moves.size(), 40);

  // Verify moves from both queens exist
  EXPECT_TRUE(contains_move(moves, {D1, D7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {D8, D2, std::nullopt, false, false, false}));
}

/**
 * @test One queen pinned, one queen free.
 * @brief Confirms generate_queen_legal_moves() handles mixed pinned and
 *        unpinned queens correctly.
 */
TEST(GenerateQueenLegalMovesTest, OnePinnedOneFree) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, WHITE_QUEEN);  // Pinned
  board.set_piece(D1, WHITE_QUEEN);  // Free
  board.set_piece(E8, BLACK_ROOK);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_queen_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // E4 queen: 6 moves along pin ray (including one capture)
  // D1 queen: 17 moves (blocked by king on E1)
  EXPECT_EQ(moves.size(), 23);

  // Verify pinned queen restricted to file
  EXPECT_TRUE(contains_move(moves, {E4, E2, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, D4, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, F4, std::nullopt, false, false, false}));

  // Verify free queen moves normally
  EXPECT_TRUE(contains_move(moves, {D1, D8, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {D1, A1, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {D1, G1, std::nullopt, false, false, false}));
}

/**
 * @test Black queens generate moves correctly.
 * @brief Confirms generate_queen_legal_moves() works correctly for
 *        black queens.
 */
TEST(GenerateQueenLegalMovesTest, BlackQueenMoves) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_QUEEN);
  board.set_piece(E2, BLACK_PAWN);    // Friendly
  board.set_piece(E6, WHITE_PAWN);    // Enemy
  board.set_piece(G6, WHITE_KNIGHT);  // Enemy

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_queen_legal_moves(moves, board, Color::BLACK, check_mask, pins);

  // Verify E2 is blocked (friendly)
  EXPECT_FALSE(contains_move(moves, {E4, E2, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, E1, std::nullopt, false, false, false}));

  // Verify captures
  EXPECT_TRUE(contains_move(moves, {E4, E6, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, G6, std::nullopt, true, false, false}));

  EXPECT_TRUE(contains_move(moves, {E4, E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, D3, std::nullopt, false, false, false}));
}

/**
 * @test Check mask allows capturing checker.
 * @brief Confirms generate_queen_legal_moves() allows capturing checking
 *        piece when it's in the check mask.
 */
TEST(GenerateQueenLegalMovesTest, CaptureChecker) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_QUEEN);
  board.set_piece(E8, BLACK_ROOK);  // Checking piece

  Bitboard check_mask = Bitboard::Zeros();
  check_mask.set(E5);
  check_mask.set(E6);
  check_mask.set(E7);
  check_mask.set(E8);  // Can capture checker

  std::vector<Move> moves;
  PinResult pins;

  generate_queen_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 4);

  EXPECT_TRUE(contains_move(moves, {E4, E8, std::nullopt, true, false, false}));
}

/**
 * @test No queens on board.
 * @brief Confirms generate_queen_legal_moves() handles empty queen
 *        bitboard gracefully.
 */
TEST(GenerateQueenLegalMovesTest, NoQueensOnBoard) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_queen_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Moves vector accumulates correctly.
 * @brief Confirms generate_queen_legal_moves() appends to existing moves
 *        vector without clearing it.
 */
TEST(GenerateQueenLegalMovesTest, MovesVectorAccumulates) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_QUEEN);

  std::vector<Move> moves;
  // Add a dummy move first
  moves.emplace_back(Move::make(A1, A2));

  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_queen_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Should have 1 dummy + 27 queen moves = 28 total
  EXPECT_EQ(moves.size(), 28);
  EXPECT_TRUE(contains_move(moves, Move::make(A1, A2)));
}

/**
 * @test Combined restrictions.
 * @brief Confirms generate_queen_legal_moves() correctly applies pins,
 *        blockers, and check mask simultaneously.
 */
TEST(GenerateQueenLegalMovesTest, CombinedRestrictions) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, WHITE_QUEEN);  // Pinned by rook on E8
  board.set_piece(E8, BLACK_ROOK);   // Pins queen
  board.set_piece(A1, BLACK_QUEEN);  // Checking the king from A1

  // Check mask: must block or capture checker queen on A1
  // Diagonal from E1 to A1 is blocked, but check is along diagonal
  Bitboard check_mask = Bitboard::Zeros();
  check_mask.set(B2);
  check_mask.set(C3);
  check_mask.set(D4);

  std::vector<Move> moves;
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_queen_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Queen pinned to E-file
  // Pin ray: E2, E3, E4, E5, E6, E7, E8
  // Check mask: B2, C3, D4
  // Intersection: none
  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Move properties are correct.
 * @brief Confirms generate_queen_legal_moves() sets all move properties
 *        correctly (no promotion, en passant, or castling).
 */
TEST(GenerateQueenLegalMovesTest, MovePropertiesCorrect) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_QUEEN);
  board.set_piece(E6, BLACK_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_queen_legal_moves(moves, board, Color::WHITE, check_mask, pins);

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
 * @test Queen blocked in all directions.
 * @brief Confirms generate_queen_legal_moves() generates no moves when
 *        queen is completely surrounded.
 */
TEST(GenerateQueenLegalMovesTest, QueenCompletelyBlocked) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_QUEEN);
  board.set_piece(E5, WHITE_PAWN);
  board.set_piece(E3, WHITE_PAWN);
  board.set_piece(F4, WHITE_PAWN);
  board.set_piece(D4, WHITE_PAWN);
  board.set_piece(F5, WHITE_PAWN);
  board.set_piece(D5, WHITE_PAWN);
  board.set_piece(F3, WHITE_PAWN);
  board.set_piece(D3, WHITE_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_queen_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Queen on edge of board.
 * @brief Confirms generate_queen_legal_moves() correctly handles queens
 *        on board edges.
 */
TEST(GenerateQueenLegalMovesTest, QueenOnEdge) {
  Board board = Board::Empty();
  board.set_piece(A4, WHITE_QUEEN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_queen_legal_moves(moves, board, Color::WHITE, check_mask, pins);

  // Queen on edge has 21 moves
  EXPECT_EQ(moves.size(), 21);

  EXPECT_TRUE(contains_move(moves, {A4, A1, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A4, A8, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A4, H4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A4, D7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A4, D1, std::nullopt, false, false, false}));
}
