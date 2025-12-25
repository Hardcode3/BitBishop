#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/lookups/pawn_attacks.hpp>
#include <bitbishop/square.hpp>

using namespace Lookups;

/**
 * @test White pawn single push from A2
 * Expected: White pawn on A2 can push to A3
 */
TEST(PawnPushTest, WhiteSinglePushFromA2) {
  Bitboard expected;
  expected.set(Square::A3);

  EXPECT_EQ(WHITE_PAWN_SINGLE_PUSH[Square::A2], expected);
}

/**
 * @test Black pawn single push from H7
 * Expected: Black pawn on H7 can push to H6
 */
TEST(PawnPushTest, BlackSinglePushFromH7) {
  Bitboard expected;
  expected.set(Square::H6);

  EXPECT_EQ(BLACK_PAWN_SINGLE_PUSH[Square::H7], expected);
}

/**
 * @test White pawn double push from D2
 * Expected: White pawn on D2 can jump to D4
 */
TEST(PawnPushTest, WhiteDoublePushFromD2) {
  Bitboard expected;
  expected.set(Square::D4);

  EXPECT_EQ(WHITE_PAWN_DOUBLE_PUSH[Square::D2], expected);
}

/**
 * @test Black pawn double push from E7
 * Expected: Black pawn on E7 can jump to E5
 */
TEST(PawnPushTest, BlackDoublePushFromE7) {
  Bitboard expected;
  expected.set(Square::E5);

  EXPECT_EQ(BLACK_PAWN_DOUBLE_PUSH[Square::E7], expected);
}

/**
 * @test White pawn attacks from D4
 * Expected: White pawn on D4 attacks C5 and E5
 */
TEST(PawnAttackTest, WhitePawnAttacksFromD4) {
  Bitboard expected;
  expected.set(Square::C5);
  expected.set(Square::E5);

  EXPECT_EQ(WHITE_PAWN_ATTACKS[Square::D4], expected);
}

/**
 * @test Black pawn attacks from E5
 * Expected: Black pawn on E5 attacks D4 and F4
 */
TEST(PawnAttackTest, BlackPawnAttacksFromE5) {
  Bitboard expected;
  expected.set(Square::D4);
  expected.set(Square::F4);

  EXPECT_EQ(BLACK_PAWN_ATTACKS[Square::E5], expected);
}
