#include <gtest/gtest.h>

#include <bitbishop/piece.hpp>

/**
 * @test Verifies to_char() round-trips correctly.
 */
TEST(PieceTest, ToCharRoundTrips) {
  const char pieces[] = {'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};

  for (char c : pieces) {
    Piece p(c);
    EXPECT_EQ(p.to_char(), c);
  }
}
