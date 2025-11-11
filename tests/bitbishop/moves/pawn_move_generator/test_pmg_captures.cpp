#include <gtest/gtest.h>

#include <bitbishop/moves/pawn_move_gen.hpp>

/**
 * @test Verifies captures returns the correct pre-computed array for White pawns.
 */
TEST(PawnMoveGeneratorTest, AttacksWhiteReturnsCorrectArray) {
  auto result = PawnMoveGenerator::captures(Color::WHITE);
  EXPECT_EQ(result.size(), 64);
  EXPECT_EQ(result, Attacks::WHITE_PAWN_ATTACKS);
}

/**
 * @test Verifies captures returns the correct pre-computed array for Black pawns.
 */
TEST(PawnMoveGeneratorTest, AttacksBlackReturnsCorrectArray) {
  auto result = PawnMoveGenerator::captures(Color::BLACK);
  EXPECT_EQ(result.size(), 64);
  EXPECT_EQ(result, Attacks::BLACK_PAWN_ATTACKS);
}

/**
 * @test Confirms White and Black capture arrays are distinct.
 */
TEST(PawnMoveGeneratorTest, AttacksWhiteAndBlackAreDifferent) {
  auto white_result = PawnMoveGenerator::captures(Color::WHITE);
  auto black_result = PawnMoveGenerator::captures(Color::BLACK);
  EXPECT_NE(white_result, black_result);
}
