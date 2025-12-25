#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/lookups/rook_rays.hpp>
#include <bitbishop/square.hpp>

TEST(RookAttacksTest, CornerA1) {
  Bitboard bb = Lookups::ROOK_RAYS[Square::A1];

  // A1 attacks consit in A file + rank 1 without A1
  Bitboard expected = Bitboard(Bitmasks::FILE_A | Bitmasks::RANK_1);
  expected.clear(Square::A1);

  EXPECT_EQ(bb, expected);
}

TEST(RookAttacksTest, CornerA8) {
  Bitboard bb = Lookups::ROOK_RAYS[Square::A8];

  // A8 attacks consit in A file + rank 8 without A8
  Bitboard expected = Bitboard(Bitmasks::FILE_A | Bitmasks::RANK_8);
  expected.clear(Square::A8);

  EXPECT_EQ(bb, expected);
}

TEST(RookAttacksTest, CornerH1) {
  Bitboard bb = Lookups::ROOK_RAYS[Square::H1];

  // H1 attacks consit in H file + rank 1 without H1
  Bitboard expected = Bitboard(Bitmasks::FILE_H | Bitmasks::RANK_1);
  expected.clear(Square::H1);

  EXPECT_EQ(bb, expected);
}

TEST(RookAttacksTest, CornerH8) {
  Bitboard bb = Lookups::ROOK_RAYS[Square::H8];

  // H8 attacks consit in H file + rank 8 without H8
  Bitboard expected = Bitboard(Bitmasks::FILE_H | Bitmasks::RANK_8);
  expected.clear(Square::H8);

  EXPECT_EQ(bb, expected);
}

TEST(RookAttacksTest, CenterD4) {
  Bitboard bb = Lookups::ROOK_RAYS[Square::D4];

  // D4 attacks consit in D file + rank 4 without D4
  Bitboard expected = Bitboard(Bitmasks::FILE_D | Bitmasks::RANK_4);
  expected.clear(Square::D4);

  EXPECT_EQ(bb, expected);
}
