#include <gtest/gtest.h>

#include <chess_engine/movegen/pawn_move_gen.hpp>

TEST(PawnMoveGeneratorTest, AttacksWhiteReturnsCorrectArray) {
  auto result = PawnMoveGenerator::captures(Color::WHITE);

  EXPECT_EQ(result.size(), 64);
  EXPECT_EQ(result, Attacks::WHITE_PAWN_ATTACKS);
}

TEST(PawnMoveGeneratorTest, AttacksBlackReturnsCorrectArray) {
  auto result = PawnMoveGenerator::captures(Color::BLACK);

  EXPECT_EQ(result.size(), 64);
  EXPECT_EQ(result, Attacks::BLACK_PAWN_ATTACKS);
}

TEST(PawnMoveGeneratorTest, AttacksWhiteAndBlackAreDifferent) {
  auto white_result = PawnMoveGenerator::captures(Color::WHITE);
  auto black_result = PawnMoveGenerator::captures(Color::BLACK);

  EXPECT_NE(white_result, black_result);
}
