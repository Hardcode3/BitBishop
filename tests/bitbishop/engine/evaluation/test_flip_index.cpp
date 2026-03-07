#include <gtest/gtest.h>

#include <bitbishop/engine/evaluation.hpp>

using namespace Eval;

TEST(FlipIndex, IsInvolution) {
  for (std::size_t i = 0; i < Const::BOARD_SIZE; ++i) {
    const std::size_t flipped_index = flip_index_vertically(i);
    EXPECT_EQ(i, flip_index_vertically(flipped_index)) << "at index " << i;
  }
}

TEST(FlipIndex, PreservesFileAndFlipsRank) {
  using namespace Const;

  for (std::size_t rank = 0; rank < BOARD_WIDTH; ++rank) {
    for (std::size_t file = 0; file < BOARD_WIDTH; ++file) {
      std::size_t i = rank * BOARD_WIDTH + file;
      std::size_t j = flip_index_vertically(i);

      EXPECT_EQ(j % BOARD_WIDTH, file);                    // same file
      EXPECT_EQ(j / BOARD_WIDTH, BOARD_WIDTH - 1 - rank);  // flipped rank
    }
  }
}

TEST(FlipIndex, FlipsCornersCorrectly) {
  EXPECT_EQ(flip_index_vertically(0), 56);  // A8 -> A1
  EXPECT_EQ(flip_index_vertically(7), 63);  // H8 -> H1
  EXPECT_EQ(flip_index_vertically(56), 0);  // A1 -> A8
  EXPECT_EQ(flip_index_vertically(63), 7);  // H1 -> H8
}