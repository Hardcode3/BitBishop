#include <gtest/gtest.h>

#include <bitbishop/engine/evaluation.hpp>

using namespace Eval;

class TestFlipPSqT : public ::testing::Test {
 protected:
  PieceSquareTable psqt = {
      // clang-format off
      0 ,  1,  2,  3,  4,  5,  6,  7, // 8
      8 ,  9, 10, 11, 12, 13, 14, 15, // 7
      16, 17, 18, 19, 20, 21, 22, 23, // 6
      24, 25, 26, 27, 28, 29, 30, 31, // 5
      32, 33, 34, 35, 36, 37, 38, 39, // 4
      40, 41, 42, 43, 44, 45, 46, 47, // 3
      48, 49, 50, 51, 52, 53, 54, 55, // 2
      56, 57, 58, 59, 60, 61, 62, 63  // 1
   // A , B , C , D , E , F , G , H <- File
      // clang-format on
  };
  PieceSquareTable psqt_flipped = flip_psqt(psqt);
};

TEST_F(TestFlipPSqT, FlipIsInvolution) { EXPECT_EQ(psqt, flip_psqt(psqt_flipped)); }

TEST_F(TestFlipPSqT, UsesFlipIndexMapping) {
  for (std::size_t i = 0; i < Const::BOARD_SIZE; ++i) {
    EXPECT_EQ(psqt[i], psqt_flipped[flip_index_horizontally(i)]) << "at index " << i;
  }
}
