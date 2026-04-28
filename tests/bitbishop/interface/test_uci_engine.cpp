#include <BitBishop.h>
#include <gtest/gtest.h>

#include <bitbishop/helpers/async.hpp>
#include <bitbishop/helpers/blocking_stream.hpp>
#include <bitbishop/interface/uci_engine.hpp>

using namespace Squares;

/**
 * @brief GoogleTest fixture for exercising UciEngine in a realistic threaded setup.
 *
 * This fixture reproduces the behavior of a real UCI session by decoupling
 * input and output across two threads:
 *
 * - The engine runs in its own thread and executes UciEngine::loop().
 * - The test thread acts as the GUI/client, sending commands and observing output.
 *
 * ### I/O Model
 *
 * Input (stdin simulation):
 * - Implemented via BlockingIStream (std::istream-compatible).
 * - Pull-based and blocking: the engine consumes input using std::getline().
 * - The engine thread blocks while waiting for data, mimicking std::cin behavior.
 * - The test thread produces input via write(), which appends to an internal buffer
 *   and notifies the waiting engine thread.
 *
 * Output (stdout simulation):
 * - Captured using std::stringstream.
 * - Push-based: the engine writes output immediately.
 * - The test thread passively inspects output (e.g., via polling or wait_for).
 * - No synchronization is required for output access in this setup.
 *
 * ### Thread Interaction
 *
 *                 ┌────────────────────┐
 *                 │ BlockingIStream    │
 *                 │ (std::istream)     │
 *                 └─────────┬──────────┘
 *                           │
 *                           ▼
 *                 ┌────────────────────┐
 *                 │ BlockingStreamBuf  │
 *                 │ buffer_ = ""       │
 *                 └─────────┬──────────┘
 *                           │
 *         ┌─────────────────┴─────────────────┐
 *         ▼                                   ▼
 * [Test thread]                        [Engine thread]
 *
 * write("uci\n")                          getline()
 *  pushes data eagerly            pulls data lazily (blocking)
 *      │                                     │
 *      ▼                                     ▼
 * buffer_ += "uci\n"                     underflow()
 * notify_one()                               │
 *                                            ▼
 *                                      reads 'u','c','i','\n'
 *                                            │
 *                                            ▼
 *                                      returns "uci"
 *
 * ### Lifecycle
 *
 * - SetUp():
 *     - Instantiates the engine with simulated input/output streams.
 *     - Launches the engine loop in a dedicated thread.
 *
 * - TearDown():
 *     - Sends the "quit" command to terminate the loop.
 *     - Closes the input stream to unblock any pending reads.
 *     - Joins the engine thread to ensure clean shutdown.
 *
 * This fixture is intended for integration-style tests where realistic
 * concurrency and blocking behavior are required.
 */
class UciEngineTest : public ::testing::Test {
 protected:
  BlockingIStream input;                   ///< Simulated stdin
  std::stringstream output;                ///< Captured stdout
  std::unique_ptr<Uci::UciEngine> engine;  ///< Engine under test
  std::thread engine_thread;               ///< Engine execution thread

  /**
   * @brief Start engine loop in a separate thread.
   */
  void SetUp() override {
    engine = std::make_unique<Uci::UciEngine>(input, output);
    engine_thread = std::thread([this] { engine->loop(); });
  }

  /**
   * @brief Gracefully stop the engine and join thread.
   */
  void TearDown() override {
    if (input) {
      input.write("quit\n");
      input.close();
    }
    if (engine_thread.joinable()) {
      engine_thread.join();
    }
  }
};

TEST_F(UciEngineTest, CommandWithOnlySpacesDoesNothing) {
  input.write("   \n");

  SUCCEED();
}

TEST_F(UciEngineTest, CommandWithExtraWhitespaceWorks) {
  input.write("   uci       \n");

  assert_output_contains(output, "uciok");
}

TEST_F(UciEngineTest, CommandWithTabsAndSpacesWorks) {
  input.write("  \t uci   \t  \n");

  assert_output_contains(output, "uciok");
}

TEST_F(UciEngineTest, CommandWithReturnsAndSpacesWorks) {
  input.write("  \r uci   \r  \n");

  assert_output_contains(output, "uciok");
}

TEST_F(UciEngineTest, UciCommandOutputsIdAndOk) {
  input.write("uci\n");

  assert_output_contains(output, "uciok");

  const std::string res = output.str();

  assert_output_contains(output, "id name BitBishop");
  assert_output_contains(output, "id author Hardcode");
}

TEST_F(UciEngineTest, MultipleUciCommandsDoNotBreakEngine) {
  input.write("uci\nuci\n");

  assert_output_contains(output, "uciok");
}

TEST_F(UciEngineTest, IsReadyCommandOutputsReadyOk) {
  input.write("uci\nisready\n");

  assert_output_contains(output, "readyok");
}

TEST_F(UciEngineTest, IsReadyBeforeUciStillWorks) {
  input.write("isready\n");

  assert_output_contains(output, "readyok");
}

TEST_F(UciEngineTest, PositionCommandWithOneArgDoesNothing) {
  input.write("position\n");

  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  ASSERT_EQ(engine->get_board(), Board::StartingPosition());
}

TEST_F(UciEngineTest, PositionCommandWithMissingFenArgDoesNothing) {
  // Invalid fen: missing color to play (FEN has 5 components instead of 6)
  input.write("position fen rnkqnbbr/pppppppp/8/8/8/8/PPPPPPPP/RNKQNBBR - - 0 1\n");

  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  ASSERT_EQ(engine->get_board(), Board::StartingPosition());
}

TEST_F(UciEngineTest, PositionCommandWithInvalidSecondaryKeyworkDoesNothing) {
  // Invalid command: fren instead of fen
  input.write("position fren ...\n");

  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  ASSERT_EQ(engine->get_board(), Board::StartingPosition());
}

TEST_F(UciEngineTest, PositionStartposMovesAppliesMoves) {
  input.write("position startpos moves e2e4\n");

  ASSERT_TRUE(wait_for([&] {
    const Board& b = engine->get_board();
    return b.get_piece(E4).has_value();
  }));

  const Board& board = engine->get_board();
  auto moved = board.get_piece(E4);
  auto origin = board.get_piece(E2);

  ASSERT_TRUE(moved.has_value());
  EXPECT_TRUE(moved->is_white());
  EXPECT_TRUE(moved->is_pawn());
  EXPECT_FALSE(origin.has_value());
}

TEST_F(UciEngineTest, PositionStartposMovesWithSpacesAppliesMoves) {
  input.write("position    startpos             moves e2e4\n");

  ASSERT_TRUE(wait_for([&] {
    const Board& b = engine->get_board();
    return b.get_piece(E4).has_value();
  }));

  const Board& board = engine->get_board();
  auto moved = board.get_piece(E4);
  auto origin = board.get_piece(E2);

  ASSERT_TRUE(moved.has_value());
  EXPECT_TRUE(moved->is_white());
  EXPECT_TRUE(moved->is_pawn());
  EXPECT_FALSE(origin.has_value());
}

TEST_F(UciEngineTest, PositionStartposWithEmptyMovesDoesNothing) {
  input.write("position startpos moves\n");

  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  ASSERT_EQ(engine->get_board(), Board::StartingPosition());
}

TEST_F(UciEngineTest, CommandWithoutNewlineDoesNotApply) {
  input.write("position startpos moves e2e4");  // no '\n'

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  ASSERT_EQ(engine->get_board(), Board::StartingPosition());
}

TEST_F(UciEngineTest, PositionFenMovesAppliesMoves) {
  input.write("position fen rnqkbbnr/pppppppp/8/8/3P1Q2/8/PPP1PPPP/RN1KBBNR b - - 0 1 moves e1b4\n");

  ASSERT_TRUE(wait_for([&] {
    const Board& b = engine->get_board();
    return b.get_piece(B4).has_value();
  }));

  const Board& board = engine->get_board();
  auto moved = board.get_piece(B4);
  auto origin = board.get_piece(E1);

  ASSERT_TRUE(moved.has_value());
  EXPECT_TRUE(moved->is_white());
  EXPECT_TRUE(moved->is_bishop());
  EXPECT_FALSE(origin.has_value());
}

TEST_F(UciEngineTest, UciNewGameResetsBoard) {
  input.write("position startpos moves e2e4\n");

  ASSERT_TRUE(wait_for([&] {
    const Board& b = engine->get_board();
    return b.get_piece(E4).has_value();
  }));

  input.write("ucinewgame\n");

  ASSERT_TRUE(wait_for([&] {
    const Board& b = engine->get_board();
    return b.get_piece(E2).has_value();
  }));

  const Board& board = engine->get_board();
  auto start_pawn = board.get_piece(E2);
  auto moved = board.get_piece(E4);

  ASSERT_TRUE(start_pawn.has_value());
  EXPECT_TRUE(start_pawn->is_white());
  EXPECT_TRUE(start_pawn->is_pawn());
  EXPECT_FALSE(moved.has_value());
}

TEST_F(UciEngineTest, GoWithoutPositionProducesBestMove) {
  input.write("go depth 2\n");

  assert_output_contains(output, "bestmove ");
}

TEST_F(UciEngineTest, GoMovetimeProducesBestMove) {
  input.write("go movetime 50\n");

  assert_output_contains(output, "bestmove ");
}

TEST_F(UciEngineTest, UnknownCommandProducesNoOutput) {
  // Clear the output containing the startup message.
  assert_output_contains(output, " by ");
  output.str(std::string());
  output.clear();

  input.write("this_is_not_a_uci_command\n");

  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  EXPECT_TRUE(output.str().empty());
}

TEST_F(UciEngineTest, PositionStartposResetsBoard) {
  input.write("position fen 8/8/8/8/8/4P3/8/4K3 b - - 0 1 moves e3e4\n");

  ASSERT_TRUE(wait_for([&] {
    const Board& b = engine->get_board();
    return b.get_piece(E4).has_value();
  }));

  input.write("position startpos\n");

  ASSERT_TRUE(wait_for([&] {
    const Board& b = engine->get_board();
    return b.get_piece(E2).has_value();
  }));

  ASSERT_EQ(engine->get_board(), Board::StartingPosition());
}

TEST_F(UciEngineTest, InvalidMoveStopsFurtherProcessing) {
  input.write("position startpos moves e2e4 notamove e7e5\n");

  ASSERT_TRUE(wait_for([&] {
    const Board& b = engine->get_board();
    return b.get_piece(E4).has_value();
  }));

  const Board& board = engine->get_board();
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
  input.write(
      "position startpos\n"
      "go infinite\n"
      "stop\n");

  assert_output_contains(output, "bestmove ");
}

TEST_F(UciEngineTest, GoWithoutDepthIsInfinite) {
  input.write("go\n");

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  ASSERT_TRUE(output.str().find("bestmove ") == std::string::npos);
}

TEST_F(UciEngineTest, GoDepthKeepsEngineResponsive) {
  input.write(
      "go depth 8\n"
      "isready\n"
      "stop\n");

  assert_output_contains(output, "readyok");
  assert_output_contains(output, "bestmove ");
}

TEST_F(UciEngineTest, GoStopGoDoesNotCrash) {
  input.write(
      "go infinite\n"
      "stop\n"
      "go depth 2\n");

  assert_output_contains(output, "bestmove ");
}

TEST_F(UciEngineTest, StopWithoutGoDoesNothing) {
  // Clear the output containing the startup message.
  assert_output_contains(output, " by ");
  output.str(std::string());
  output.clear();

  input.write("stop\n");

  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  EXPECT_TRUE(output.str().empty());
}

TEST_F(UciEngineTest, DoubleStopDoesNotCrash) {
  input.write(
      "go infinite\n"
      "stop\n"
      "stop\n");

  assert_output_contains(output, "bestmove ");
}

TEST_F(UciEngineTest, GoWhileAlreadySearchingStopsAndRestarts) {
  input.write(
      "go infinite\n"
      "go depth 2\n");

  assert_output_contains(output, "bestmove ");
}

TEST_F(UciEngineTest, RapidFireCommandsDoNotBreakEngine) {
  input.write(
      "uci\n"
      "isready\n"
      "position startpos\n"
      "go depth 1\n");

  assert_output_contains(output, "bestmove ");
}

TEST_F(UciEngineTest, EngineStopsOnInputCloseEOF) {
  input.close();
  engine_thread.join();

  SUCCEED();
}

void assert_display_works(const std::stringstream& output) {
  assert_output_contains(output, "+---+---+---+---+---+---+---+---+");
  assert_output_contains(output, "  a   b   c   d   e   f   g   h  ");
  assert_output_contains(output, "FEN notation");
  assert_output_contains(output, "Zobrist hash");
}

TEST_F(UciEngineTest, DisplayWorksWithStartPos) {
  input.write(
      "position startpos\n"
      "d\n");

  assert_display_works(output);
}

TEST_F(UciEngineTest, DisplayWorksWithRandomPosition) {
  input.write(
      "position fen rk2n1qr/ppppp1pp/1bn2b2/2NQp2P/8/2B4B/PPPPPP1P/RK1N3R b - - 0 1\n"
      "d\n");

  assert_display_works(output);
}

TEST_F(UciEngineTest, DisplayWorksWithEmptyBoard) {
  input.write(
      "position fen 8/8/8/8/8/8/8/8 b - - 0 1\n"
      "d\n");

  assert_display_works(output);
}

TEST_F(UciEngineTest, StartupMsgIsDisplayed) {
  assert_output_contains(output, BITBISHOP_PROJECT_NAME);
  assert_output_contains(output, BITBISHOP_VERSION);
  assert_output_contains(output, " by ");
}

TEST_F(UciEngineTest, HelpMessageIsDisplayed) {
  input.write("help\n");

  assert_output_contains(output, " is a chess engine ");
  assert_output_contains(output, " published under ");
  assert_output_contains(output, "For any further information, ");
}

TEST_F(UciEngineTest, BenchProducesBenchReport) {
  input.write("bench depth 2\n");

  assert_output_contains(output, "bench nodes ");
}

TEST_F(UciEngineTest, BenchCanBeStopped) {
  input.write(
      "bench depth 8\n"
      "stop\n");

  assert_output_contains(output, "bench nodes ");
}
