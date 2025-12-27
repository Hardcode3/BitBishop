#include <gtest/gtest.h>

#include <bitbishop/square.hpp>

using namespace Squares;

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

/**
 * @test Same file detection for vertical alignment.
 * @brief Confirms squares on the same file are detected correctly.
 */
TEST(SquareTest, SameFileVerticalAlignment) {
  EXPECT_TRUE(A1.same_file(A8));
  EXPECT_TRUE(E2.same_file(E7));
  EXPECT_TRUE(H1.same_file(H4));
  EXPECT_TRUE(D5.same_file(D5));
}

/**
 * @test Different file detection.
 * @brief Confirms squares on different files are detected correctly.
 */
TEST(SquareTest, SameFileDifferentFiles) {
  EXPECT_FALSE(A1.same_file(B1));
  EXPECT_FALSE(D4.same_file(E4));
  EXPECT_FALSE(H8.same_file(A8));
}

/**
 * @test Same rank detection for horizontal alignment.
 * @brief Confirms squares on the same rank are detected correctly.
 */
TEST(SquareTest, SameRankHorizontalAlignment) {
  EXPECT_TRUE(A1.same_rank(H1));
  EXPECT_TRUE(B4.same_rank(G4));
  EXPECT_TRUE(C8.same_rank(F8));
  EXPECT_TRUE(D5.same_rank(D5));
}

/**
 * @test Different rank detection.
 * @brief Confirms squares on different ranks are detected correctly.
 */
TEST(SquareTest, SameRankDifferentRanks) {
  EXPECT_FALSE(A1.same_rank(A2));
  EXPECT_FALSE(E4.same_rank(E5));
  EXPECT_FALSE(H1.same_rank(H8));
}

/**
 * @test NE-SW diagonal alignment (file - rank constant).
 * @brief Confirms squares on the same NE-SW diagonal are detected.
 */
TEST(SquareTest, SameNESWDiagonalAlignment) {
  EXPECT_TRUE(A1.same_ne_sw_diag(B2));
  EXPECT_TRUE(A1.same_ne_sw_diag(C3));
  EXPECT_TRUE(A1.same_ne_sw_diag(H8));
  EXPECT_TRUE(H8.same_ne_sw_diag(G7));
  EXPECT_TRUE(D4.same_ne_sw_diag(E5));
  EXPECT_TRUE(D4.same_ne_sw_diag(C3));
  EXPECT_TRUE(E5.same_ne_sw_diag(E5));
}

/**
 * @test NE-SW diagonal non-alignment.
 * @brief Confirms squares not on the same NE-SW diagonal are detected.
 */
TEST(SquareTest, SameNESWDiagonalNonAlignment) {
  EXPECT_FALSE(A1.same_ne_sw_diag(A8));
  EXPECT_FALSE(D4.same_ne_sw_diag(D5));
  EXPECT_FALSE(E4.same_ne_sw_diag(D5));
}

/**
 * @test NW-SE diagonal alignment (file + rank constant).
 * @brief Confirms squares on the same NW-SE diagonal are detected.
 */
TEST(SquareTest, SameNWSEDiagonalAlignment) {
  EXPECT_TRUE(A8.same_nw_se_diag(B7));
  EXPECT_TRUE(A8.same_nw_se_diag(H1));
  EXPECT_TRUE(H1.same_nw_se_diag(G2));
  EXPECT_TRUE(D4.same_nw_se_diag(E3));
  EXPECT_TRUE(D4.same_nw_se_diag(C5));
  EXPECT_TRUE(D4.same_nw_se_diag(B6));
  EXPECT_TRUE(E5.same_nw_se_diag(E5));
}

/**
 * @test NW-SE diagonal non-alignment.
 * @brief Confirms squares not on the same NW-SE diagonal are detected.
 */
TEST(SquareTest, SameNWSEDiagonalNonAlignment) {
  EXPECT_FALSE(A1.same_nw_se_diag(H8));
  EXPECT_FALSE(D4.same_nw_se_diag(D5));
  EXPECT_FALSE(E4.same_nw_se_diag(D3));
}

/**
 * @test General diagonal alignment (any diagonal).
 * @brief Confirms squares on either NE-SW or NW-SE diagonals are detected.
 */
TEST(SquareTest, SameDiagonalAnyAlignment) {
  EXPECT_TRUE(A1.same_diag(H8));
  EXPECT_TRUE(D4.same_diag(E5));
  EXPECT_TRUE(A8.same_diag(H1));
  EXPECT_TRUE(D4.same_diag(C5));
  EXPECT_TRUE(E3.same_diag(B6));
  EXPECT_TRUE(D5.same_diag(D5));
}

/**
 * @test General diagonal non-alignment.
 * @brief Confirms squares on neither diagonal type are detected.
 */
TEST(SquareTest, SameDiagonalNoAlignment) {
  EXPECT_FALSE(A1.same_diag(A8));
  EXPECT_FALSE(A1.same_diag(H1));
  EXPECT_FALSE(D4.same_diag(D5));
  EXPECT_FALSE(E4.same_diag(F6));
}

/**
 * @test Edge case: corner squares on main diagonals.
 * @brief Confirms A1-H8 and A8-H1 diagonal relationships.
 */
TEST(SquareTest, SameDiagonalCornerSquares) {
  EXPECT_TRUE(A1.same_ne_sw_diag(H8));
  EXPECT_TRUE(A8.same_nw_se_diag(H1));
  EXPECT_FALSE(A1.same_ne_sw_diag(H1));
  EXPECT_FALSE(A8.same_ne_sw_diag(H1));
}

/**
 * @test Edge case: single square files and ranks.
 * @brief Confirms edge squares correctly identify alignment.
 */
TEST(SquareTest, SameFileRankEdgeCases) {
  EXPECT_TRUE(A1.same_file(A1));
  EXPECT_TRUE(A1.same_rank(A1));
  EXPECT_TRUE(H8.same_file(H1));
  EXPECT_TRUE(H8.same_rank(A8));
}
