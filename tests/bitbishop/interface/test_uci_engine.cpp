#include <gtest/gtest.h>

#include <bitbishop/interface/uci_engine.hpp>
#include <sstream>

using namespace Squares;

class UciEngineTest : public ::testing::Test {
 protected:
  std::stringstream input;
  std::stringstream output;

  void SetUp() override {
    // Clear streams before each test
    input.clear();
    output.clear();
  }

  void TearDown() override {
    // Clean up after each test
    input.str("");
    output.str("");
  }
};

TEST_F(UciEngineTest, UciCommandOutputsIdAndOk) {
  Uci::UciEngine engine(input, output);

  engine.dispatch("uci");

  const std::string res = output.str();
  EXPECT_NE(res.find("id name BitBishop"), std::string::npos);
  EXPECT_NE(res.find("id author Hardcode"), std::string::npos);
  EXPECT_NE(res.find("uciok"), std::string::npos);
}

TEST_F(UciEngineTest, IsReadyCommandOutputsReadyOk) {
  Uci::UciEngine engine(input, output);

  engine.dispatch("isready");

  EXPECT_EQ(output.str(), "readyok\n");
}

TEST_F(UciEngineTest, PositionStartposMovesAppliesMoves) {
  Uci::UciEngine engine(input, output);

  engine.dispatch("position startpos moves e2e4");

  const Board &board = engine.get_board();
  auto moved = board.get_piece(E4);
  auto origin = board.get_piece(E2);

  ASSERT_TRUE(moved.has_value());
  EXPECT_TRUE(moved->is_white());
  EXPECT_TRUE(moved->is_pawn());
  EXPECT_FALSE(origin.has_value());
}

TEST_F(UciEngineTest, PositionFenMovesAppliesMoves) {
  Uci::UciEngine engine(input, output);

  engine.dispatch("position fen 8/8/8/8/8/8/4P3/4K3 w - - 0 1 moves e2e4");

  const Board &board = engine.get_board();
  auto moved = board.get_piece(E4);
  auto origin = board.get_piece(E2);

  ASSERT_TRUE(moved.has_value());
  EXPECT_TRUE(moved->is_white());
  EXPECT_TRUE(moved->is_pawn());
  EXPECT_FALSE(origin.has_value());
}

TEST_F(UciEngineTest, UciNewGameResetsBoard) {
  Uci::UciEngine engine(input, output);

  engine.dispatch("position startpos moves e2e4");
  engine.dispatch("ucinewgame");

  const Board &board = engine.get_board();
  auto start_pawn = board.get_piece(E2);
  auto moved = board.get_piece(E4);

  ASSERT_TRUE(start_pawn.has_value());
  EXPECT_TRUE(start_pawn->is_white());
  EXPECT_TRUE(start_pawn->is_pawn());
  EXPECT_FALSE(moved.has_value());
}

TEST_F(UciEngineTest, GoWithoutPositionUsesStartpos) {
  {
    Uci::UciEngine engine(input, output);
    engine.dispatch("go depth 1");
  }

  const std::string res = output.str();
  EXPECT_NE(res.find("bestmove "), std::string::npos);
}

TEST_F(UciEngineTest, UnknownCommandProducesNoOutput) {
  Uci::UciEngine engine(input, output);

  engine.dispatch("this_is_not_a_uci_command");

  EXPECT_TRUE(output.str().empty());
}

TEST_F(UciEngineTest, PositionStartposResetsBoard) {
  Uci::UciEngine engine(input, output);

  engine.dispatch("position startpos moves e2e4");
  engine.dispatch("position startpos");

  const Board &board = engine.get_board();
  auto start_pawn = board.get_piece(E2);
  auto moved = board.get_piece(E4);

  ASSERT_TRUE(start_pawn.has_value());
  EXPECT_TRUE(start_pawn->is_white());
  EXPECT_TRUE(start_pawn->is_pawn());
  EXPECT_FALSE(moved.has_value());
}

TEST_F(UciEngineTest, InvalidMoveStopsFurtherProcessing) {
  Uci::UciEngine engine(input, output);

  engine.dispatch("position startpos moves e2e4 notamove e7e5");

  const Board &board = engine.get_board();
  auto e4 = board.get_piece(E4);
  auto e2 = board.get_piece(E2);
  auto e5 = board.get_piece(E5);
  auto e7 = board.get_piece(E7);

  ASSERT_TRUE(e4.has_value());
  EXPECT_TRUE(e4->is_white());
  EXPECT_TRUE(e4->is_pawn());
  EXPECT_FALSE(e2.has_value());
  EXPECT_FALSE(e5.has_value());
  ASSERT_TRUE(e7.has_value());
  EXPECT_TRUE(e7->is_black());
  EXPECT_TRUE(e7->is_pawn());
}

TEST_F(UciEngineTest, GoInfiniteThenStopProducesBestmove) {
  {
    Uci::UciEngine engine(input, output);
    engine.dispatch("position startpos");
    engine.dispatch("go infinite");
    engine.dispatch("stop");
  }

  const std::string res = output.str();
  EXPECT_NE(res.find("bestmove "), std::string::npos);
}
