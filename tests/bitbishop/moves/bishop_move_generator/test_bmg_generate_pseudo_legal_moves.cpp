#include <gtest/gtest.h>

#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/moves/bishop_move_gen.hpp>

/**
 * @brief Test fixture for bishop pseudo-legal move generation.
 */
class BishopPseudoLegalMovesTest : public ::testing::Test {
 protected:
  std::vector<Move> moves;

  void SetUp() override {}

  void TearDown() override {}
};

/**
 * @test White bishop has no move / capture from starting position
 */
TEST_F(BishopPseudoLegalMovesTest, StartingPositionWhiteHas0Moves) {
  Board board;

  BishopMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 0);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test Black bishop has no moves / captures from starting position
 */
TEST_F(BishopPseudoLegalMovesTest, StartingPositionBlackHas0Moves) {
  Board board;

  BishopMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 0);
  EXPECT_EQ(count_captures(moves), 0);
}

/**
 * @test White bishop in the center has 13 regular moves
 */
TEST_F(BishopPseudoLegalMovesTest, WhiteBishopCenterEmptyBoardHas13Moves) {
  Board board("8/8/8/8/3B4/8/8/8 w - - 0 1");  // White bishop in D4

  BishopMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 13);
  EXPECT_EQ(count_captures(moves), 0);

  // North East Ray
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::G7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::H8, std::nullopt, false, false, false}));

  // North West Ray
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::A7, std::nullopt, false, false, false}));

  // South East Ray
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::G1, std::nullopt, false, false, false}));

  // South West Ray
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::A1, std::nullopt, false, false, false}));
}

/**
 * @test Black bishop in the center has 13 regular moves
 */
TEST_F(BishopPseudoLegalMovesTest, BlackBishopCenterEmptyBoardHas13Moves) {
  Board board("8/8/8/8/3b4/8/8/8 b - - 0 1");  // Black bishop in D4

  BishopMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 13);
  EXPECT_EQ(count_captures(moves), 0);

  // North East Ray
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::G7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::H8, std::nullopt, false, false, false}));

  // North West Ray
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::A7, std::nullopt, false, false, false}));

  // South East Ray
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::G1, std::nullopt, false, false, false}));

  // South West Ray
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::A1, std::nullopt, false, false, false}));
}

/**
 * @test White bishop in the top right hand corner has 7 regular moves
 */
TEST_F(BishopPseudoLegalMovesTest, WhiteBishopCornerHas8Moves) {
  Board board("7B/8/8/8/8/8/8/8 w - - 0 1");  // White bishop in H8 (top right hand corner)

  BishopMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 7);
  EXPECT_EQ(count_captures(moves), 0);

  // South West Ray
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::G7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::F6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::D4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::C3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::B2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H8, Squares::A1, std::nullopt, false, false, false}));
}

/**
 * @test Black bishop in the bottom left corner has 7 regular moves
 */
TEST_F(BishopPseudoLegalMovesTest, BlackBishopCornerHas7Moves) {
  Board board("8/8/8/8/8/8/8/b7 b - - 0 1");  // Black bishop in A1 (bottom left hand corner)

  BishopMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 7);
  EXPECT_EQ(count_captures(moves), 0);

  // North East Ray
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::H8, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::G7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::F6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::D4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::C3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A1, Squares::B2, std::nullopt, false, false, false}));
}

/**
 * @test White bishop on the right side has 7 regular moves
 */
TEST_F(BishopPseudoLegalMovesTest, WhiteBishopEdgeHas7Moves) {
  Board board("8/8/8/7B/8/8/8/8 w - - 0 1");  // White bishop in H5

  BishopMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(moves.size(), 7);
  EXPECT_EQ(count_captures(moves), 0);

  // North West Ray
  EXPECT_TRUE(contains_move(moves, {Squares::H5, Squares::G6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H5, Squares::F7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H5, Squares::E8, std::nullopt, false, false, false}));

  // South West Ray
  EXPECT_TRUE(contains_move(moves, {Squares::H5, Squares::G4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H5, Squares::F3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H5, Squares::E2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::H5, Squares::D1, std::nullopt, false, false, false}));
}

/**
 * @test Black bishop on the left edge has 7 regular moves
 */
TEST_F(BishopPseudoLegalMovesTest, BlackBishopEdgeHas7Moves) {
  Board board("8/8/8/b7/8/8/8/8 b - - 0 1");  // Black bishop in A5

  BishopMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(moves.size(), 7);
  EXPECT_EQ(count_captures(moves), 0);

  // North East Ray
  EXPECT_TRUE(contains_move(moves, {Squares::A5, Squares::B6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A5, Squares::C7, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A5, Squares::D8, std::nullopt, false, false, false}));

  // South East Ray
  EXPECT_TRUE(contains_move(moves, {Squares::A5, Squares::B4, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A5, Squares::C3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A5, Squares::D2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::A5, Squares::E1, std::nullopt, false, false, false}));
}

/**
 * @test White bishop can capture enemy pieces and stops at capture
 */
TEST_F(BishopPseudoLegalMovesTest, WhiteBishopCanCaptureEnemyPiece) {
  // White bishop on D4 with black pawns on F6 and B2
  Board board("8/8/5p2/8/3B4/8/1p6/8 w - - 0 1");

  BishopMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_captures(moves), 2);

  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B2, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F6, std::nullopt, true, false, false}));
}

/**
 * @test Black bishop can capture enemy pieces and stops at capture
 */
TEST_F(BishopPseudoLegalMovesTest, BlackBishopCanCaptureEnemyPiece) {
  // Black bishop on D4 with white pawns on F6 and B2
  Board board("8/8/5P2/8/3b4/8/1P6/8 b - - 0 1");

  BishopMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_captures(moves), 2);

  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B2, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F6, std::nullopt, true, false, false}));
}

/**
 * @test White bishop cannot capture own pieces and stops before them
 */
TEST_F(BishopPseudoLegalMovesTest, WhiteBishopCannotCaptureOwnPiece) {
  // White bishop on D4 with white pawns on F6 and B2
  Board board("8/8/5P2/8/3B4/8/1P6/8 w - - 0 1");

  BishopMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  EXPECT_EQ(count_captures(moves), 0);

  // North East Ray - should stop before F6 (own piece)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E5, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::F6, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::G7, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::H8, std::nullopt, false, false, false}));

  // South West Ray - should stop before B2 (own piece)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C3, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::B2, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::A1, std::nullopt, false, false, false}));

  // North West Ray - should continue unblocked
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::A7, std::nullopt, false, false, false}));

  // South East Ray - should continue unblocked
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::G1, std::nullopt, false, false, false}));
}

/**
 * @test Black bishop cannot capture own pieces and stops before them
 */
TEST_F(BishopPseudoLegalMovesTest, BlackBishopCannotCaptureOwnPiece) {
  // Black bishop on D4 with black pawns on F6 and B2
  Board board("8/8/5p2/8/3b4/8/1p6/8 b - - 0 1");

  BishopMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  EXPECT_EQ(count_captures(moves), 0);

  // North East Ray - should stop before F6 (own piece)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E5, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::F6, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::G7, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::H8, std::nullopt, false, false, false}));

  // South West Ray - should stop before B2 (own piece)
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C3, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::B2, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {Squares::D4, Squares::A1, std::nullopt, false, false, false}));

  // North West Ray - should continue unblocked
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::C5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::B6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::A7, std::nullopt, false, false, false}));

  // South East Ray - should continue unblocked
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::F2, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::G1, std::nullopt, false, false, false}));
}

/**
 * @test White bishop with mixed occupancy (own and enemy pieces on different rays)
 */
TEST_F(BishopPseudoLegalMovesTest, WhiteBishopMixedOccupancy) {
  // White bishop on D4
  // White knight on B6 (NW ray), black rook on G7 (NE ray)
  // Black queen on E3 (SE ray), white pawn on C3 (SW ray)
  Board board("8/6r1/1N6/8/3B4/2P1q3/8/8 w - - 0 1");

  BishopMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::WHITE);

  // Should have 6 moves total: 2 captures (G7, E3) and 3 quiet moves
  EXPECT_EQ(moves.size(), 5);
  EXPECT_EQ(count_captures(moves), 2);

  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E3, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::G7, std::nullopt, true, false, false}));
}

/**
 * @test Black bishop with mixed occupancy (own and enemy pieces on different rays)
 */
TEST_F(BishopPseudoLegalMovesTest, BlackBishopMixedOccupancy) {
  // Black bishop on D4
  // Black knight on B6 (NW ray), white rook on G7 (NE ray)
  // White queen on E3 (SE ray), black pawn on C3 (SW ray)
  Board board("8/6R1/1n6/8/3b4/2p1Q3/8/8 b - - 0 1");

  BishopMoveGenerator::generate_pseudo_legal_moves(moves, board, Color::BLACK);

  // Should have 5 moves total: 2 captures (G7, E3) and 3 quiet moves
  EXPECT_EQ(moves.size(), 5);
  EXPECT_EQ(count_captures(moves), 2);

  // North East Ray - can capture white rook on G7
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::E3, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {Squares::D4, Squares::G7, std::nullopt, true, false, false}));
}
