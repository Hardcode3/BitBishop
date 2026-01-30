#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/square.hpp>

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
  EXPECT_EQ(*it, Squares::D4);

  ++it;
  EXPECT_EQ(it, end);  // should be at the end after one increment
}

/**
 * @test Bitboard iterator is constexpr-compatible
 * @brief Ensures that a constexpr bitboard can be iterated over at compile time,
 *        and that the iteration order is ascending by bit index.
 */
TEST(BitboardIteratorTest, IteratorDereferenceIsConstexprCompatible) {
  CX_CONST Bitboard bb_constexpr = [] {
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
