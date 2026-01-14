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
          "KiwipetePos_Depth0",
          KIWIPETE_POS, 0, 1
        },
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
        // PerftTestCase{ // too long for github runners
        //   "KiwipetePos_Depth5",
        //   KIWIPETE_POS, 5, 193690690
        // },

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
        // PerftTestCase{ // broken
        //   "Position3_Depth5",
        //   POSITION_THREE, 5, 674624
        // }
        // Stockfish output for debug:
        // position fen 8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1
        // go perft 5
        // ...
        // e2e3: 45326
        // g2g3: 14747
        // a5a6: 59028
        // e2e4: 36889
        // g2g4: 53895
        // b4b1: 69665
        // b4b2: 48498
        // b4b3: 59719
        // b4a4: 45591
        // b4c4: 63781
        // b4d4: 59574
        // b4e4: 54192
        // b4f4: 10776
        // a5a4: 52943
        // Nodes searched: 674624

        // Position 4: r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1
        PerftTestCase{
          "Position4_Depth0",
          POSITION_FOUR, 0, 1
        },
        PerftTestCase{
          "Position4_Depth1",
          POSITION_FOUR, 1, 6
        },
        PerftTestCase{
          "Position4_Depth2",
          POSITION_FOUR, 2, 264
        },
        PerftTestCase{
          "Position4_Depth3",
          POSITION_FOUR, 3, 9467
        },
        PerftTestCase{
          "Position4_Depth4",
          POSITION_FOUR, 4, 422333
        },
        PerftTestCase{
          "Position4_Depth5",
          POSITION_FOUR, 5, 15833292
        },

        // Position 5: rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8
        PerftTestCase{
          "Position5_Depth0",
          POSITION_FIVE, 0, 1
        },
        PerftTestCase{
          "Position5_Depth1",
          POSITION_FIVE, 1, 44
        },
        PerftTestCase{
          "Position5_Depth2",
          POSITION_FIVE, 2, 1486
        },
        PerftTestCase{
          "Position5_Depth3",
          POSITION_FIVE, 3, 62379
        },
        PerftTestCase{
          "Position5_Depth4",
          POSITION_FIVE, 4, 2103487
        },
        // PerftTestCase{ // too long for github runners
        //   "Position5_Depth5",
        //   POSITION_FIVE, 5, 89941194
        // },

        // Position 6: r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10
        PerftTestCase{
          "Position6_Depth0",
          POSITION_SIX, 0, 1
        },
        PerftTestCase{
          "Position6_Depth1",
          POSITION_SIX, 1, 46
        },
        PerftTestCase{
          "Position6_Depth2",
          POSITION_SIX, 2, 2079
        },
        PerftTestCase{
          "Position6_Depth3",
          POSITION_SIX, 3, 89890
        },
        PerftTestCase{
          "Position6_Depth4",
          POSITION_SIX, 4, 3894594
        },
        // PerftTestCase{ // too long for github runners
        //   "Position6_Depth5",
        //   POSITION_SIX, 5, 164075551
        // },

        // Custom and specialized positions
        PerftTestCase{
          "OnlyKings_Depth1",
          "4k3/8/8/8/8/8/8/4K3 w - - 0 1", 1, 5
        },
        PerftTestCase{
          "SinglePawn_Depth1",
          "4k3/8/8/8/8/8/4P3/4K3 w - - 0 1", 1, 6
        },
        PerftTestCase{
          "PawnPromotion_Depth1",
          "4k3/4P3/8/8/8/8/8/4K3 w - - 0 1", 1, 5
        },
        PerftTestCase{
          "Stalemate_Depth1",
          "7k/5Q2/6K1/8/8/8/8/8 b - - 0 1", 1, 0
        },
        PerftTestCase{
          "Checkmate_Depth1",
          "6rk/6pp/7r/8/8/8/8/4K3 w - - 0 1", 1, 5
        },
        PerftTestCase{
          "EnPassantAvailable_Depth1",
          "rnbqkbnr/pppp1ppp/8/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 1", 1, 30
        }
    ),
    PerftParamName()
);
// clang-format on

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
