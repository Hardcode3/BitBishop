#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/moves/king_move_gen.hpp>
#include <bitbishop/moves/rook_move_gen.hpp>

/**
 * @test RookMoveGenerator::add_rook_castling() with no castling rights
 * @brief Verifies that no castling moves are added when castling rights are not available.
 */
TEST(RookMoveGeneratorTest, AddRookCastlingNoCastlingRights) {
  Board board("8/8/8/8/8/8/8/R3K2R w - - 0 1");

  std::vector<Move> moves;

  RookMoveGenerator::add_rook_castling(moves, Squares::A1, Color::WHITE, board);

  EXPECT_EQ(moves.size(), 0);

  RookMoveGenerator::add_rook_castling(moves, Squares::H1, Color::WHITE, board);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test RookMoveGenerator::add_rook_castling() with no castling rights
 * @brief Verifies that no castling moves are added when castling rights are not available.
 */
TEST(RookMoveGeneratorTest, AddRookCastlingBlackNoCastlingRights) {
  Board board("r3k2r/8/8/8/8/8/8/8 w - - 0 1");

  std::vector<Move> moves;

  RookMoveGenerator::add_rook_castling(moves, Squares::A8, Color::BLACK, board);

  EXPECT_EQ(moves.size(), 0);

  RookMoveGenerator::add_rook_castling(moves, Squares::H8, Color::BLACK, board);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test RookMoveGenerator::add_rook_castling() with both castling rights available
 * @brief Verifies that both kingside and queenside castling moves are added when both are legal.
 */
TEST(RookMoveGeneratorTest, AddRookCastlingBothSidesAvailable) {
  Board board("8/8/8/8/8/8/8/R3K2R w KQ - 0 1");

  std::vector<Move> moves;

  RookMoveGenerator::add_rook_castling(moves, Squares::A1, Color::WHITE, board);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_EQ(count_rook_queenside_castling(moves, Color::WHITE), 1);

  RookMoveGenerator::add_rook_castling(moves, Squares::H1, Color::WHITE, board);

  EXPECT_EQ(moves.size(), 2);
  EXPECT_EQ(count_rook_kingside_castling(moves, Color::WHITE), 1);
}

/**
 * @test RookMoveGenerator::add_rook_castling() with only kingside castling available
 * @brief Verifies that only kingside castling move is added when queenside is not legal.
 */
TEST(RookMoveGeneratorTest, AddRookCastlingOnlyKingsideAvailable) {
  Board board("8/8/8/8/8/8/8/R3K2R w K - 0 1");

  std::vector<Move> moves;

  RookMoveGenerator::add_rook_castling(moves, Squares::A1, Color::WHITE, board);

  EXPECT_EQ(moves.size(), 0);
  EXPECT_EQ(count_rook_queenside_castling(moves, Color::WHITE), 0);

  RookMoveGenerator::add_rook_castling(moves, Squares::H1, Color::WHITE, board);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_EQ(count_rook_kingside_castling(moves, Color::WHITE), 1);
}

/**
 * @test RookMoveGenerator::add_rook_castling() with only queenside castling available
 * @brief Verifies that only queenside castling move is added when kingside is not legal.
 */
TEST(RookMoveGeneratorTest, AddRookCastlingOnlyQueensideAvailable) {
  Board board("8/8/8/8/8/8/8/R3K2R w Q - 0 1");

  std::vector<Move> moves;

  RookMoveGenerator::add_rook_castling(moves, Squares::A1, Color::WHITE, board);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_EQ(count_rook_queenside_castling(moves, Color::WHITE), 1);

  RookMoveGenerator::add_rook_castling(moves, Squares::H1, Color::WHITE, board);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_EQ(count_rook_kingside_castling(moves, Color::WHITE), 0);
}

/**
 * @test RookMoveGenerator::add_rook_castling() for black with both sides available
 * @brief Verifies that castling moves are correctly generated for black pieces.
 */
TEST(RookMoveGeneratorTest, AddRookCastlingBlackBothSidesAvailable) {
  Board board("r3k2r/8/8/8/8/8/8/8 b kq - 0 1");

  std::vector<Move> moves;

  RookMoveGenerator::add_rook_castling(moves, Squares::A8, Color::BLACK, board);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_EQ(count_rook_queenside_castling(moves, Color::BLACK), 1);

  RookMoveGenerator::add_rook_castling(moves, Squares::H8, Color::BLACK, board);

  EXPECT_EQ(moves.size(), 2);
  EXPECT_EQ(count_rook_kingside_castling(moves, Color::BLACK), 1);
}

/**
 * @test RookMoveGenerator::add_rook_castling() for black with only kingside available
 * @brief Verifies that only kingside castling is added for black when queenside is blocked.
 */
TEST(RookMoveGeneratorTest, AddRookCastlingBlackOnlyKingsideAvailable) {
  Board board("r3k2r/8/8/8/8/8/8/8 b k - 0 1");

  std::vector<Move> moves;

  RookMoveGenerator::add_rook_castling(moves, Squares::A8, Color::BLACK, board);

  EXPECT_EQ(moves.size(), 0);
  EXPECT_EQ(count_rook_queenside_castling(moves, Color::BLACK), 0);

  RookMoveGenerator::add_rook_castling(moves, Squares::H8, Color::BLACK, board);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_EQ(count_rook_kingside_castling(moves, Color::BLACK), 1);
}

/**
 * @test RookMoveGenerator::add_rook_castling() for black with only queenside available
 * @brief Verifies that only queenside castling is added for black when kingside is blocked.
 */
TEST(RookMoveGeneratorTest, AddRookCastlingBlackOnlyQueensideAvailable) {
  Board board("r3k2r/8/8/8/8/8/8/8 b q - 0 1");

  std::vector<Move> moves;

  RookMoveGenerator::add_rook_castling(moves, Squares::A8, Color::BLACK, board);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_EQ(count_rook_queenside_castling(moves, Color::BLACK), 1);

  RookMoveGenerator::add_rook_castling(moves, Squares::H8, Color::BLACK, board);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_EQ(count_rook_kingside_castling(moves, Color::BLACK), 0);
}

/**
 * @test RookMoveGenerator::add_rook_castling() with pieces blocking castling
 * @brief Verifies that castling moves are not added when pieces block the path.
 */
TEST(RookMoveGeneratorTest, AddRookCastlingBlockedByPieces) {
  Board board("8/8/8/8/8/8/8/R1B1KN1R w KQ - 0 1");

  std::vector<Move> moves;

  RookMoveGenerator::add_rook_castling(moves, Squares::A1, Color::WHITE, board);

  EXPECT_EQ(moves.size(), 0);

  RookMoveGenerator::add_rook_castling(moves, Squares::H1, Color::WHITE, board);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test RookMoveGenerator::add_rook_castling() with pieces blocking castling
 * @brief Verifies that castling moves are not added when pieces block the path.
 */
TEST(RookMoveGeneratorTest, AddRookCastlingBlackBlockedByPieces) {
  Board board("r1b1kn1r/8/8/8/8/8/8/8 w kq - 0 1");

  std::vector<Move> moves;

  RookMoveGenerator::add_rook_castling(moves, Squares::A8, Color::BLACK, board);

  EXPECT_EQ(moves.size(), 0);

  RookMoveGenerator::add_rook_castling(moves, Squares::H8, Color::BLACK, board);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test RookMoveGenerator::add_rook_castling() with invalid starting square for a black rook
 * @brief Verifies that castling moves are not added when black rook's starting position is invalid.
 */
TEST(RookMoveGeneratorTest, AddRookCastlingInvalidFromSquare) {
  Board board("8/8/8/8/8/8/8/R1B1KN1R w KQ - 0 1");

  std::vector<Move> moves;

  // Should start from square A1 for white pieces
  RookMoveGenerator::add_rook_castling(moves, Squares::A8, Color::BLACK, board);
  RookMoveGenerator::add_rook_castling(moves, Squares::D4, Color::BLACK, board);
  RookMoveGenerator::add_rook_castling(moves, Squares::E1, Color::BLACK, board);

  EXPECT_EQ(moves.size(), 0);

  // Should start from square H1 for white pieces
  RookMoveGenerator::add_rook_castling(moves, Squares::H8, Color::BLACK, board);
  RookMoveGenerator::add_rook_castling(moves, Squares::D5, Color::BLACK, board);
  RookMoveGenerator::add_rook_castling(moves, Squares::E1, Color::BLACK, board);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test RookMoveGenerator::add_rook_castling() with invalid starting square for a white rook
 * @brief Verifies that castling moves are not added when white rook's starting position is invalid.
 */
TEST(RookMoveGeneratorTest, AddRookCastlingBlackInvalidFromSquare) {
  Board board("8/8/8/8/8/8/8/R3K2R w KQ - 0 1");

  std::vector<Move> moves;

  // Should start from square A8 for white pieces
  RookMoveGenerator::add_rook_castling(moves, Squares::A1, Color::BLACK, board);
  RookMoveGenerator::add_rook_castling(moves, Squares::D4, Color::BLACK, board);
  RookMoveGenerator::add_rook_castling(moves, Squares::E8, Color::BLACK, board);

  EXPECT_EQ(moves.size(), 0);

  // Should start from square H8 for white pieces
  RookMoveGenerator::add_rook_castling(moves, Squares::H1, Color::BLACK, board);
  RookMoveGenerator::add_rook_castling(moves, Squares::D5, Color::BLACK, board);
  RookMoveGenerator::add_rook_castling(moves, Squares::E8, Color::BLACK, board);

  EXPECT_EQ(moves.size(), 0);
}
