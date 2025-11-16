#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/lookups/bishop.hpp>
#include <bitbishop/square.hpp>

TEST(BishopAttacksTest, CornerA1) {
  Bitboard bb = Lookups::BISHOP_ATTACKS[Square::A1];

  EXPECT_TRUE(bb.test(Square::B2));
  EXPECT_TRUE(bb.test(Square::C3));
  EXPECT_TRUE(bb.test(Square::D4));
  EXPECT_TRUE(bb.test(Square::E5));
  EXPECT_TRUE(bb.test(Square::F6));
  EXPECT_TRUE(bb.test(Square::G7));
  EXPECT_TRUE(bb.test(Square::H8));
}

TEST(BishopAttacksTest, CornerA8) {
  Bitboard bb = Lookups::BISHOP_ATTACKS[Square::A8];

  EXPECT_TRUE(bb.test(Square::B7));
  EXPECT_TRUE(bb.test(Square::C6));
  EXPECT_TRUE(bb.test(Square::D5));
  EXPECT_TRUE(bb.test(Square::E4));
  EXPECT_TRUE(bb.test(Square::F3));
  EXPECT_TRUE(bb.test(Square::G2));
  EXPECT_TRUE(bb.test(Square::H1));
}

TEST(BishopAttacksTest, CornerH1) {
  Bitboard bb = Lookups::BISHOP_ATTACKS[Square::H1];

  EXPECT_TRUE(bb.test(Square::A8));
  EXPECT_TRUE(bb.test(Square::B7));
  EXPECT_TRUE(bb.test(Square::C6));
  EXPECT_TRUE(bb.test(Square::D5));
  EXPECT_TRUE(bb.test(Square::E4));
  EXPECT_TRUE(bb.test(Square::F3));
  EXPECT_TRUE(bb.test(Square::G2));
}

TEST(BishopAttacksTest, CornerH8) {
  Bitboard bb = Lookups::BISHOP_ATTACKS[Square::H8];

  EXPECT_TRUE(bb.test(Square::A1));
  EXPECT_TRUE(bb.test(Square::B2));
  EXPECT_TRUE(bb.test(Square::C3));
  EXPECT_TRUE(bb.test(Square::D4));
  EXPECT_TRUE(bb.test(Square::E5));
  EXPECT_TRUE(bb.test(Square::F6));
  EXPECT_TRUE(bb.test(Square::G7));
}

TEST(BishopAttacksTest, CenterD4) {
  Bitboard bb = Lookups::BISHOP_ATTACKS[Square::D4];

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
