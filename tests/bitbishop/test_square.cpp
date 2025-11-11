#include <gtest/gtest.h>

#include <bitbishop/square.hpp>

// --- Construction Tests ---

/**
 * @test Construct a Square from raw integer values (0 and 63).
 * @brief Verifies correct mapping to "a1" and "h8".
 */
TEST(SquareTest, ConstructFromIntValid) {
  Square s1(0);
  Square s2(63);

  EXPECT_EQ(s1.to_string(), "a1");
  EXPECT_EQ(s2.to_string(), "h8");
}

/**
 * @test Construct a Square from invalid raw integer values.
 * @brief Ensures out-of-range values throw std::invalid_argument.
 */
TEST(SquareTest, ConstructFromIntInvalid) {
  EXPECT_THROW(Square(-1), std::invalid_argument);
  EXPECT_THROW(Square(64), std::invalid_argument);
}

/**
 * @test Construct a Square from valid file/rank.
 * @brief Confirms mapping of (4,3) → "e4".
 */
TEST(SquareTest, ConstructFromFileRankValid) {
  Square sq(4, 3);  // e4
  EXPECT_EQ(sq.file(), 4);
  EXPECT_EQ(sq.rank(), 3);
  EXPECT_EQ(sq.to_string(), "e4");
}

/**
 * @test Construct a Square from invalid file/rank.
 * @brief Ensures out-of-range file or rank throws std::invalid_argument.
 */
TEST(SquareTest, ConstructFromFileRankInvalid) {
  EXPECT_THROW(Square(8, 0), std::invalid_argument);
  EXPECT_THROW(Square(0, -1), std::invalid_argument);
}

/**
 * @test Construct a Square from a valid algebraic string.
 * @brief Confirms "e4" maps to (file=4, rank=3).
 */
TEST(SquareTest, ConstructFromStringValid) {
  Square sq("e4");
  EXPECT_EQ(sq.file(), 4);
  EXPECT_EQ(sq.rank(), 3);
  EXPECT_EQ(sq.to_string(), "e4");
}

/**
 * @test Construct a Square from invalid strings.
 * @brief Ensures malformed input throws std::invalid_argument.
 */
TEST(SquareTest, ConstructFromStringInvalid) {
  EXPECT_THROW(Square("z9"), std::invalid_argument);
  EXPECT_THROW(Square("abc"), std::invalid_argument);
  EXPECT_THROW(Square(""), std::invalid_argument);
}

/**
 * @test Roundtrip conversion check.
 * @brief (file, rank) → string → Square must yield the same Square.
 */
TEST(SquareTest, RoundtripConsistency) {
  for (int f = 0; f < 8; ++f) {
    for (int r = 0; r < 8; ++r) {
      Square sq(f, r);
      std::string s = sq.to_string();
      Square sq2(s);
      EXPECT_EQ(sq, sq2) << "Mismatch at " << s;
    }
  }
}

/**
 * @test Construct a Square from case-insensitive strings.
 * @brief Ensures "E4" and "e4" produce the same Square.
 */
TEST(SquareTest, ConstructFromStringCaseInsensitive) {
  Square s1("E4");
  Square s2("e4");
  EXPECT_EQ(s1, s2);
}

/**
 * @test Construct Squares from boundary strings.
 * @brief Confirms "a1" → (0,0) and "h8" → (7,7).
 */
TEST(SquareTest, ConstructFromStringBounds) {
  Square s1("a1");
  Square s2("h8");
  EXPECT_EQ(s1.file(), 0);
  EXPECT_EQ(s1.rank(), 0);
  EXPECT_EQ(s2.file(), 7);
  EXPECT_EQ(s2.rank(), 7);
}

/**
 * @test File/rank coverage.
 * @brief Ensures file() and rank() are consistent with raw enum values.
 */
TEST(SquareTest, FileAndRankCoverage) {
  for (int i = 0; i < 64; ++i) {
    Square sq(i);
    EXPECT_EQ(sq.value(), static_cast<Square::Value>(i));
    EXPECT_EQ(sq.file() + 8 * sq.rank(), i);
  }
}

// --- Equality ---

/**
 * @test Equality and inequality operators.
 * @brief Confirms correct comparison behavior for Squares.
 */
TEST(SquareTest, EqualityOperators) {
  Square s1("e4");
  Square s2(4, 3);
  Square s3("d5");

  EXPECT_TRUE(s1 == s2);
  EXPECT_FALSE(s1 == s3);
  EXPECT_TRUE(s1 != s3);
}

/**
 * @test Self-equality.
 * @brief Ensures a Square always equals itself.
 */
TEST(SquareTest, EqualitySelf) {
  Square sq("c6");
  EXPECT_TRUE(sq == sq);
  EXPECT_FALSE(sq != sq);
}

// --- Enum Mapping ---

/**
 * @test Enum value mapping.
 * @brief Confirms Square::A1 → "a1" and Square::H8 → "h8".
 */
TEST(SquareTest, EnumValueMapping) {
  Square s1(Square::A1);
  Square s2(Square::H8);

  EXPECT_EQ(s1.to_string(), "a1");
  EXPECT_EQ(s2.to_string(), "h8");
  EXPECT_EQ(s1.file(), 0);
  EXPECT_EQ(s1.rank(), 0);
  EXPECT_EQ(s2.file(), 7);
  EXPECT_EQ(s2.rank(), 7);
}