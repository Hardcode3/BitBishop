#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/movegen/king_moves.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test King on empty board generates all moves.
 * @brief Confirms generate_legal_king_moves() produces 8 moves when king
 *        is in center with no restrictions.
 */
TEST(GenerateLegalKingMovesTest, CenterKingEmptyBoard) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);

  std::vector<Move> moves;
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_legal_king_moves(moves, board, Color::WHITE, E4, enemy_attacks);

  EXPECT_EQ(moves.size(), 8);

  // Verify all 8 surrounding squares are in moves
  EXPECT_TRUE(contains_move(moves, {E4, D3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, D4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, D5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F5, std::nullopt, false, false, false}));
}

/**
 * @test King on corner generates limited moves.
 * @brief Confirms generate_legal_king_moves() produces only 3 moves when
 *        king is on corner square.
 */
TEST(GenerateLegalKingMovesTest, CornerKingLimitedMoves) {
  Board board = Board::Empty();
  board.set_piece(A1, WHITE_KING);

  std::vector<Move> moves;
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_legal_king_moves(moves, board, Color::WHITE, A1, enemy_attacks);

  EXPECT_EQ(moves.size(), 3);

  // Corner king can only move to 3 squares
  EXPECT_TRUE(contains_move(moves, {A1, A2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A1, B1, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A1, B2, std::nullopt, false, false, false}));
}

/**
 * @test King on edge generates limited moves.
 * @brief Confirms generate_legal_king_moves() produces only 5 moves when
 *        king is on edge square.
 */
TEST(GenerateLegalKingMovesTest, EdgeKingLimitedMoves) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);

  std::vector<Move> moves;
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_legal_king_moves(moves, board, Color::WHITE, E1, enemy_attacks);

  EXPECT_EQ(moves.size(), 5);

  // Edge king can move to 5 squares
  EXPECT_TRUE(contains_move(moves, {E1, D1, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E1, D2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E1, E2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E1, F1, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E1, F2, std::nullopt, false, false, false}));
}

/**
 * @test Friendly pieces block king moves.
 * @brief Confirms generate_legal_king_moves() excludes squares occupied
 *        by friendly pieces.
 */
TEST(GenerateLegalKingMovesTest, FriendlyPiecesBlocked) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(D3, WHITE_PAWN);
  board.set_piece(E5, WHITE_KNIGHT);
  board.set_piece(F4, WHITE_BISHOP);

  std::vector<Move> moves;
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_legal_king_moves(moves, board, Color::WHITE, E4, enemy_attacks);

  EXPECT_EQ(moves.size(), 5);

  // Verify blocked squares are not in moves
  EXPECT_FALSE(contains_move(moves, {E4, D3, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, E5, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, F4, std::nullopt, false, false, false}));

  // Verify remaining moves are present
  EXPECT_TRUE(contains_move(moves, {E4, D4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, D5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F5, std::nullopt, false, false, false}));
}

/**
 * @test Enemy attacks prevent king moves.
 * @brief Confirms generate_legal_king_moves() excludes squares under
 *        enemy attack.
 */
TEST(GenerateLegalKingMovesTest, EnemyAttacksBlocked) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);

  Bitboard enemy_attacks = Bitboard::Zeros();
  enemy_attacks.set(D3);
  enemy_attacks.set(D4);
  enemy_attacks.set(D5);

  std::vector<Move> moves;

  generate_legal_king_moves(moves, board, Color::WHITE, E4, enemy_attacks);

  EXPECT_EQ(moves.size(), 5);

  // Verify attacked squares are not in moves
  EXPECT_FALSE(contains_move(moves, {E4, D3, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, D4, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, D5, std::nullopt, false, false, false}));

  // Verify remaining moves are present
  EXPECT_TRUE(contains_move(moves, {E4, E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F5, std::nullopt, false, false, false}));
}

/**
 * @test King captures enemy pieces.
 * @brief Confirms generate_legal_king_moves() generates capture moves
 *        and marks them correctly.
 */
TEST(GenerateLegalKingMovesTest, KingCapturesEnemyPieces) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(D3, BLACK_PAWN);
  board.set_piece(E5, BLACK_KNIGHT);

  std::vector<Move> moves;
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_legal_king_moves(moves, board, Color::WHITE, E4, enemy_attacks);

  // Should generate 8 moves (6 empty + 2 captures)
  EXPECT_EQ(moves.size(), 8);

  // Verify capture moves
  EXPECT_TRUE(contains_move(moves, {E4, D3, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E5, std::nullopt, true, false, false}));

  // Verify non-capture moves
  EXPECT_TRUE(contains_move(moves, {E4, D4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, D5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F5, std::nullopt, false, false, false}));
}

/**
 * @test King cannot capture on attacked squares.
 * @brief Confirms generate_legal_king_moves() excludes enemy pieces
 *        if their square is under attack.
 */
TEST(GenerateLegalKingMovesTest, CannotCaptureOnAttackedSquare) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(D3, BLACK_PAWN);

  Bitboard enemy_attacks = Bitboard::Zeros();
  enemy_attacks.set(D3);  // Square is defended

  std::vector<Move> moves;

  generate_legal_king_moves(moves, board, Color::WHITE, E4, enemy_attacks);

  // Should generate 7 moves (D3 is excluded)
  EXPECT_EQ(moves.size(), 7);

  EXPECT_FALSE(contains_move(moves, {E4, D3, std::nullopt, true, false, false}));
}

/**
 * @test All restrictions combined.
 * @brief Confirms generate_legal_king_moves() correctly applies friendly
 *        pieces and enemy attacks simultaneously.
 */
TEST(GenerateLegalKingMovesTest, AllRestrictionsCombined) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(D3, WHITE_PAWN);    // Friendly blocker
  board.set_piece(E5, BLACK_KNIGHT);  // Enemy piece (attacks D3, F3)

  Bitboard enemy_attacks = Bitboard::Zeros();
  enemy_attacks.set(D3);  // Attacked by knight
  enemy_attacks.set(D4);  // Enemy attacks from somewhere else
  enemy_attacks.set(E5);  // Knight square defended
  enemy_attacks.set(F3);  // Attacked by knight

  std::vector<Move> moves;

  generate_legal_king_moves(moves, board, Color::WHITE, E4, enemy_attacks);

  // Available: D5, E3, F4, F5 (4 moves)
  EXPECT_EQ(moves.size(), 4);

  EXPECT_TRUE(contains_move(moves, {E4, D5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F5, std::nullopt, false, false, false}));
}

/**
 * @test Move properties are correct.
 * @brief Confirms generate_legal_king_moves() sets all move properties
 *        correctly (no promotion, en passant, or castling).
 */
TEST(GenerateLegalKingMovesTest, MovePropertiesCorrect) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(D3, BLACK_PAWN);

  std::vector<Move> moves;
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_legal_king_moves(moves, board, Color::WHITE, E4, enemy_attacks);

  for (const Move& move : moves) {
    EXPECT_EQ(move.from, E4);
    EXPECT_FALSE(move.promotion.has_value());
    EXPECT_FALSE(move.is_en_passant);
    EXPECT_FALSE(move.is_castling);
  }

  // Verify capture flag specifically
  EXPECT_TRUE(contains_move(moves, {E4, D3, std::nullopt, true, false, false}));
}

/**
 * @test Black king generates moves correctly.
 * @brief Confirms generate_legal_king_moves() works correctly for
 *        black king.
 */
TEST(GenerateLegalKingMovesTest, BlackKingMoves) {
  Board board = Board::Empty();
  board.set_piece(E8, BLACK_KING);
  board.set_piece(D7, BLACK_PAWN);  // Friendly
  board.set_piece(E7, WHITE_PAWN);  // Enemy

  std::vector<Move> moves;
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_legal_king_moves(moves, board, Color::BLACK, E8, enemy_attacks);

  // Should generate 4 moves (D7 blocked, E7 capturable), D8, F8 and F7 free
  EXPECT_EQ(moves.size(), 4);

  // Verify D7 not in moves
  EXPECT_FALSE(contains_move(moves, {E8, D7, std::nullopt, false, false, false}));

  // Verify E7 is a capture
  EXPECT_TRUE(contains_move(moves, {E8, E7, std::nullopt, true, false, false}));

  // Verify other moves
  EXPECT_TRUE(contains_move(moves, {E8, D8, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E8, F7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E8, F8, std::nullopt, false, false, false}));
}

/**
 * @test King completely surrounded by friendly pieces.
 * @brief Confirms generate_legal_king_moves() generates no moves when
 *        all adjacent squares have friendly pieces.
 */
TEST(GenerateLegalKingMovesTest, KingSurroundedNoMoves) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(D3, WHITE_PAWN);
  board.set_piece(D4, WHITE_PAWN);
  board.set_piece(D5, WHITE_PAWN);
  board.set_piece(E3, WHITE_PAWN);
  board.set_piece(E5, WHITE_PAWN);
  board.set_piece(F3, WHITE_PAWN);
  board.set_piece(F4, WHITE_PAWN);
  board.set_piece(F5, WHITE_PAWN);

  std::vector<Move> moves;
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_legal_king_moves(moves, board, Color::WHITE, E4, enemy_attacks);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test King completely surrounded by enemy attacks.
 * @brief Confirms generate_legal_king_moves() generates no moves when
 *        all adjacent squares are under attack.
 */
TEST(GenerateLegalKingMovesTest, AllSquaresAttackedNoMoves) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);

  Bitboard enemy_attacks = Bitboard::Zeros();
  enemy_attacks.set(D3);
  enemy_attacks.set(D4);
  enemy_attacks.set(D5);
  enemy_attacks.set(E3);
  enemy_attacks.set(E5);
  enemy_attacks.set(F3);
  enemy_attacks.set(F4);
  enemy_attacks.set(F5);

  std::vector<Move> moves;

  generate_legal_king_moves(moves, board, Color::WHITE, E4, enemy_attacks);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Moves vector accumulates correctly.
 * @brief Confirms generate_legal_king_moves() appends to existing moves
 *        vector without clearing it.
 */
TEST(GenerateLegalKingMovesTest, MovesVectorAccumulates) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);

  std::vector<Move> moves;
  // Add a dummy move first
  moves.emplace_back(A1, A2, std::nullopt, false, false, false);

  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_legal_king_moves(moves, board, Color::WHITE, E4, enemy_attacks);

  // Should have 1 dummy + 8 king moves = 9 total
  EXPECT_EQ(moves.size(), 9);
  EXPECT_TRUE(contains_move(moves, {A1, A2, std::nullopt, false, false, false}));
}

/**
 * @test King captures undefended enemy pieces only.
 * @brief Confirms generate_legal_king_moves() allows captures of
 *        undefended enemy pieces but not defended ones.
 */
TEST(GenerateLegalKingMovesTest, CaptureUndefendedOnly) {
  Board board = Board::Empty();
  board.set_piece(E4, WHITE_KING);
  board.set_piece(D3, BLACK_PAWN);  // Undefended
  board.set_piece(E5, BLACK_PAWN);  // Defended

  Bitboard enemy_attacks = Bitboard::Zeros();
  enemy_attacks.set(E5);  // E5 is defended

  std::vector<Move> moves;

  generate_legal_king_moves(moves, board, Color::WHITE, E4, enemy_attacks);

  // Verify D3 capture is allowed
  EXPECT_TRUE(contains_move(moves, {E4, D3, std::nullopt, true, false, false}));

  // E5 should not be in moves
  EXPECT_FALSE(contains_move(moves, {E4, E5, std::nullopt, true, false, false}));
}
