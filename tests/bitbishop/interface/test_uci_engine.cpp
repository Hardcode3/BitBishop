#include <gtest/gtest.h>

#include <bitbishop/interface/uci_engine.hpp>
#include <sstream>

using namespace Squares;

class UciEngineTest : public ::testing::Test {
 protected:
  std::stringstream input;
  std::stringstream output;
  std::unique_ptr<Uci::UciEngine> engine_ptr;

  void SetUp() override { engine_ptr = std::make_unique<Uci::UciEngine>(input, output); }

  void TearDown() override {
    input.str("");
    input.clear();

    output.str("");
    output.clear();

    engine_ptr.reset();
  }
};

TEST_F(UciEngineTest, UciCommandOutputsIdAndOk) {
  input.str("uci\n");
  engine_ptr->loop();

  const std::string res = output.str();
  EXPECT_NE(res.find("id name BitBishop"), std::string::npos);
  EXPECT_NE(res.find("id author Hardcode"), std::string::npos);
  EXPECT_NE(res.find("uciok"), std::string::npos);
}

TEST_F(UciEngineTest, IsReadyCommandOutputsReadyOk) {
  input.str("isready\n");
  engine_ptr->loop();

  EXPECT_EQ(output.str(), "readyok\n");
}

TEST_F(UciEngineTest, PositionStartposMovesAppliesMoves) {
  input.str("position startpos moves e2e4\n");
  engine_ptr->loop();

  const Board &board = engine_ptr->get_board();
  auto moved = board.get_piece(E4);
  auto origin = board.get_piece(E2);

  ASSERT_TRUE(moved.has_value());
  EXPECT_TRUE(moved->is_white());
  EXPECT_TRUE(moved->is_pawn());
  EXPECT_FALSE(origin.has_value());
}

TEST_F(UciEngineTest, PositionFenMovesAppliesMoves) {
  input.str("position fen 8/8/8/8/8/8/4P3/4K3 w - - 0 1 moves e2e4\n");
  engine_ptr->loop();

  const Board &board = engine_ptr->get_board();
  auto moved = board.get_piece(E4);
  auto origin = board.get_piece(E2);

  ASSERT_TRUE(moved.has_value());
  EXPECT_TRUE(moved->is_white());
  EXPECT_TRUE(moved->is_pawn());
  EXPECT_FALSE(origin.has_value());
}

TEST_F(UciEngineTest, UciNewGameResetsBoard) {
  input.str(
      "position startpos moves e2e4\n"
      "ucinewgame\n");
  engine_ptr->loop();

  const Board &board = engine_ptr->get_board();
  auto start_pawn = board.get_piece(E2);
  auto moved = board.get_piece(E4);

  ASSERT_TRUE(start_pawn.has_value());
  EXPECT_TRUE(start_pawn->is_white());
  EXPECT_TRUE(start_pawn->is_pawn());
  EXPECT_FALSE(moved.has_value());
}

// TEST_F(UciEngineTest, GoWithoutPositionUsesStartpos) {
//   input.str("go depth 1\n");
//   engine_ptr->loop();

//   const std::string res = output.str();
//   EXPECT_NE(res.find("bestmove "), std::string::npos);
// }

TEST_F(UciEngineTest, UnknownCommandProducesNoOutput) {
  input.str("this_is_not_a_uci_command\n");
  engine_ptr->loop();

  EXPECT_TRUE(output.str().empty());
}

TEST_F(UciEngineTest, PositionStartposResetsBoard) {
  input.str(
      "position startpos moves e2e4\n"
      "position startpos\n");
  engine_ptr->loop();

  const Board &board = engine_ptr->get_board();
  auto start_pawn = board.get_piece(E2);
  auto moved = board.get_piece(E4);

  ASSERT_TRUE(start_pawn.has_value());
  EXPECT_TRUE(start_pawn->is_white());
  EXPECT_TRUE(start_pawn->is_pawn());
  EXPECT_FALSE(moved.has_value());
}

TEST_F(UciEngineTest, InvalidMoveStopsFurtherProcessing) {
  input.str("position startpos moves e2e4 notamove e7e5\n");
  engine_ptr->loop();

  const Board &board = engine_ptr->get_board();
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

// TEST_F(UciEngineTest, GoInfiniteThenStopProducesBestmove) {
//   input.str(
//       "position startpos\n"
//       "go infinite\n"
//       "stop\n");
//   engine_ptr->loop();

//   const std::string res = output.str();
//   EXPECT_NE(res.find("bestmove "), std::string::npos);
// }
