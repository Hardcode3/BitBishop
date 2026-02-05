#include <gtest/gtest.h>

#include <bitbishop/engine/evaluation.hpp>

using namespace Eval;
using namespace Squares;

class TestComputeScoreFromPSqT : public ::testing::Test {
 protected:
  PieceSquareTable psqt = {
      // clang-format off
      // This PSQT is built with the values being the mapping to the msb bitboard ordering
      56, 57, 58, 59, 60, 61, 62, 63,  // 1
      48, 49, 50, 51, 52, 53, 54, 55, // 2
      40, 41, 42, 43, 44, 45, 46, 47, // 3
      32, 33, 34, 35, 36, 37, 38, 39, // 4
      24, 25, 26, 27, 28, 29, 30, 31, // 5
      16, 17, 18, 19, 20, 21, 22, 23, // 6
      8 ,  9, 10, 11, 12, 13, 14, 15, // 7
      0 ,  1,  2,  3,  4,  5,  6,  7 // 8
   // A , B , C , D , E , F , G , H <- File
      // clang-format on
  };
};

TEST_F(TestComputeScoreFromPSqT, SingleSquareA1) {
  Bitboard bb;
  bb.set(A1);

  int score = compute_score_from_psqt(psqt, bb);
  EXPECT_EQ(score, static_cast<int>(A1.flat_index()));
}

TEST_F(TestComputeScoreFromPSqT, SingleSquareD4) {
  Bitboard bb;
  bb.set(D4);

  int score = compute_score_from_psqt(psqt, bb);

  EXPECT_EQ(score, static_cast<int>(D4.flat_index()));
}

TEST_F(TestComputeScoreFromPSqT, MultipleSquaresAccumulate) {
  Bitboard bb;
  bb.set(A1);
  bb.set(H1);
  bb.set(D4);

  int expected = static_cast<int>(A1.flat_index() + H1.flat_index() + D4.flat_index());
  int score = compute_score_from_psqt(psqt, bb);

  EXPECT_EQ(score, expected);
}

TEST_F(TestComputeScoreFromPSqT, OrderIndependence) {
  Bitboard bb1, bb2;

  bb1.set(A1);
  bb1.set(D4);

  bb2.set(D4);
  bb2.set(A1);

  EXPECT_EQ(compute_score_from_psqt(psqt, bb1), compute_score_from_psqt(psqt, bb2));
}

TEST_F(TestComputeScoreFromPSqT, EmptyBitboardReturnsZero) {
  Bitboard bb;

  int score = compute_score_from_psqt(psqt, bb);
  EXPECT_EQ(score, 0);
}

TEST_F(TestComputeScoreFromPSqT, FullRankFlip) {
  Bitboard bb;
  int expected = 0;

  for (Square sq : {A1, B1, C1, D1, E1, F1, G1, H1}) {
    bb.set(sq);
    expected += static_cast<int>(sq.flat_index());
  }

  int score = compute_score_from_psqt(psqt, bb);

  EXPECT_EQ(score, expected);
}

TEST_F(TestComputeScoreFromPSqT, FullFileFlip) {
  Bitboard bb;
  int expected = 0;

  for (Square sq : {E1, E2, E3, E4, E5, E6, E7, E8}) {
    bb.set(sq);
    expected += static_cast<int>(sq.flat_index());
  }

  int score = compute_score_from_psqt(psqt, bb);

  EXPECT_EQ(score, expected);
}
