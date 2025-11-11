#include <gtest/gtest.h>

#include <chess_engine/bitboard.hpp>
#include <chess_engine/bitmasks.hpp>
#include <chess_engine/lookups/knight.hpp>
#include <chess_engine/square.hpp>

using namespace Attacks;

TEST(KnightAttackTest, CornerA1) {
  Bitboard expected;
  expected.set(Square::B3);
  expected.set(Square::C2);

  EXPECT_EQ(KNIGHT_ATTACKS[Square::A1], expected);
}

TEST(KnightAttackTest, CornerH1) {
  Bitboard expected;
  expected.set(Square::F2);
  expected.set(Square::G3);

  EXPECT_EQ(KNIGHT_ATTACKS[Square::H1], expected);
}

TEST(KnightAttackTest, CornerA8) {
  Bitboard expected;
  expected.set(Square::B6);
  expected.set(Square::C7);

  EXPECT_EQ(KNIGHT_ATTACKS[Square::A8], expected);
}

TEST(KnightAttackTest, CornerH8) {
  Bitboard expected;
  expected.set(Square::F7);
  expected.set(Square::G6);

  EXPECT_EQ(KNIGHT_ATTACKS[Square::H8], expected);
}

TEST(KnightAttackTest, CenterD4) {
  Bitboard expected;
  expected.set(Square::C6);
  expected.set(Square::E6);
  expected.set(Square::F5);
  expected.set(Square::F3);
  expected.set(Square::E2);
  expected.set(Square::C2);
  expected.set(Square::B3);
  expected.set(Square::B5);

  EXPECT_EQ(KNIGHT_ATTACKS[Square::D4], expected);
}