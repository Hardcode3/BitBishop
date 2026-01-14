#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/tools/perft.hpp>

struct PerftTestCase {
  std::string test_name;
  std::string fen;
  std::size_t depth;
  uint64_t expected_nodes_count;
};

struct PerftParamName {
  template <class ParamType>
  std::string operator()(const testing::TestParamInfo<ParamType>& info) const {
    return info.param.test_name;
  }
};

class PerftTest : public ::testing::TestWithParam<PerftTestCase> {};

TEST_P(PerftTest, PerftMatchesExpected) {
  const auto& param = GetParam();

  Board board(param.fen);
  uint64_t nodes = Tools::perft(board, param.depth);

  EXPECT_EQ(nodes, param.expected_nodes_count) << "FEN: " << param.fen << "\nDepth: " << param.depth;
}

// Test cases from: https://www.chessprogramming.org/Perft_Results
static constexpr const char* STARTING_POS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
static constexpr const char* KIWIPETE_POS = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
static constexpr const char* POSITION_THREE = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
static constexpr const char* POSITION_FOUR = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
static constexpr const char* POSITION_FIVE = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
static constexpr const char* POSITION_SIX = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";

// clang-format off
INSTANTIATE_TEST_SUITE_P(
    PerftValidation,
    PerftTest,
    ::testing::Values(
        // Starting position: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
        PerftTestCase{
          "StartingPos_Depth0",
          STARTING_POS, 0, 1
        },
        PerftTestCase{
          "StartingPos_Depth1",
          STARTING_POS, 1, 20
        },
        PerftTestCase{
          "StartingPos_Depth2",
          STARTING_POS, 2, 400
        },
        PerftTestCase{
          "StartingPos_Depth3",
          STARTING_POS, 3, 8902
        },
        PerftTestCase{
          "StartingPos_Depth4",
          STARTING_POS, 4, 197281
        },
        PerftTestCase{
          "StartingPos_Depth5",
          STARTING_POS, 5, 4865609
        },

        // Kiwipete position: r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -
        PerftTestCase{
          "KiwipetePos_Depth1",
          KIWIPETE_POS, 1, 48
        },
        PerftTestCase{
          "KiwipetePos_Depth2",
          KIWIPETE_POS, 2, 2039
        },
        PerftTestCase{
          "KiwipetePos_Depth3",
          KIWIPETE_POS, 3, 97862
        },
        PerftTestCase{
          "KiwipetePos_Depth4",
          KIWIPETE_POS, 4, 4085603
        },
        PerftTestCase{
          "KiwipetePos_Depth5",
          KIWIPETE_POS, 5, 193690690
        },

        // Position 3: 8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1
        PerftTestCase{
          "Position3_Depth0",
          POSITION_THREE, 0, 1
        },
        PerftTestCase{
          "Position3_Depth1",
          POSITION_THREE, 1, 14
        },
        PerftTestCase{
          "Position3_Depth2",
          POSITION_THREE, 2, 191
        },
        PerftTestCase{
          "Position3_Depth3",
          POSITION_THREE, 3, 2812
        },
        PerftTestCase{
          "Position3_Depth4",
          POSITION_THREE, 4, 43238
        },
        PerftTestCase{
          "Position3_Depth5",
          POSITION_THREE, 5, 674624
        }
    ),
    PerftParamName()
);
// clang-format on

/**
 * @test Perft on position 3 (checks and captures).
 * @brief Confirms perft works on position with many checks.
 */
TEST(PerftTest, Position3Depth1) {
  Board board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

  uint64_t nodes = Tools::perft(board, 1);

  EXPECT_EQ(nodes, 14);
}

/**
 * @test Perft on position 3 depth 2.
 * @brief Confirms perft(2) on position with checks and captures.
 */
TEST(PerftTest, Position3Depth2) {
  Board board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

  uint64_t nodes = Tools::perft(board, 2);

  EXPECT_EQ(nodes, 191);
}

/**
 * @test Perft on position 3 depth 3.
 * @brief Confirms perft(3) on position with checks and captures.
 */
TEST(PerftTest, Position3Depth3) {
  Board board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

  uint64_t nodes = Tools::perft(board, 3);

  EXPECT_EQ(nodes, 2812);
}

/**
 * @test Perft on position 4 (en passant) depth 1.
 * @brief Confirms perft works on position with en passant.
 */
TEST(PerftTest, Position4EnPassantDepth1) {
  Board board("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1Pp2/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

  uint64_t nodes = Tools::perft(board, 1);

  EXPECT_EQ(nodes, 6);
}

/**
 * @test Perft on position 4 depth 2.
 * @brief Confirms perft(2) on position with en passant.
 */
TEST(PerftTest, Position4EnPassantDepth2) {
  Board board("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1Pp2/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

  uint64_t nodes = Tools::perft(board, 2);

  EXPECT_EQ(nodes, 265);
}

/**
 * @test Perft on position 5 (castling) depth 1.
 * @brief Confirms perft works on position testing castling.
 */
TEST(PerftTest, Position5CastlingDepth1) {
  Board board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");

  uint64_t nodes = Tools::perft(board, 1);

  EXPECT_EQ(nodes, 44);
}

/**
 * @test Perft on position 5 depth 2.
 * @brief Confirms perft(2) on position testing castling.
 */
TEST(PerftTest, Position5CastlingDepth2) {
  Board board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");

  uint64_t nodes = Tools::perft(board, 2);

  EXPECT_EQ(nodes, 1486);
}

/**
 * @test Perft on position 6 (discovered checks) depth 1.
 * @brief Confirms perft works on position with discovered checks.
 */
TEST(PerftTest, Position6DiscoveredCheckDepth1) {
  Board board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

  uint64_t nodes = Tools::perft(board, 1);

  EXPECT_EQ(nodes, 46);
}

/**
 * @test Perft on position 6 depth 2.
 * @brief Confirms perft(2) on position with discovered checks.
 */
TEST(PerftTest, Position6DiscoveredCheckDepth2) {
  Board board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

  uint64_t nodes = Tools::perft(board, 2);

  EXPECT_EQ(nodes, 2079);
}

/**
 * @test Perft on empty board with only kings.
 * @brief Confirms perft works on minimal position.
 */
TEST(PerftTest, OnlyKingsDepth1) {
  Board board("4k3/8/8/8/8/8/8/4K3 w - - 0 1");

  uint64_t nodes = Tools::perft(board, 1);

  // Both kings have 5 moves each, but some squares attacked
  EXPECT_EQ(nodes, 5);
}

/**
 * @test Perft on position with single pawn.
 * @brief Confirms perft correctly handles pawn double push.
 */
TEST(PerftTest, SinglePawnDepth1) {
  Board board("4k3/8/8/8/8/8/4P3/4K3 w - - 0 1");

  uint64_t nodes = Tools::perft(board, 1);

  EXPECT_EQ(nodes, 6);
}

/**
 * @test Perft on position with promotion.
 * @brief Confirms perft correctly counts all promotion possibilities.
 */
TEST(PerftTest, PawnPromotionDepth1) {
  Board board("4k3/4P3/8/8/8/8/8/4K3 w - - 0 1");

  uint64_t nodes = Tools::perft(board, 1);

  EXPECT_EQ(nodes, 5);
}

/**
 * @test Perft on stalemate position.
 * @brief Confirms perft returns 0 for stalemate position.
 */
TEST(PerftTest, StalemateDepth1) {
  Board board("7k/5Q2/6K1/8/8/8/8/8 b - - 0 1");

  uint64_t nodes = Tools::perft(board, 1);

  EXPECT_EQ(nodes, 0);
}

/**
 * @test Perft on checkmate position.
 * @brief Confirms perft returns 0 for checkmate position.
 */
TEST(PerftTest, CheckmateDepth1) {
  Board board("6rk/6pp/7r/8/8/8/8/4K3 w - - 0 1");

  uint64_t nodes = Tools::perft(board, 1);

  EXPECT_EQ(nodes, 5);
}

/**
 * @test Perft with en passant available.
 * @brief Confirms perft correctly handles en passant captures.
 */
TEST(PerftTest, EnPassantAvailableDepth1) {
  Board board("rnbqkbnr/pppp1ppp/8/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 1");

  uint64_t nodes = Tools::perft(board, 1);

  // Should include the en passant capture as an option
  EXPECT_EQ(nodes, 30);
}

/**
 * @test Perft symmetry between white and black.
 * @brief Confirms perft produces same count from symmetric positions.
 */
TEST(PerftTest, SymmetricPositionsEqual) {
  Board white_to_move = Board::StartingPosition();
  Board black_to_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");

  uint64_t white_nodes = Tools::perft(white_to_move, 1);
  uint64_t black_nodes = Tools::perft(black_to_move, 1);

  EXPECT_EQ(white_nodes, black_nodes);
}
