#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/movegen/castling_moves.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test White kingside castling available.
 * @brief Confirms generate_castling_moves() generates kingside castle
 *        for white when all conditions are met.
 */
TEST(GenerateCastlingMovesTest, WhiteKingsideCastling) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_TRUE(contains_move(moves, {E1, G1, std::nullopt, false, false, true}));
}

/**
 * @test White queenside castling available.
 * @brief Confirms generate_castling_moves() generates queenside castle
 *        for white when all conditions are met.
 */
TEST(GenerateCastlingMovesTest, WhiteQueensideCastling) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_TRUE(contains_move(moves, {E1, C1, std::nullopt, false, false, true}));
}

/**
 * @test White both castling available.
 * @brief Confirms generate_castling_moves() generates both castling moves
 *        when both are legal.
 */
TEST(GenerateCastlingMovesTest, WhiteBothCastling) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 2);
  EXPECT_TRUE(contains_move(moves, {E1, G1, std::nullopt, false, false, true}));
  EXPECT_TRUE(contains_move(moves, {E1, C1, std::nullopt, false, false, true}));
}

/**
 * @test Black kingside castling available.
 * @brief Confirms generate_castling_moves() generates kingside castle
 *        for black when all conditions are met.
 */
TEST(GenerateCastlingMovesTest, BlackKingsideCastling) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_castling_moves(moves, board, Color::BLACK, checkers, enemy_attacks);

  EXPECT_TRUE(contains_move(moves, {E8, G8, std::nullopt, false, false, true}));
}

/**
 * @test Black queenside castling available.
 * @brief Confirms generate_castling_moves() generates queenside castle
 *        for black when all conditions are met.
 */
TEST(GenerateCastlingMovesTest, BlackQueensideCastling) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_castling_moves(moves, board, Color::BLACK, checkers, enemy_attacks);

  EXPECT_TRUE(contains_move(moves, {E8, C8, std::nullopt, false, false, true}));
}

/**
 * @test Black both castling available.
 * @brief Confirms generate_castling_moves() generates both castling moves
 *        for black when both are legal.
 */
TEST(GenerateCastlingMovesTest, BlackBothCastling) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_castling_moves(moves, board, Color::BLACK, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 2);
  EXPECT_TRUE(contains_move(moves, {E8, G8, std::nullopt, false, false, true}));
  EXPECT_TRUE(contains_move(moves, {E8, C8, std::nullopt, false, false, true}));
}

/**
 * @test No castling when in check.
 * @brief Confirms generate_castling_moves() does not generate castling
 *        when king is in check.
 */
TEST(GenerateCastlingMovesTest, NoCastlingWhenInCheck) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  checkers.set(E8);  // Some checker
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test No castling without castling rights.
 * @brief Confirms generate_castling_moves() does not generate moves
 *        when castling rights are not available.
 */
TEST(GenerateCastlingMovesTest, NoCastlingWithoutRights) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test No kingside castling without kingside rights.
 * @brief Confirms generate_castling_moves() does not generate kingside
 *        castle when right is not available.
 */
TEST(GenerateCastlingMovesTest, NoKingsideCastlingWithoutRight) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w Qq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {E1, C1, std::nullopt, false, false, true}));
  EXPECT_FALSE(contains_move(moves, {E1, G1, std::nullopt, false, false, true}));
}

/**
 * @test No queenside castling without queenside rights.
 * @brief Confirms generate_castling_moves() does not generate queenside
 *        castle when right is not available.
 */
TEST(GenerateCastlingMovesTest, NoQueensideCastlingWithoutRight) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w Kk - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {E1, G1, std::nullopt, false, false, true}));
  EXPECT_FALSE(contains_move(moves, {E1, C1, std::nullopt, false, false, true}));
}

/**
 * @test No kingside castling when F square attacked.
 * @brief Confirms generate_castling_moves() does not generate kingside
 *        castle when F square is under attack.
 */
TEST(GenerateCastlingMovesTest, NoKingsideCastlingFSquareAttacked) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();
  enemy_attacks.set(F1);

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {E1, C1, std::nullopt, false, false, true}));
  EXPECT_FALSE(contains_move(moves, {E1, G1, std::nullopt, false, false, true}));
}

/**
 * @test No kingside castling when G square attacked.
 * @brief Confirms generate_castling_moves() does not generate kingside
 *        castle when G square is under attack.
 */
TEST(GenerateCastlingMovesTest, NoKingsideCastlingGSquareAttacked) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();
  enemy_attacks.set(G1);

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {E1, C1, std::nullopt, false, false, true}));
  EXPECT_FALSE(contains_move(moves, {E1, G1, std::nullopt, false, false, true}));
}

/**
 * @test No queenside castling when D square attacked.
 * @brief Confirms generate_castling_moves() does not generate queenside
 *        castle when D square is under attack.
 */
TEST(GenerateCastlingMovesTest, NoQueensideCastlingDSquareAttacked) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();
  enemy_attacks.set(D1);

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {E1, G1, std::nullopt, false, false, true}));
  EXPECT_FALSE(contains_move(moves, {E1, C1, std::nullopt, false, false, true}));
}

/**
 * @test No queenside castling when C square attacked.
 * @brief Confirms generate_castling_moves() does not generate queenside
 *        castle when C square is under attack.
 */
TEST(GenerateCastlingMovesTest, NoQueensideCastlingCSquareAttacked) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();
  enemy_attacks.set(C1);

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {E1, G1, std::nullopt, false, false, true}));
  EXPECT_FALSE(contains_move(moves, {E1, C1, std::nullopt, false, false, true}));
}

/**
 * @test B square attacked does not prevent queenside castling.
 * @brief Confirms generate_castling_moves() allows queenside castling
 *        even when B square is attacked (king doesn't pass through it).
 */
TEST(GenerateCastlingMovesTest, QueensideCastlingBSquareAttackedAllowed) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();
  enemy_attacks.set(B1);  // B square attacked but king doesn't pass through

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 2);
  EXPECT_TRUE(contains_move(moves, {E1, C1, std::nullopt, false, false, true}));
  EXPECT_TRUE(contains_move(moves, {E1, G1, std::nullopt, false, false, true}));
}

/**
 * @test No castling when both kingside squares attacked.
 * @brief Confirms generate_castling_moves() does not generate kingside
 *        castle when both F and G are attacked.
 */
TEST(GenerateCastlingMovesTest, NoCastlingBothKingsideSquaresAttacked) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();
  enemy_attacks.set(F1);
  enemy_attacks.set(G1);

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {E1, C1, std::nullopt, false, false, true}));
}

/**
 * @test No castling when both queenside squares attacked.
 * @brief Confirms generate_castling_moves() does not generate queenside
 *        castle when both D and C are attacked.
 */
TEST(GenerateCastlingMovesTest, NoCastlingBothQueensideSquaresAttacked) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();
  enemy_attacks.set(D1);
  enemy_attacks.set(C1);

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {E1, G1, std::nullopt, false, false, true}));
}

/**
 * @test No castling when all squares attacked.
 * @brief Confirms generate_castling_moves() does not generate any castling
 *        when all relevant squares are attacked.
 */
TEST(GenerateCastlingMovesTest, NoCastlingAllSquaresAttacked) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();
  enemy_attacks.set(F1);
  enemy_attacks.set(G1);
  enemy_attacks.set(D1);
  enemy_attacks.set(C1);

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Black kingside castling F square attacked.
 * @brief Confirms generate_castling_moves() does not generate kingside
 *        castle for black when F8 is attacked.
 */
TEST(GenerateCastlingMovesTest, BlackKingsideCastlingFSquareAttacked) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();
  enemy_attacks.set(F8);

  generate_castling_moves(moves, board, Color::BLACK, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {E8, C8, std::nullopt, false, false, true}));
  EXPECT_FALSE(contains_move(moves, {E8, G8, std::nullopt, false, false, true}));
}

/**
 * @test Black queenside castling D square attacked.
 * @brief Confirms generate_castling_moves() does not generate queenside
 *        castle for black when D8 is attacked.
 */
TEST(GenerateCastlingMovesTest, BlackQueensideCastlingDSquareAttacked) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();
  enemy_attacks.set(D8);

  generate_castling_moves(moves, board, Color::BLACK, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {E8, G8, std::nullopt, false, false, true}));
  EXPECT_FALSE(contains_move(moves, {E8, C8, std::nullopt, false, false, true}));
}

/**
 * @test Moves vector accumulates correctly.
 * @brief Confirms generate_castling_moves() appends to existing moves.
 */
TEST(GenerateCastlingMovesTest, MovesVectorAccumulates) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  std::vector<Move> moves;
  moves.emplace_back(A1, A2, std::nullopt, false, false, false);

  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 3);
  EXPECT_TRUE(contains_move(moves, {A1, A2, std::nullopt, false, false, false}));
}

/**
 * @test Castling move properties correct.
 * @brief Confirms castling moves have correct flags set.
 */
TEST(GenerateCastlingMovesTest, CastlingMovePropertiesCorrect) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  for (const Move& move : moves) {
    EXPECT_EQ(move.from, E1);
    EXPECT_TRUE(move.is_castling);
    EXPECT_FALSE(move.is_capture);
    EXPECT_FALSE(move.is_en_passant);
    EXPECT_FALSE(move.promotion.has_value());
  }
}

/**
 * @test Starting position allows castling with rights.
 * @brief Confirms generate_castling_moves() generates castling from
 *        starting position when squares are clear.
 */
TEST(GenerateCastlingMovesTest, StartingPositionWithClearSquares) {
  Board board("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 2);
}

/**
 * @test Multiple checks prevent castling.
 * @brief Confirms generate_castling_moves() does not generate castling
 *        when multiple checkers are present.
 */
TEST(GenerateCastlingMovesTest, MultipleChecksPreventCastling) {
  Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  checkers.set(D8);
  checkers.set(F8);
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Empty board with rights but wrong position.
 * @brief Confirms generate_castling_moves() relies on can_castle checks
 *        to verify piece positions (not directly tested here).
 */
TEST(GenerateCastlingMovesTest, CastlingRightsButEmptyBoard) {
  Board board("8/8/8/8/8/8/8/8 w KQkq - 0 1");

  std::vector<Move> moves;
  Bitboard checkers = Bitboard::Zeros();
  Bitboard enemy_attacks = Bitboard::Zeros();

  generate_castling_moves(moves, board, Color::WHITE, checkers, enemy_attacks);

  // Should generate no moves because can_castle_kingside/queenside will fail
  EXPECT_EQ(moves.size(), 0);
}
