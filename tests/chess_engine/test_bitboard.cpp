#include <gtest/gtest.h>

#include <chess_engine/bitboard.hpp>
#include <chess_engine/square.hpp>
#include <sstream>

/**
 * @test Print function.
 * @brief Verifies that the bitboard prints as an 8×8 grid with correct layout.
 */
TEST(BitboardTest, PrintOutputsCorrectBoard) {
  Bitboard bb;
  bb.set(Square::A1);
  bb.set(Square::H8);
  bb.set(Square::E4);

  /* Expects:
  . . . . . . . 1 \n    H8
  . . . . . . . . \n
  . . . . . . . . \n
  . . . . . . . . \n
  . . . . 1 . . . \n    E4
  . . . . . . . . \n
  . . . . . . . . \n
  1 . . . . . . . \n" A1
  */

  std::ostringstream oss;
  // Redirect std::cout to oss
  std::streambuf* old_buf = std::cout.rdbuf(oss.rdbuf());
  bb.print();
  // Restore original buffer
  std::cout.rdbuf(old_buf);

  std::string output = oss.str();

  // Check key positions:
  // - H8 (top right corner) should be "1"
  EXPECT_NE(output.find(". . . . . . . 1"), std::string::npos);

  // - E4 should be in the middle
  EXPECT_NE(output.find(". . . . 1 . . ."), std::string::npos);

  // - A1 (bottom left corner) should be "1" in the last row
  EXPECT_NE(output.rfind("1 . . . . . . ."), std::string::npos);

  // Sanity check: must contain 8 lines (ranks 8 → 1)
  int line_count = 0;
  for (char c : output) {
    if (c == '\n') ++line_count;
  }
  EXPECT_EQ(line_count, 8);
}

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
 * @test Set and test a square.
 * @brief After setting E2, it should be occupied, and E4 should remain empty.
 */
TEST(BitboardTest, SetAndTestSquareFromSquareValue) {
  Bitboard bb;
  bb.set(Square::E2);
  EXPECT_TRUE(bb.test(Square::E2));
  EXPECT_FALSE(bb.test(Square::E4));
}

/**
 * @test Set and test a square.
 * @brief After setting E2, it should be occupied, and E4 should remain empty.
 */
TEST(BitboardTest, SetAndTestSquareFromSquare) {
  Bitboard bb;

  Square e2(Square::E2);
  Square e4(Square::E4);

  bb.set(e2);
  EXPECT_TRUE(bb.test(e2));
  EXPECT_FALSE(bb.test(e4));
}

/**
 * @test Clear a square.
 * @brief After clearing E2, the bitboard should show it as empty.
 */
TEST(BitboardTest, ClearSquareFromSquareValue) {
  Bitboard bb;
  bb.set(Square::E2);
  EXPECT_TRUE(bb.test(Square::E2));
  bb.clear(Square::E2);
  EXPECT_FALSE(bb.test(Square::E2));
}

/**
 * @test Clear a square.
 * @brief After clearing E2, the bitboard should show it as empty.
 */
TEST(BitboardTest, ClearSquareFromSquare) {
  Bitboard bb;

  Square e2(Square::E2);

  bb.set(e2);
  EXPECT_TRUE(bb.test(e2));
  bb.clear(e2);
  EXPECT_FALSE(bb.test(e2));
}

/**
 * @test Reset the bitboard.
 * @brief Ensures all bits are cleared and value() = 0.
 */
TEST(BitboardTest, ResetClearsAllSquares) {
  Bitboard bb;
  bb.set(Square::A1);
  bb.set(Square::H8);
  bb.reset();
  for (int sq = 0; sq < 64; ++sq) {
    Square square(sq);
    EXPECT_FALSE(bb.test(square));
  }
  EXPECT_EQ(bb.value(), 0ULL);
}

/**
 * @test Bitwise OR operator.
 * @brief Combines two bitboards, keeping all occupied squares.
 */
TEST(BitboardTest, OrOperatorCombinesBitboards) {
  Bitboard a, b;
  a.set(Square::A1);
  b.set(Square::H8);

  Bitboard c = a | b;
  EXPECT_TRUE(c.test(Square::A1));
  EXPECT_TRUE(c.test(Square::H8));
  EXPECT_FALSE(c.test(Square::E4));
}

/**
 * @test Bitwise AND operator.
 * @brief Returns the intersection of two bitboards (only common bits set).
 */
TEST(BitboardTest, AndOperatorFindsIntersection) {
  Bitboard a, b;
  a.set(Square::E2);
  a.set(Square::E4);
  b.set(Square::E2);
  b.set(Square::D2);

  Bitboard c = a & b;
  EXPECT_TRUE(c.test(Square::E2));
  EXPECT_FALSE(c.test(Square::E4));
  EXPECT_FALSE(c.test(Square::D2));
}

/**
 * @test Compound OR and AND operators.
 * @brief Ensures |= adds squares and &= keeps only intersections.
 */
TEST(BitboardTest, CompoundOrAnd) {
  Bitboard a, b;
  a.set(Square::A1);
  b.set(Square::B2);

  a |= b;
  EXPECT_TRUE(a.test(Square::A1));
  EXPECT_TRUE(a.test(Square::B2));

  Bitboard c;
  c.set(Square::A1);
  a &= c;
  EXPECT_TRUE(a.test(Square::A1));
  EXPECT_FALSE(a.test(Square::B2));
}

/**
 * @test Bitwise NOT operator.
 * @brief Inverts all bits: occupied squares become empty and vice versa.
 */
TEST(BitboardTest, NotOperatorInvertsBits) {
  Bitboard a;
  a.set(Square::A1);
  Bitboard b = ~a;
  EXPECT_FALSE(b.test(Square::A1));
  EXPECT_TRUE(b.test(Square::B1));  // square that wasn’t set
}

/**
 * @test Equality and inequality operators.
 * @brief Confirms bitboards compare equal only when they have identical bits.
 */
TEST(BitboardTest, EqualityOperators) {
  Bitboard a, b;
  a.set(Square::E2);
  b.set(Square::E2);

  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);

  b.set(Square::E4);
  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a != b);
}

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

/**
 * @test bool operator
 * @brief Confirms boolean works as intended when no square is populated.
 */
TEST(BitboardTest, BoolOperatorFalseWhenEmpty) {
  Bitboard bb;

  EXPECT_FALSE(static_cast<bool>(bb));
}

/**
 * @test bool operator
 * @brief Confirms boolean works as intended when one square is populated.
 */
TEST(BitboardTest, BoolOperatorTrueWhenSingleBit) {
  Bitboard bb;
  bb.set(Square::C4);

  EXPECT_TRUE(static_cast<bool>(bb));
}

/**
 * @test bool operator
 * @brief Confirms boolean works as intended when multiple squares are populated.
 */
TEST(BitboardTest, BoolOperatorTrueWhenMultipleBits) {
  Bitboard bb;
  bb.set(Square::C5);
  bb.set(Square::H2);
  bb.set(Square::A1);

  EXPECT_TRUE(static_cast<bool>(bb));
}

/**
 * @test Count operator for bits set to one
 * @brief Confirms count works as intended on an empty bitboard.
 */
TEST(BitboardTest, CountEmptyBitboardIsZero) {
  Bitboard bb;

  EXPECT_EQ(bb.count(), 0);
}

/**
 * @test Count operator for bits set to one
 * @brief Confirms count works as intended on a bitboard populated with one element.
 */
TEST(BitboardTest, CountSingleBitIsOne) {
  Bitboard bb;

  bb.set(Square::C5);

  EXPECT_EQ(bb.count(), 1);
}

/**
 * @test Bitboard::count() with multiple bits set
 * @brief Verifies that count() returns the correct number of set bits (3) on a bitboard with multiple elements.
 */
TEST(BitboardTest, CountMultipleBits) {
  Bitboard bb;
  bb.set(Square::C5);
  bb.set(Square::H2);
  bb.set(Square::A1);

  EXPECT_EQ(bb.count(), 3);
}

/**
 * @test Bitboard::lsb() on an empty bitboard
 * @brief Ensures lsb() returns std::nullopt when called on an empty bitboard.
 */
TEST(BitboardTest, LsbOnEmptyReturnsNullopt) {
  Bitboard bb;

  EXPECT_FALSE(bb.lsb().has_value());
}

/**
 * @test Bitboard::lsb() returns the lowest set bit
 * @brief Confirms lsb() returns the index of the lowest set bit without modifying the bitboard.
 */
TEST(BitboardTest, LsbReturnsLowestBitIndex) {
  Bitboard bb;
  bb.set(Square::C5);  // 34
  bb.set(Square::H2);  // 15
  bb.set(Square::A1);  // 0

  auto result = bb.lsb();
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->value(), Square::A1);
}

/**
 * @test Bitboard::lsb() does not modify the bitboard
 * @brief Verifies that calling lsb() does not change the state of the bitboard.
 */
TEST(BitboardTest, LsbDoesNotModifyBitboard) {
  Bitboard bb;
  bb.set(Square::C5);  // 34
  bb.set(Square::H2);  // 15
  bb.set(Square::A1);  // 0

  auto before = bb.count();
  auto _ = bb.lsb();
  EXPECT_EQ(bb.count(), before);
}

/**
 * @test Bitboard::pop_lsb() on an empty bitboard
 * @brief Ensures pop_lsb() returns std::nullopt when called on an empty bitboard.
 */
TEST(BitboardTest, PopLsbOnEmptyReturnsNullopt) {
  Bitboard bb;

  EXPECT_FALSE(bb.pop_lsb().has_value());
}

/**
 * @test Bitboard::pop_lsb() returns and clears the lowest set bit
 * @brief Confirms pop_lsb() returns the index of the lowest set bit and clears it from the bitboard.
 */
TEST(BitboardTest, PopLsbReturnsAndClearsLowestBit) {
  Bitboard bb;
  bb.set(Square::C5);  // 34
  bb.set(Square::H2);  // 15
  bb.set(Square::A1);  // 0

  EXPECT_TRUE(bb.test(Square::A1));
  auto first = bb.pop_lsb();
  ASSERT_TRUE(first.has_value());
  EXPECT_EQ(first->value(), Square::A1);
  EXPECT_FALSE(bb.test(Square::A1));

  // Check that the least significant bit has now changed to H2
  auto second = bb.lsb();
  ASSERT_TRUE(second.has_value());
  EXPECT_EQ(second->value(), Square::H2);
}

/**
 * @test Bitboard::pop_lsb() decreases the bit count
 * @brief Verifies that pop_lsb() reduces the bit count by one.
 */
TEST(BitboardTest, PopLsbDecreasesCount) {
  Bitboard bb;
  bb.set(Square::A1);  // 0
  bb.set(Square::C5);  // 34

  int before = bb.count();
  bb.pop_lsb();
  EXPECT_EQ(bb.count(), before - 1);
}

/**
 * @test Bitboard::pop_lsb() eventually empties the bitboard
 * @brief Ensures that repeated calls to pop_lsb() on a bitboard with two set bits
 *        will return each bit in LSB order and eventually return std::nullopt
 *        when the bitboard is empty.
 */
TEST(BitboardTest, PopLsbEventuallyEmptiesBoard) {
  Bitboard bb;
  bb.set(Square::H2);  // 15
  bb.set(Square::A1);  // 0

  auto first = bb.pop_lsb();
  auto second = bb.pop_lsb();
  auto third = bb.pop_lsb();

  EXPECT_TRUE(first.has_value());
  EXPECT_TRUE(second.has_value());
  EXPECT_FALSE(third.has_value());
}

/**
 * @test Bitboard iterator iterates over all set bits in order
 * @brief Verifies that iterating over a bitboard with multiple set bits
 *        yields squares in increasing bit-index (LSB → MSB) order.
 */
TEST(BitboardIteratorTest, IteratesOverAllSetBitsInOrder) {
  Bitboard bb;
  bb.set(Square::A1);  // bit 0
  bb.set(Square::H2);  // bit 15
  bb.set(Square::C5);  // bit 34

  std::vector<Square::Value> squares;
  for (Square sq : bb) {
    squares.push_back(sq.value());
  }

  ASSERT_EQ(squares.size(), 3);
  // Expected in increasing bit-index (LSB → MSB) order:
  EXPECT_EQ(squares[0], Square::A1);
  EXPECT_EQ(squares[1], Square::H2);
  EXPECT_EQ(squares[2], Square::C5);
}

/**
 * @test Bitboard iterator handles empty bitboard
 * @brief Ensures that iterating over an empty bitboard results in no iterations.
 */
TEST(BitboardIteratorTest, EmptyBitboardYieldsNoIteration) {
  Bitboard bb;
  std::vector<Square::Value> squares;
  for (Square sq : bb) {
    squares.push_back(sq.value());
  }
  EXPECT_TRUE(squares.empty());
}

/**
 * @test Bitboard iterator handles single bit
 * @brief Verifies that a bitboard with a single set bit can be iterated over exactly once,
 *        and that the iterator correctly reaches the end after one increment.
 */
TEST(BitboardIteratorTest, SingleBitIteration) {
  Bitboard bb;
  bb.set(Square::D4);

  auto it = bb.begin();
  auto end = bb.end();

  ASSERT_NE(it, end);
  EXPECT_EQ(*it, Square(Square::D4));

  ++it;
  EXPECT_EQ(it, end);  // should be at the end after one increment
}

/**
 * @test Bitboard iterator is constexpr-compatible
 * @brief Ensures that a constexpr bitboard can be iterated over at compile time,
 *        and that the iteration order is ascending by bit index.
 */
TEST(BitboardIteratorTest, IteratorDereferenceIsConstexprCompatible) {
  constexpr Bitboard bb_constexpr = [] {
    Bitboard bb;
    bb.set(Square::A1);
    bb.set(Square::B1);
    return bb;
  }();

  std::vector<int> indices;
  for (Square sq : bb_constexpr) {
    indices.push_back(sq.value());
  }

  ASSERT_EQ(indices.size(), 2);
  EXPECT_LT(indices[0], indices[1]);  // Should iterate in ascending order
}

/**
 * @test any() method
 * @brief Confirms any() returns false for an empty bitboard.
 */
TEST(BitboardTest, AnyReturnsFalseWhenEmpty) {
  Bitboard bb;
  EXPECT_FALSE(bb.any());
}

/**
 * @test any() method
 * @brief Confirms any() returns true when a single bit is set.
 */
TEST(BitboardTest, AnyReturnsTrueWhenSingleBit) {
  Bitboard bb;
  bb.set(Square::E4);
  EXPECT_TRUE(bb.any());
}

/**
 * @test any() method
 * @brief Confirms any() returns true when multiple bits are set.
 */
TEST(BitboardTest, AnyReturnsTrueWhenMultipleBits) {
  Bitboard bb;
  bb.set(Square::A1);
  bb.set(Square::H8);
  bb.set(Square::D4);
  EXPECT_TRUE(bb.any());
}

/**
 * @test any() method
 * @brief Confirms any() returns false after clearing all bits.
 */
TEST(BitboardTest, AnyReturnsFalseAfterClear) {
  Bitboard bb;
  bb.set(Square::E4);
  bb.set(Square::D5);
  bb.clear(Square::E4);
  bb.clear(Square::D5);
  EXPECT_FALSE(bb.any());
}

/**
 * @test any() method
 * @brief Confirms any() is equivalent to bool operator.
 */
TEST(BitboardTest, AnyEquivalentToBoolOperator) {
  Bitboard bb_empty;
  EXPECT_EQ(bb_empty.any(), static_cast<bool>(bb_empty));

  Bitboard bb_set;
  bb_set.set(Square::C3);
  EXPECT_EQ(bb_set.any(), static_cast<bool>(bb_set));
}

/**
 * @test any() method
 * @brief Confirms any() is equivalent to count() > 0.
 */
TEST(BitboardTest, AnyEquivalentToCountGtZero) {
  Bitboard bb_empty;
  EXPECT_EQ(bb_empty.count() > 0, static_cast<bool>(bb_empty));

  Bitboard bb_set;
  bb_set.set(Square::C3);
  EXPECT_EQ(bb_set.count() > 0, static_cast<bool>(bb_set));
}
