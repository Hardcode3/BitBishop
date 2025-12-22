#include <gtest/gtest.h>

#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/moves/queen_move_gen.hpp>

/**
 * @brief Test fixture for queen pseudo-legal move generation.
 */
class QueenPseudoLegalMovesTest : public ::testing::Test {
 protected:
  std::vector<Move> moves;

  void SetUp() override {}

  void TearDown() override {}
};

/**
 * @test White queen has no moves from starting position
 */
TEST_F(QueenPseudoLegalMovesTest, StartingPositionWhiteHas0Moves) {
  Board board;

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Black queen has no moves from starting position
 */
TEST_F(QueenPseudoLegalMovesTest, StartingPositionBlackHas0Moves) {
  Board board;

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test White queen in the center has 27 regular moves (8 directions)
 */
TEST_F(QueenPseudoLegalMovesTest, WhiteQueenCenterEmptyBoardHas27Moves) {
  Board board("8/8/8/8/3Q4/8/8/8 w - - 0 1");  // White queen on D4

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 27);
  EXPECT_EQ(count_captures(moves), 0);

  // North Ray (rook-like)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D8, std::nullopt, false, false, false}));

  // South Ray (rook-like)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D1, std::nullopt, false, false, false}));

  // East Ray (rook-like)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::G4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::H4, std::nullopt, false, false, false}));

  // West Ray (rook-like)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::A4, std::nullopt, false, false, false}));

  // North East Ray (bishop-like)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::G7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::H8, std::nullopt, false, false, false}));

  // North West Ray (bishop-like)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::A7, std::nullopt, false, false, false}));

  // South East Ray (bishop-like)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::G1, std::nullopt, false, false, false}));

  // South West Ray (bishop-like)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::A1, std::nullopt, false, false, false}));
}

/**
 * @test Black queen in the center has 27 regular moves (8 directions)
 */
TEST_F(QueenPseudoLegalMovesTest, BlackQueenCenterEmptyBoardHas27Moves) {
  Board board("8/8/8/8/3q4/8/8/8 b - - 0 1");  // Black queen on D4

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 27);
  EXPECT_EQ(count_captures(moves), 0);

  // North Ray (rook-like)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D8, std::nullopt, false, false, false}));

  // South Ray (rook-like)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D1, std::nullopt, false, false, false}));

  // East Ray (rook-like)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::G4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::H4, std::nullopt, false, false, false}));

  // West Ray (rook-like)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::A4, std::nullopt, false, false, false}));

  // North East Ray (bishop-like)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::G7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::H8, std::nullopt, false, false, false}));

  // North West Ray (bishop-like)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::A7, std::nullopt, false, false, false}));

  // South East Ray (bishop-like)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::G1, std::nullopt, false, false, false}));

  // South West Ray (bishop-like)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::A1, std::nullopt, false, false, false}));
}

/**
 * @test White queen in corner has 21 regular moves
 */
TEST_F(QueenPseudoLegalMovesTest, WhiteQueenCornerHas21Moves) {
  Board board("7Q/8/8/8/8/8/8/8 w - - 0 1");  // White queen on H8

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 21);
  EXPECT_EQ(count_captures(moves), 0);

  // South Ray
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::H7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::H6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::H5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::H4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::H3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::H2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::H1, std::nullopt, false, false, false}));

  // West Ray
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::G8, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::F8, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::E8, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::D8, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::C8, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::B8, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::A8, std::nullopt, false, false, false}));

  // South West Ray (diagonal)
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::G7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::F6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::D4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::C3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::B2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::A1, std::nullopt, false, false, false}));
}

/**
 * @test Black queen in corner has 21 regular moves
 */
TEST_F(QueenPseudoLegalMovesTest, BlackQueenCornerHas21Moves) {
  Board board("8/8/8/8/8/8/8/q7 b - - 0 1");  // Black queen on A1

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 21);
  EXPECT_EQ(count_captures(moves), 0);

  // North Ray
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::A2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::A3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::A4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::A5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::A6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::A7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::A8, std::nullopt, false, false, false}));

  // East Ray
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::B1, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::C1, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::D1, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::E1, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::F1, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::G1, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::H1, std::nullopt, false, false, false}));

  // North East Ray (diagonal)
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::B2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::C3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::D4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::F6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::G7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::H8, std::nullopt, false, false, false}));
}

/**
 * @test White queen on edge has 21 regular moves
 */
TEST_F(QueenPseudoLegalMovesTest, WhiteQueenEdgeHas21Moves) {
  Board board("8/8/8/8/7Q/8/8/8 w - - 0 1");  // White queen on H4

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 21);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Black queen on edge has 21 regular moves
 */
TEST_F(QueenPseudoLegalMovesTest, BlackQueenEdgeHas21Moves) {
  Board board("8/8/8/8/q7/8/8/8 b - - 0 1");  // Black queen on A4

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 21);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test White queen can capture enemy pieces and stops at capture
 */
TEST_F(QueenPseudoLegalMovesTest, WhiteQueenCanCaptureEnemyPieces) {
  // White queen on D4 with black pawns on D6 (north), F4 (east), F6 (NE), B2 (SW)
  Board board("8/8/3p1p2/8/3Q1p2/8/1p6/8 w - - 0 1");

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_captures(moves), 4);

  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D6, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F4, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F6, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B2, std::nullopt, true, false, false}));

  // Should not go beyond captures
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::D7, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::G4, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::G7, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::A1, std::nullopt, false, false, false}));
}

/**
 * @test Black queen can capture enemy pieces and stops at capture
 */
TEST_F(QueenPseudoLegalMovesTest, BlackQueenCanCaptureEnemyPieces) {
  // Black queen on D4 with white pawns on D6 (north), F4 (east), F6 (NE), B2 (SW)
  Board board("8/8/3P1P2/8/3q1P2/8/1P6/8 b - - 0 1");

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_captures(moves), 4);

  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D6, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F4, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F6, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B2, std::nullopt, true, false, false}));
}

/**
 * @test White queen cannot capture own pieces and stops before them
 */
TEST_F(QueenPseudoLegalMovesTest, WhiteQueenCannotCaptureOwnPieces) {
  // White queen on D4 with white pawns on D6 (north), F4 (east), F6 (NE), B2 (SW)
  Board board("8/8/3P1P2/8/3Q1P2/8/1P6/8 w - - 0 1");

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_captures(moves), 0);

  // North Ray - should stop before D6 (own piece)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D5, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::D6, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::D7, std::nullopt, false, false, false}));

  // East Ray - should stop before F4 (own piece)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E4, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::F4, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::G4, std::nullopt, false, false, false}));

  // North East Ray - should stop before F6 (own piece)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E5, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::F6, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::G7, std::nullopt, false, false, false}));

  // South West Ray - should stop before B2 (own piece)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C3, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::B2, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::A1, std::nullopt, false, false, false}));
}

/**
 * @test Black queen cannot capture own pieces and stops before them
 */
TEST_F(QueenPseudoLegalMovesTest, BlackQueenCannotCaptureOwnPieces) {
  // Black queen on D4 with black pawns on D6 (north), F4 (east), F6 (NE), B2 (SW)
  Board board("8/8/3p1p2/8/3q1p2/8/1p6/8 b - - 0 1");

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_captures(moves), 0);

  // North Ray - should stop before D6 (own piece)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D5, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::D6, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::D7, std::nullopt, false, false, false}));

  // East Ray - should stop before F4 (own piece)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E4, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::F4, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::G4, std::nullopt, false, false, false}));

  // North East Ray - should stop before F6 (own piece)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E5, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::F6, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::G7, std::nullopt, false, false, false}));

  // South West Ray - should stop before B2 (own piece)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C3, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::B2, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::A1, std::nullopt, false, false, false}));
}

/**
 * @test White queen with mixed occupancy (own and enemy pieces on different rays)
 */
TEST_F(QueenPseudoLegalMovesTest, WhiteQueenMixedOccupancy) {
  // White queen on D4
  // White knight on D6 (N), black rook on F4 (E)
  // White pawn on C3 (SW), black queen on E5 (NE)
  Board board("8/8/3N4/4q3/3Q1r2/2P5/8/8 w - - 0 1");

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_captures(moves), 2);

  // Can capture enemy pieces
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F4, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E5, std::nullopt, true, false, false}));

  // Cannot capture own pieces
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::D6, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::C3, std::nullopt, false, false, false}));
}

/**
 * @test Black queen with mixed occupancy (own and enemy pieces on different rays)
 */
TEST_F(QueenPseudoLegalMovesTest, BlackQueenMixedOccupancy) {
  // Black queen on D4
  // Black knight on D6 (N), white rook on F4 (E)
  // Black pawn on C3 (SW), white queen on E5 (NE)
  Board board("8/8/3n4/4Q3/3q1R2/2p5/8/8 b - - 0 1");

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_captures(moves), 2);

  // Can capture enemy pieces
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F4, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E5, std::nullopt, true, false, false}));

  // Cannot capture own pieces
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::D6, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::C3, std::nullopt, false, false, false}));
}

/**
 * @test Multiple white queens on the board (after promotion)
 */
TEST_F(QueenPseudoLegalMovesTest, MultipleWhiteQueens) {
  // Two white queens on D4 and E5
  Board board("8/8/8/4Q3/3Q4/8/8/8 w - - 0 1");

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  // Each queen should have moves, though some rays will be blocked by the other queen
  EXPECT_GT(moves.size(), 0);

  // D4 queen can move to C4, B4, A4 (west ray unblocked)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::A4, std::nullopt, false, false, false}));

  // E5 queen can move to F5, G5, H5 (east ray unblocked)
  EXPECT_TRUE(contains_move(moves, {Squares::E5, Squares::F5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::E5, Squares::G5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::E5, Squares::H5, std::nullopt, false, false, false}));

  // D4 queen cannot move through or capture E5 queen (friendly)
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::E5, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::F6, std::nullopt, false, false, false}));
}

/**
 * @test Multiple black queens on the board (after promotion)
 */
TEST_F(QueenPseudoLegalMovesTest, MultipleBlackQueens) {
  // Two black queens on D4 and E5
  Board board("8/8/8/4q3/3q4/8/8/8 b - - 0 1");

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  // Each queen should have moves, though some rays will be blocked by the other queen
  EXPECT_GT(moves.size(), 0);

  // D4 queen can move to C4, B4, A4 (west ray unblocked)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::A4, std::nullopt, false, false, false}));

  // E5 queen can move to F5, G5, H5 (east ray unblocked)
  EXPECT_TRUE(contains_move(moves, {Squares::E5, Squares::F5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::E5, Squares::G5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::E5, Squares::H5, std::nullopt, false, false, false}));

  // D4 queen cannot move through or capture E5 queen (friendly)
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::E5, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::F6, std::nullopt, false, false, false}));
}

/**
 * @test White queen fully surrounded by own pieces has no moves
 */
TEST_F(QueenPseudoLegalMovesTest, WhiteQueenFullySurrounded) {
  // White queen on D4 completely surrounded by white pawns
  Board board("8/8/8/2PPP3/2PQP3/2PPP3/8/8 w - - 0 1");

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Black queen fully surrounded by own pieces has no moves
 */
TEST_F(QueenPseudoLegalMovesTest, BlackQueenFullySurrounded) {
  // Black queen on D4 completely surrounded by black pawns
  Board board("8/8/8/2ppp3/2pqp3/2ppp3/8/8 b - - 0 1");

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test White queen can capture all surrounding enemy pieces
 */
TEST_F(QueenPseudoLegalMovesTest, WhiteQueenCanCaptureAllSurrounding) {
  // White queen on D4 completely surrounded by black pawns
  Board board("8/8/8/2ppp3/2pQp3/2ppp3/8/8 w - - 0 1");

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_captures(moves), 8);

  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D5, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E5, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E4, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E3, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D3, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C3, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C4, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C5, std::nullopt, true, false, false}));
}

/**
 * @test Black queen can capture all surrounding enemy pieces
 */
TEST_F(QueenPseudoLegalMovesTest, BlackQueenCanCaptureAllSurrounding) {
  // Black queen on D4 completely surrounded by white pawns
  Board board("8/8/8/2PPP3/2PqP3/2PPP3/8/8 b - - 0 1");

  QueenMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 8);
  EXPECT_EQ(count_captures(moves), 8);

  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D5, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E5, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E4, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E3, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::D3, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C3, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C4, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C5, std::nullopt, true, false, false}));
}
