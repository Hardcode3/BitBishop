#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/config.hpp>
#include <bitbishop/square.hpp>

/**
 * @test Verify default constructor.
 * @brief Ensures a new Bitboard starts empty (all bits = 0).
 */
TEST(BitboardTest, DefaultConstructorInitializesToZero) {
  Bitboard bb;
  EXPECT_EQ(bb.value(), 0ULL);
}

/**
 * @test Construct from a raw 64-bit value.
 * @brief Confirms the stored value matches the given constant.
 */
TEST(BitboardTest, ConstructorFromValue) {
  Bitboard bb(0xF0F0F0F0F0F0F0F0ULL);
  EXPECT_EQ(bb.value(), 0xF0F0F0F0F0F0F0F0ULL);
}

/**
 * @test Copy constructor with modified bitboards.
 * @brief Verifies that the copy is independent from the original.
 */
TEST(BitboardTest, CopyConstructor) {
  Bitboard bb;
  bb.set(Square::E4);
  bb.set(Square::D4);
  Bitboard cp(bb);
  EXPECT_EQ(bb, cp);
  cp.set(Square::F4);
  EXPECT_NE(bb, cp);
  EXPECT_TRUE(bb.test(Square::E4));
  EXPECT_FALSE(bb.test(Square::F4));
  EXPECT_TRUE(cp.test(Square::F4));
}

/**
 * @test Copy constructor with an empty bitboard.
 * @brief Confirms the copy matches the original empty state.
 */
TEST(BitboardTest, CopyConstructorEmptyBitboard) {
  Bitboard bb;
  Bitboard cp(bb);
  EXPECT_EQ(bb, cp);
}

/**
 * @test Copy constructor with a full bitboard.
 * @brief Confirms the copy matches the original with all squares set.
 */
TEST(BitboardTest, CopyConstructorFullBitboard) {
  Bitboard bb;
  for (int i = 0; i < 64; ++i) {
    bb.set(static_cast<Square>(i));
  }
  Bitboard cp(bb);
  EXPECT_EQ(bb, cp);
}

/**
 * @test Copy constructor is constexpr.
 * @brief Verifies that the copy constructor can be evaluated at compile time.
 */
TEST(BitboardTest, CopyConstructorIsConstexpr) {
  CX_CONST Bitboard bb(0x00000000000000FFULL);
  CX_CONST Bitboard cp(bb);
  VALIDATE_CX(cp.value() == 0x00000000000000FFULL);
  EXPECT_EQ(cp.value(), 0x00000000000000FFULL);
}

/**
 * @test Static Bitboard::Zeros() constructor.
 * @brief Verifies that the constructor can be evaluated at compile time.
 */
TEST(BitboardTest, ZerosStaticConstructor) {
  CX_CONST Bitboard bb = Bitboard::Zeros();

  VALIDATE_CX(bb.value() == 0ULL);
  EXPECT_EQ(bb.value(), 0ULL);
}

/**
 * @test Static Bitboard::Ones() constructor.
 * @brief Verifies that the constructor can be evaluated at compile time.
 */
TEST(BitboardTest, OnesStaticConstructor) {
  CX_CONST Bitboard bb = Bitboard::Ones();

  VALIDATE_CX(bb.value() == (~0ULL));
  EXPECT_EQ(bb.value(), (~0ULL));
}

/**
 * @test Bitboard(Square) constructor.
 * @brief Verifies that the constructor can be evaluated at compile time.
 */
TEST(BitboardConstructorsTest, SquareConstructorIsConstexpr) {
  CX_CONST Bitboard bb = Bitboard(Squares::A4);

  VALIDATE_CX(bb.test(Squares::A4) == true);
  EXPECT_TRUE(bb.test(Squares::A4));
  EXPECT_EQ(bb.count(), 1);
}
