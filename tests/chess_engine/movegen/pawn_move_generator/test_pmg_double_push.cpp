#include <gtest/gtest.h>

#include <chess_engine/movegen/pawn_move_gen.hpp>

TEST(PawnMoveGeneratorTest, DoublePushWhiteReturnsCorrectArray) {
  auto result = PawnMoveGenerator::double_push(Color::WHITE);

  EXPECT_EQ(result.size(), 64);
  EXPECT_EQ(result, Attacks::WHITE_PAWN_DOUBLE_PUSH);
}

TEST(PawnMoveGeneratorTest, DoublePushBlackReturnsCorrectArray) {
  auto result = PawnMoveGenerator::double_push(Color::BLACK);

  EXPECT_EQ(result.size(), 64);
  EXPECT_EQ(result, Attacks::BLACK_PAWN_DOUBLE_PUSH);
}

TEST(PawnMoveGeneratorTest, DoublePushWhiteAndBlackAreDifferent) {
  auto white_result = PawnMoveGenerator::double_push(Color::WHITE);
  auto black_result = PawnMoveGenerator::double_push(Color::BLACK);

  EXPECT_NE(white_result, black_result);
}
