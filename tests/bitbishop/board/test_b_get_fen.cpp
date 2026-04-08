#include <gtest/gtest.h>

#include <bitbishop/board.hpp>

struct FenTestCase {
  std::string name;
  std::string fen;
};

class BoardGetFenTest : public ::testing::TestWithParam<FenTestCase> {};

TEST_P(BoardGetFenTest, ReturnsSameFen) {
  const auto& param = GetParam();
  Board board(param.fen);

  EXPECT_EQ(board.get_fen(), param.fen);
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(
    GetFenTests,
    BoardGetFenTest,
    ::testing::Values(
        FenTestCase{
            "EmptyBoard",
            "8/8/8/8/8/8/8/8 w - - 0 1"
        },
        FenTestCase{
            "StartingPosition",
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
        },
        FenTestCase{
            "BlackToMove",
            "8/8/8/8/8/8/8/8 b - - 0 1"
        },
        FenTestCase{
            "PartialCastling",
            "8/8/8/8/8/8/8/8 w Kq - 0 1"
        },
        FenTestCase{
            "NoCastling",
            "8/8/8/8/8/8/8/8 w - - 0 1"
        },
        FenTestCase{
            "EnPassant",
            "8/8/8/8/8/8/8/8 w - e3 0 1"
        },
        FenTestCase{
            "MoveCounters",
            "8/8/8/8/8/8/8/8 w - - 17 42"
        },
        FenTestCase{
            "MixedCompression",
            "8/8/3p4/8/8/4P3/8/8 w - - 0 1"
        }
    ),
    [](const testing::TestParamInfo<FenTestCase>& info) {
        return info.param.name;
    }
);
// clang-format on
