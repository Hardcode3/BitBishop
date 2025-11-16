#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/lookups/queen.hpp>
#include <bitbishop/square.hpp>

TEST(QueenAttacksTest, CornerA1) {
  Bitboard bb = Lookups::QUEEN_ATTACKS[Square::A1];

  Bitboard expected = Bitboard(Bitmasks::FILE_A | Bitmasks::RANK_1);
  expected.clear(Square::A1);

  EXPECT_EQ(bb.value() & (Bitmasks::FILE_A | Bitmasks::RANK_1), expected.value());

  EXPECT_TRUE(bb.test(Square::B2));
  EXPECT_TRUE(bb.test(Square::C3));
  EXPECT_TRUE(bb.test(Square::D4));
  EXPECT_TRUE(bb.test(Square::E5));
  EXPECT_TRUE(bb.test(Square::F6));
  EXPECT_TRUE(bb.test(Square::G7));
  EXPECT_TRUE(bb.test(Square::H8));
}

TEST(QueenAttacksTest, CornerA8) {
  Bitboard bb = Lookups::QUEEN_ATTACKS[Square::A8];

  Bitboard expected = Bitboard(Bitmasks::FILE_A | Bitmasks::RANK_8);
  expected.clear(Square::A8);

  EXPECT_EQ(bb.value() & (Bitmasks::FILE_A | Bitmasks::RANK_8), expected.value());

  EXPECT_TRUE(bb.test(Square::B7));
  EXPECT_TRUE(bb.test(Square::C6));
  EXPECT_TRUE(bb.test(Square::D5));
  EXPECT_TRUE(bb.test(Square::E4));
  EXPECT_TRUE(bb.test(Square::F3));
  EXPECT_TRUE(bb.test(Square::G2));
  EXPECT_TRUE(bb.test(Square::H1));
}

TEST(QueenAttacksTest, CornerH1) {
  Bitboard bb = Lookups::QUEEN_ATTACKS[Square::H1];

  Bitboard expected = Bitboard(Bitmasks::FILE_H | Bitmasks::RANK_1);
  expected.clear(Square::H1);

  EXPECT_EQ(bb.value() & (Bitmasks::FILE_H | Bitmasks::RANK_1), expected.value());

  EXPECT_TRUE(bb.test(Square::A8));
  EXPECT_TRUE(bb.test(Square::B7));
  EXPECT_TRUE(bb.test(Square::C6));
  EXPECT_TRUE(bb.test(Square::D5));
  EXPECT_TRUE(bb.test(Square::E4));
  EXPECT_TRUE(bb.test(Square::F3));
  EXPECT_TRUE(bb.test(Square::G2));
}

TEST(QueenAttacksTest, CornerH8) {
  Bitboard bb = Lookups::QUEEN_ATTACKS[Square::H8];

  Bitboard expected = Bitboard(Bitmasks::FILE_H | Bitmasks::RANK_8);
  expected.clear(Square::H8);

  EXPECT_EQ(bb.value() & (Bitmasks::FILE_H | Bitmasks::RANK_8), expected.value());

  EXPECT_TRUE(bb.test(Square::A1));
  EXPECT_TRUE(bb.test(Square::B2));
  EXPECT_TRUE(bb.test(Square::C3));
  EXPECT_TRUE(bb.test(Square::D4));
  EXPECT_TRUE(bb.test(Square::E5));
  EXPECT_TRUE(bb.test(Square::F6));
  EXPECT_TRUE(bb.test(Square::G7));
}

TEST(QueenAttacksTest, CenterD4) {
  Bitboard bb = Lookups::QUEEN_ATTACKS[Square::D4];

  Bitboard expected = Bitboard(Bitmasks::FILE_D | Bitmasks::RANK_4);
  expected.clear(Square::D4);

  EXPECT_EQ(bb.value() & (Bitmasks::FILE_D | Bitmasks::RANK_4), expected.value());

  // Towards NE
  EXPECT_TRUE(bb.test(Square::E5));
  EXPECT_TRUE(bb.test(Square::F6));
  EXPECT_TRUE(bb.test(Square::G7));
  EXPECT_TRUE(bb.test(Square::H8));

  // Towards NW
  EXPECT_TRUE(bb.test(Square::C5));
  EXPECT_TRUE(bb.test(Square::B6));
  EXPECT_TRUE(bb.test(Square::A7));

  // Towards SE
  EXPECT_TRUE(bb.test(Square::E3));
  EXPECT_TRUE(bb.test(Square::F2));
  EXPECT_TRUE(bb.test(Square::G1));

  // Towards SW
  EXPECT_TRUE(bb.test(Square::C3));
  EXPECT_TRUE(bb.test(Square::B2));
  EXPECT_TRUE(bb.test(Square::A1));
}
