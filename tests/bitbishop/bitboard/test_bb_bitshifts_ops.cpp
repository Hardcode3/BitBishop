#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/square.hpp>

/**
 * @test Left bitshift operator.
 * @brief Confirms left bitshift operator works as intended.
 */
TEST(BitboardTest, ShiftLeft) {
  Bitboard b1(0x1);                      // binary: ...0001  (only bit 0 set -> square A1)
  EXPECT_EQ((b1 << 1).value(), 0x2ULL);  // binary: ...0010  (only bit 1 set -> square B1)
  EXPECT_EQ((b1 << 3).value(), 0x8ULL);  // binary: ...1000  (only bit 3 set -> square D1)

  Bitboard b2(0x100);                       // binary: ...0000 0001 0000 0000  (only bit 8 set -> square A2)
  EXPECT_EQ((b2 << 4).value(), 0x1000ULL);  // binary: ...0001 0000 0000 0000 (only bit 12 set -> square E2)
}

/**
 * @test Right bitshift operator.
 * @brief Confirms right bitshift operator works as intended.
 */
TEST(BitboardTest, ShiftRight) {
  Bitboard b1(0x8);                      // binary: ...1000  (only bit 3 set -> square D1)
  EXPECT_EQ((b1 >> 3).value(), 0x1ULL);  // binary: ...0001  (only bit 0 set -> square A1)

  Bitboard b2(0x1000);                     // binary: ...0001 0000 0000 0000  (only bit 12 set -> square E2)
  EXPECT_EQ((b2 >> 4).value(), 0x100ULL);  // binary: ...0000 0001 0000 0000  (only bit 8 set -> square A2)
}
