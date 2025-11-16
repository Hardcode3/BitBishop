#include <gtest/gtest.h>

#include <bitbishop/moves/pawn_move_gen.hpp>

/**
 * @test Verifies double_push returns the correct pre-computed array for White pawns.
 */
TEST(PawnMoveGeneratorTest, DoublePushWhiteReturnsCorrectArray) {
  auto result = PawnMoveGenerator::double_push(Color::WHITE);
  EXPECT_EQ(result.size(), 64);
  EXPECT_EQ(result, Lookups::WHITE_PAWN_DOUBLE_PUSH);
}

/**
 * @test Verifies double_push returns the correct pre-computed array for Black pawns.
 */
TEST(PawnMoveGeneratorTest, DoublePushBlackReturnsCorrectArray) {
  auto result = PawnMoveGenerator::double_push(Color::BLACK);
  EXPECT_EQ(result.size(), 64);
  EXPECT_EQ(result, Lookups::BLACK_PAWN_DOUBLE_PUSH);
}

/**
 * @test Confirms White and Black double push arrays are distinct.
 */
TEST(PawnMoveGeneratorTest, DoublePushWhiteAndBlackAreDifferent) {
  auto white_result = PawnMoveGenerator::double_push(Color::WHITE);
  auto black_result = PawnMoveGenerator::double_push(Color::BLACK);
  EXPECT_NE(white_result, black_result);
}
