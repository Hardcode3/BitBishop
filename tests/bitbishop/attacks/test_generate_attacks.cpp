#include <gtest/gtest.h>

#include <bitbishop/attacks/generate_attacks.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test Empty board produces no attacks.
 * @brief Confirms generate_attacks() returns empty bitboard when enemy
 *        has no pieces.
 */
TEST(GenerateAttacksTest, EmptyBoardNoAttacks) {
  Board board = Board::Empty();

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  EXPECT_EQ(attacks, Bitboard::Zeros());
  EXPECT_EQ(attacks.count(), 0);
}

/**
 * @test Single king generates attack squares.
 * @brief Confirms generate_attacks() returns king attack pattern for
 *        a lone king.
 */
TEST(GenerateAttacksTest, SingleKingAttacks) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_KING);

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  // King on E4 attacks 8 squares around it
  EXPECT_EQ(attacks.count(), 8);
  EXPECT_TRUE(attacks.test(D3));
  EXPECT_TRUE(attacks.test(D4));
  EXPECT_TRUE(attacks.test(D5));
  EXPECT_TRUE(attacks.test(E3));
  EXPECT_TRUE(attacks.test(E5));
  EXPECT_TRUE(attacks.test(F3));
  EXPECT_TRUE(attacks.test(F4));
  EXPECT_TRUE(attacks.test(F5));
  EXPECT_FALSE(attacks.test(E4));
}

/**
 * @test King on corner generates limited attacks.
 * @brief Confirms generate_attacks() returns correct attack pattern for
 *        king on corner square.
 */
TEST(GenerateAttacksTest, KingOnCornerAttacks) {
  Board board = Board::Empty();
  board.set_piece(A1, BLACK_KING);

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  EXPECT_EQ(attacks.count(), 3);
  EXPECT_TRUE(attacks.test(A2));
  EXPECT_TRUE(attacks.test(B1));
  EXPECT_TRUE(attacks.test(B2));
}

/**
 * @test Single knight generates attack squares.
 * @brief Confirms generate_attacks() returns knight attack pattern.
 */
TEST(GenerateAttacksTest, SingleKnightAttacks) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_KNIGHT);

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  // Knight on E4 attacks 8 squares
  EXPECT_EQ(attacks.count(), 8);
  EXPECT_TRUE(attacks.test(D2));
  EXPECT_TRUE(attacks.test(F2));
  EXPECT_TRUE(attacks.test(C3));
  EXPECT_TRUE(attacks.test(G3));
  EXPECT_TRUE(attacks.test(C5));
  EXPECT_TRUE(attacks.test(G5));
  EXPECT_TRUE(attacks.test(D6));
  EXPECT_TRUE(attacks.test(F6));
}

/**
 * @test Multiple knights accumulate attacks.
 * @brief Confirms generate_attacks() combines attack patterns from
 *        multiple knights.
 */
TEST(GenerateAttacksTest, MultipleKnightsAttacks) {
  Board board = Board::Empty();
  board.set_piece(B1, BLACK_KNIGHT);
  board.set_piece(G1, BLACK_KNIGHT);

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  // Should include attacks from both knights
  EXPECT_TRUE(attacks.test(A3));  // From B1
  EXPECT_TRUE(attacks.test(C3));  // From B1
  EXPECT_TRUE(attacks.test(D2));  // From B1
  EXPECT_TRUE(attacks.test(E2));  // From G1
  EXPECT_TRUE(attacks.test(F3));  // From G1
  EXPECT_TRUE(attacks.test(H3));  // From G1
}

/**
 * @test White pawn attacks upward.
 * @brief Confirms generate_attacks() returns correct attack pattern for
 *        white pawns (attacking toward higher ranks).
 */
TEST(GenerateAttacksTest, WhitePawnAttacks) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_PAWN);

  Bitboard attacks = generate_attacks(board, Color::WHITE);

  EXPECT_EQ(attacks.count(), 2);
  EXPECT_TRUE(attacks.test(D5));
  EXPECT_TRUE(attacks.test(F5));
  EXPECT_FALSE(attacks.test(E5));  // Pawns don't attack forward
}

/**
 * @test Black pawn attacks downward.
 * @brief Confirms generate_attacks() returns correct attack pattern for
 *        black pawns (attacking toward lower ranks).
 */
TEST(GenerateAttacksTest, BlackPawnAttacks) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_PAWN);

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  EXPECT_EQ(attacks.count(), 2);
  EXPECT_TRUE(attacks.test(D3));
  EXPECT_TRUE(attacks.test(F3));
  EXPECT_FALSE(attacks.test(E3));  // Pawns don't attack forward
}

/**
 * @test Multiple pawns accumulate attacks.
 * @brief Confirms generate_attacks() combines attack patterns from
 *        multiple pawns.
 */
TEST(GenerateAttacksTest, MultiplePawnsAttacks) {
  Board board = Board::Empty();
  board.set_piece(D4, BLACK_PAWN);
  board.set_piece(E4, BLACK_PAWN);
  board.set_piece(F4, BLACK_PAWN);

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  // Pawns attack diagonally down
  EXPECT_TRUE(attacks.test(C3));
  EXPECT_TRUE(attacks.test(D3));
  EXPECT_TRUE(attacks.test(E3));
  EXPECT_TRUE(attacks.test(F3));
  EXPECT_TRUE(attacks.test(G3));
}

/**
 * @test Pawn on edge has limited attacks.
 * @brief Confirms generate_attacks() returns correct attack pattern for
 *        pawns on board edges.
 */
TEST(GenerateAttacksTest, PawnOnEdgeAttacks) {
  Board board = Board::Empty();
  board.set_piece(A4, BLACK_PAWN);
  board.print();

  Bitboard attacks = generate_attacks(board, Color::BLACK);
  attacks.print();

  EXPECT_EQ(attacks.count(), 1);
  EXPECT_TRUE(attacks.test(B3));
  EXPECT_FALSE(attacks.test(A3));
}

/**
 * @test Single rook attacks on empty board.
 * @brief Confirms generate_attacks() returns full rook rays when
 *        board is empty.
 */
TEST(GenerateAttacksTest, SingleRookEmptyBoard) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_ROOK);

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  // Rook attacks entire rank and file (14 squares, excluding E4 itself)
  EXPECT_EQ(attacks.count(), 14);
  // Check file
  EXPECT_TRUE(attacks.test(E1));
  EXPECT_TRUE(attacks.test(E8));
  // Check rank
  EXPECT_TRUE(attacks.test(A4));
  EXPECT_TRUE(attacks.test(H4));
  EXPECT_FALSE(attacks.test(E4));
}

/**
 * @test Rook attacks blocked by pieces.
 * @brief Confirms generate_attacks() returns rook attacks limited by
 *        blockers on occupied board.
 */
TEST(GenerateAttacksTest, RookAttacksBlocked) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_ROOK);
  board.set_piece(E6, WHITE_PAWN);  // Blocks north
  board.set_piece(E2, WHITE_PAWN);  // Blocks south
  board.set_piece(G4, WHITE_PAWN);  // Blocks east
  board.set_piece(C4, WHITE_PAWN);  // Blocks west

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  // Rook attacks up to and including blockers
  EXPECT_TRUE(attacks.test(E5));
  EXPECT_TRUE(attacks.test(E6));
  EXPECT_FALSE(attacks.test(E7));  // Blocked by E6
  EXPECT_TRUE(attacks.test(E3));
  EXPECT_TRUE(attacks.test(E2));
  EXPECT_FALSE(attacks.test(E1));  // Blocked by E2
  EXPECT_TRUE(attacks.test(F4));
  EXPECT_TRUE(attacks.test(G4));
  EXPECT_FALSE(attacks.test(H4));  // Blocked by G4
  EXPECT_TRUE(attacks.test(D4));
  EXPECT_TRUE(attacks.test(C4));
  EXPECT_FALSE(attacks.test(B4));  // Blocked by C4
}

/**
 * @test Single bishop attacks on empty board.
 * @brief Confirms generate_attacks() returns full bishop diagonals when
 *        board is empty.
 */
TEST(GenerateAttacksTest, SingleBishopEmptyBoard) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_BISHOP);

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  // Bishop attacks both diagonals (13 squares)
  EXPECT_EQ(attacks.count(), 13);
  EXPECT_TRUE(attacks.test(A8));  // NW diagonal
  EXPECT_TRUE(attacks.test(H1));  // SE diagonal
  EXPECT_TRUE(attacks.test(H7));  // NE diagonal
  EXPECT_TRUE(attacks.test(B1));  // SW diagonal
}

/**
 * @test Bishop attacks blocked by pieces.
 * @brief Confirms generate_attacks() returns bishop attacks limited by
 *        blockers on occupied board.
 */
TEST(GenerateAttacksTest, BishopAttacksBlocked) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_BISHOP);
  board.set_piece(G6, WHITE_PAWN);  // Blocks NE
  board.set_piece(C6, WHITE_PAWN);  // Blocks NW
  board.set_piece(G2, WHITE_PAWN);  // Blocks SE
  board.set_piece(C2, WHITE_PAWN);  // Blocks SW

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  // Bishop attacks up to and including blockers
  EXPECT_TRUE(attacks.test(F5));
  EXPECT_TRUE(attacks.test(G6));
  EXPECT_FALSE(attacks.test(H7));  // Blocked by G6
  EXPECT_TRUE(attacks.test(D5));
  EXPECT_TRUE(attacks.test(C6));
  EXPECT_FALSE(attacks.test(B7));  // Blocked by C6
}

/**
 * @test Single queen attacks on empty board.
 * @brief Confirms generate_attacks() returns combined rook and bishop
 *        attacks for queen on empty board.
 */
TEST(GenerateAttacksTest, SingleQueenEmptyBoard) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_QUEEN);

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  // Queen attacks in all 8 directions (27 squares)
  EXPECT_EQ(attacks.count(), 27);
  // Orthogonal
  EXPECT_TRUE(attacks.test(E1));
  EXPECT_TRUE(attacks.test(E8));
  EXPECT_TRUE(attacks.test(A4));
  EXPECT_TRUE(attacks.test(H4));
  // Diagonal
  EXPECT_TRUE(attacks.test(A8));
  EXPECT_TRUE(attacks.test(H7));
  EXPECT_TRUE(attacks.test(H1));
  EXPECT_TRUE(attacks.test(B1));
}

/**
 * @test Queen attacks blocked by pieces.
 * @brief Confirms generate_attacks() returns queen attacks limited by
 *        blockers on occupied board.
 */
TEST(GenerateAttacksTest, QueenAttacksBlocked) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_QUEEN);
  board.set_piece(E6, WHITE_PAWN);  // Blocks north
  board.set_piece(G6, WHITE_PAWN);  // Blocks NE diagonal

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  EXPECT_TRUE(attacks.test(E5));
  EXPECT_TRUE(attacks.test(E6));
  EXPECT_FALSE(attacks.test(E7));  // Blocked orthogonally
  EXPECT_TRUE(attacks.test(F5));
  EXPECT_TRUE(attacks.test(G6));
  EXPECT_FALSE(attacks.test(H7));  // Blocked diagonally
}

/**
 * @test Multiple sliding pieces accumulate attacks.
 * @brief Confirms generate_attacks() combines attacks from multiple
 *        rooks, bishops, and queens.
 */
TEST(GenerateAttacksTest, MultipleSlidingPieces) {
  Board board = Board::Empty();
  board.set_piece(A1, BLACK_ROOK);
  board.set_piece(H8, BLACK_BISHOP);
  board.set_piece(D4, BLACK_QUEEN);

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  // Should include attacks from all three pieces
  EXPECT_TRUE(attacks.test(A8));  // Rook on file
  EXPECT_TRUE(attacks.test(H1));  // Rook on rank
  EXPECT_TRUE(attacks.test(A1));  // Bishop diagonal (also attacked by rook)
  EXPECT_TRUE(attacks.test(D1));  // Queen orthogonal
  EXPECT_TRUE(attacks.test(A4));  // Queen orthogonal
  EXPECT_TRUE(attacks.test(A7));  // Queen diagonal
}

/**
 * @test All piece types combine correctly.
 * @brief Confirms generate_attacks() combines attacks from all enemy
 *        piece types simultaneously.
 */
TEST(GenerateAttacksTest, AllPieceTypesCombined) {
  Board board = Board::Empty();
  board.set_piece(E1, BLACK_KING);
  board.set_piece(B1, BLACK_KNIGHT);
  board.set_piece(D2, BLACK_PAWN);
  board.set_piece(F2, BLACK_PAWN);
  board.set_piece(A1, BLACK_ROOK);
  board.set_piece(H1, BLACK_BISHOP);
  board.set_piece(D4, BLACK_QUEEN);

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  // Verify attacks from each piece type are included
  EXPECT_TRUE(attacks.test(D1));  // King
  EXPECT_TRUE(attacks.test(E2));  // King
  EXPECT_TRUE(attacks.test(A3));  // Knight
  EXPECT_TRUE(attacks.test(C1));  // Pawn
  EXPECT_TRUE(attacks.test(E1));  // Pawn
  EXPECT_TRUE(attacks.test(A8));  // Rook
  EXPECT_TRUE(attacks.test(A7));  // Bishop (also queen)
  EXPECT_TRUE(attacks.test(D8));  // Queen
}

/**
 * @test White pieces generate attacks correctly.
 * @brief Confirms generate_attacks() works correctly when enemy is white.
 */
TEST(GenerateAttacksTest, WhiteAsEnemy) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(D4, WHITE_PAWN);
  board.set_piece(E2, WHITE_ROOK);

  Bitboard attacks = generate_attacks(board, Color::WHITE);

  // King attacks
  EXPECT_TRUE(attacks.test(D3));
  EXPECT_TRUE(attacks.test(F5));
  // Pawn attacks upward (white)
  EXPECT_TRUE(attacks.test(C5));
  EXPECT_TRUE(attacks.test(E5));
  // Rook attacks
  EXPECT_TRUE(attacks.test(E1));
  EXPECT_TRUE(attacks.test(A2));
}

/**
 * @test Standard starting position white attacks.
 * @brief Confirms generate_attacks() returns correct attack set for
 *        white pieces in starting position.
 */
TEST(GenerateAttacksTest, StartingPositionWhiteAttacks) {
  Board board = Board::StartingPosition();

  Bitboard attacks = generate_attacks(board, Color::WHITE);

  // White attacks entire third rank with pawns and some knight squares
  EXPECT_TRUE(attacks.test(A3));  // Pawn
  EXPECT_TRUE(attacks.test(H3));  // Pawn
  EXPECT_TRUE(attacks.test(C3));  // Knight from B1
  EXPECT_TRUE(attacks.test(F3));  // Knight from G1
  // No attacks on rank 4 or higher in starting position
  EXPECT_FALSE(attacks.test(E4));
}

/**
 * @test Standard starting position black attacks.
 * @brief Confirms generate_attacks() returns correct attack set for
 *        black pieces in starting position.
 */
TEST(GenerateAttacksTest, StartingPositionBlackAttacks) {
  Board board = Board::StartingPosition();

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  // Black attacks entire sixth rank with pawns and some knight squares
  EXPECT_TRUE(attacks.test(A6));  // Pawn
  EXPECT_TRUE(attacks.test(H6));  // Pawn
  EXPECT_TRUE(attacks.test(C6));  // Knight from B8
  EXPECT_TRUE(attacks.test(F6));  // Knight from G8
  // No attacks on rank 5 or lower in starting position
  EXPECT_FALSE(attacks.test(E5));
}

/**
 * @test Attacks include friendly piece squares.
 * @brief Confirms generate_attacks() includes squares occupied by
 *        attacker's own pieces (important for king safety).
 */
TEST(GenerateAttacksTest, AttacksIncludeFriendlySquares) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_ROOK);
  board.set_piece(E6, BLACK_PAWN);  // Friendly piece

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  // Rook attacks should include the friendly pawn square
  EXPECT_TRUE(attacks.test(E5));
  EXPECT_TRUE(attacks.test(E6));
  EXPECT_FALSE(attacks.test(E7));  // Blocked by friendly piece
}

/**
 * @test X-ray attacks not included.
 * @brief Confirms generate_attacks() does not include squares beyond
 *        blockers (no x-ray vision).
 */
TEST(GenerateAttacksTest, NoXRayAttacks) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_ROOK);
  board.set_piece(E5, WHITE_KING);  // Blocker
  board.set_piece(E6, WHITE_PAWN);  // Beyond blocker

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  EXPECT_TRUE(attacks.test(E5));   // Up to blocker
  EXPECT_FALSE(attacks.test(E6));  // Beyond blocker (no x-ray)
  EXPECT_FALSE(attacks.test(E7));  // Beyond blocker
}

/**
 * @test No king on board handled gracefully.
 * @brief Confirms generate_attacks() handles edge case where enemy
 *        has no king without crashing.
 */
TEST(GenerateAttacksTest, NoKingOnBoard) {
  Board board = Board::Empty();
  board.set_piece(E4, BLACK_ROOK);
  // No king placed

  Bitboard attacks = generate_attacks(board, Color::BLACK);

  // Should still generate rook attacks without crashing
  EXPECT_TRUE(attacks.test(E1));
  EXPECT_TRUE(attacks.test(A4));
}
