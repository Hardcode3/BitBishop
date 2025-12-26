#include <gtest/gtest.h>

#include <bitbishop/color.hpp>
#include <bitbishop/lookups/attackers.hpp>
#include <bitbishop/square.hpp>

/**
 * @brief Tests that attackers_to returns non-empty bitboards for obvious central squares.
 */
TEST(AttackersToTest, CentralSquareHasAttackers) {
  Bitboard white_attackers = attackers_to(Squares::E4, Color::WHITE);
  Bitboard black_attackers = attackers_to(Squares::E4, Color::BLACK);

  EXPECT_NE(white_attackers, 0ULL);
  EXPECT_NE(black_attackers, 0ULL);
}

/**
 * @brief Tests that ATTACKERS_TO table matches direct attackers_to calls.
 */
TEST(AttackersToTest, TableMatchesFunction) {
  using namespace Const;

  for (Color c : ColorUtil::all()) {
    const std::size_t ci = ColorUtil::to_index(c);
    for (int sq = 0; sq < BOARD_SIZE; ++sq) {
      Square square(sq);
      EXPECT_EQ(ATTACKERS_TO[ci][sq], attackers_to(square, c))
          << "Mismatch for square " << sq << " color " << static_cast<int>(c);
    }
  }
}

/**
 * @brief Tests pawn attackers specifically for edge files.
 */
TEST(AttackersToTest, PawnEdgeFiles) {
  Bitboard white_attackers = attackers_to(Squares::A2, Color::WHITE);
  Bitboard black_attackers = attackers_to(Squares::H7, Color::BLACK);

  EXPECT_TRUE(white_attackers.test(Squares::B3));
  EXPECT_TRUE(black_attackers.test(Squares::G6));
}
