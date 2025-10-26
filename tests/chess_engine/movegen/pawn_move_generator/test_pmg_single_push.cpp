#include <gtest/gtest.h>

#include <chess_engine/movegen/pawn_move_gen.hpp>

TEST(PawnMoveGeneratorTest, SinglePushWhiteReturnsCorrectArray) {
  auto result = PawnMoveGenerator::single_push(Color::WHITE);

  EXPECT_EQ(result.size(), 64);
  EXPECT_EQ(result, Attacks::WHITE_PAWN_SINGLE_PUSH);
}

TEST(PawnMoveGeneratorTest, SinglePushBlackReturnsCorrectArray) {
  auto result = PawnMoveGenerator::single_push(Color::BLACK);

  EXPECT_EQ(result.size(), 64);
  EXPECT_EQ(result, Attacks::BLACK_PAWN_SINGLE_PUSH);
}

TEST(PawnMoveGeneratorTest, SinglePushWhiteAndBlackAreDifferent) {
  auto white_result = PawnMoveGenerator::single_push(Color::WHITE);
  auto black_result = PawnMoveGenerator::single_push(Color::BLACK);

  EXPECT_NE(white_result, black_result);
}
