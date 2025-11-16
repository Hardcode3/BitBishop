#include <gtest/gtest.h>

#include <bitbishop/moves/pawn_move_gen.hpp>

/**
 * @test Verifies single_push returns the correct pre-computed array for White pawns.
 */
TEST(PawnMoveGeneratorTest, SinglePushWhiteReturnsCorrectArray) {
  auto result = PawnMoveGenerator::single_push(Color::WHITE);
  EXPECT_EQ(result.size(), 64);
  EXPECT_EQ(result, Lookups::WHITE_PAWN_SINGLE_PUSH);
}

/**
 * @test Verifies single_push returns the correct pre-computed array for Black pawns.
 */
TEST(PawnMoveGeneratorTest, SinglePushBlackReturnsCorrectArray) {
  auto result = PawnMoveGenerator::single_push(Color::BLACK);
  EXPECT_EQ(result.size(), 64);
  EXPECT_EQ(result, Lookups::BLACK_PAWN_SINGLE_PUSH);
}

/**
 * @test Confirms White and Black single push arrays are distinct.
 *
 * White pawns move up (+1 rank), Black pawns move down (-1 rank),
 * so their move tables must differ.
 */
TEST(PawnMoveGeneratorTest, SinglePushWhiteAndBlackAreDifferent) {
  auto white_result = PawnMoveGenerator::single_push(Color::WHITE);
  auto black_result = PawnMoveGenerator::single_push(Color::BLACK);
  EXPECT_NE(white_result, black_result);
}
