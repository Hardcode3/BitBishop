#include <gtest/gtest.h>

#include <chess_engine/bitboard.hpp>
#include <chess_engine/bitmasks.hpp>
#include <chess_engine/lookups/king.hpp>
#include <chess_engine/square.hpp>

using namespace Attacks;

/**
 * @test King attacks from A1 corner square.
 */
TEST(KingAttackTest, CornerA1) {
  Bitboard expected;
  expected.set(Square::A2);  // North
  expected.set(Square::B2);  // North-East
  expected.set(Square::B1);  // East

  EXPECT_EQ(KING_ATTACKS[Square::A1], expected);
}

/**
 * @test King attacks from H1 corner square.
 */
TEST(KingAttackTest, CornerH1) {
  Bitboard expected;
  expected.set(Square::G1);  // West
  expected.set(Square::G2);  // North-West
  expected.set(Square::H2);  // North

  EXPECT_EQ(KING_ATTACKS[Square::H1], expected);
}

/**
 * @test King attacks from A8 corner square.
 */
TEST(KingAttackTest, CornerA8) {
  Bitboard expected;
  expected.set(Square::A7);  // South
  expected.set(Square::B7);  // South-East
  expected.set(Square::B8);  // East

  EXPECT_EQ(KING_ATTACKS[Square::A8], expected);
}

/**
 * @test King attacks from H8 corner square.
 */
TEST(KingAttackTest, CornerH8) {
  Bitboard expected;
  expected.set(Square::G8);  // West
  expected.set(Square::G7);  // South-West
  expected.set(Square::H7);  // South

  EXPECT_EQ(KING_ATTACKS[Square::H8], expected);
}

/**
 * @test King attacks from D1 square.
 */
TEST(KingAttackTest, EdgeD1) {
  Bitboard expected;
  expected.set(Square::C1);  // West
  expected.set(Square::E1);  // East
  expected.set(Square::C2);  // North-West
  expected.set(Square::D2);  // North
  expected.set(Square::E2);  // North-East

  EXPECT_EQ(KING_ATTACKS[Square::D1], expected);
}

/**
 * @test King attacks from D4 central square.
 */
TEST(KingAttackTest, CenterD4) {
  Bitboard expected;
  expected.set(Square::C3);  // SW
  expected.set(Square::D3);  // S
  expected.set(Square::E3);  // SE
  expected.set(Square::C4);  // W
  expected.set(Square::E4);  // E
  expected.set(Square::C5);  // NW
  expected.set(Square::D5);  // N
  expected.set(Square::E5);  // NE

  EXPECT_EQ(KING_ATTACKS[Square::D4], expected);
}
