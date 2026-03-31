#include <gtest/gtest.h>

#include <bitbishop/interface/uci_engine.hpp>
#include <sstream>

using namespace Squares;

/**
 * @brief Thread-safe blocking stream buffer.
 *
 * Provides a producer-consumer mechanism for feeding input to a std::istream.
 * Data is pushed externally and consumed lazily via underflow(), blocking
 * until data is available or the stream is closed.
 */
class BlockingStreamBuf : public std::streambuf {
 public:
  /**
   * @brief Append data to the buffer and notify waiting readers.
   * @param data Input data to append.
   */
  void push(const std::string& data) {
    std::lock_guard<std::mutex> lock(m_);
    buffer_ += data;
    cv_.notify_one();
  }

  /**
   * @brief Mark the buffer as closed and wake all waiting readers.
   */
  void close() {
    std::lock_guard<std::mutex> lock(m_);
    closed_ = true;
    cv_.notify_all();
  }

 protected:
  /**
   * @brief Provide next character to the stream (blocking if needed).
   *
   * Waits until data is available or the stream is closed. Returns EOF when
   * no more data is available and the stream is closed.
   */
  int_type underflow() override {
    std::unique_lock<std::mutex> lock(m_);

    cv_.wait(lock, [&] { return pos_ < buffer_.size() || closed_; });

    if (pos_ >= buffer_.size()) {
      return traits_type::eof();
    }

    char* base = &buffer_[pos_];
    setg(base, base, base + 1);

    ++pos_;
    return traits_type::to_int_type(*gptr());
  }

 private:
  std::string buffer_;  ///< Internal storage
  size_t pos_ = 0;      ///< Current read position

  std::mutex m_;                ///< Synchronization mutex
  std::condition_variable cv_;  ///< Notification mechanism
  bool closed_ = false;         ///< Indicates no more data will arrive
};

/**
 * @brief Blocking input stream backed by BlockingStreamBuf.
 *
 * Acts like std::cin for the engine, but allows controlled, thread-safe
 * injection of input from tests.
 */
class BlockingIStream : public std::istream {
 public:
  /**
   * @brief Construct stream using internal blocking buffer.
   */
  BlockingIStream() : std::istream(&buf_) {}

  /**
   * @brief Write data into the stream.
   * @param s Input string (typically UCI command + '\n').
   */
  void write(const std::string& s) { buf_.push(s); }

  /**
   * @brief Close the stream (signals EOF to readers).
   */
  void close() { buf_.close(); }

 private:
  BlockingStreamBuf buf_;  ///< Underlying buffer
};

/**
 * @brief Test fixture for UciEngine using a real threaded loop.
 *
 * Simulates a real UCI environment:
 * - Engine runs in its own thread
 * - Input is fed asynchronously
 * - Output is captured via stringstream
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
    input.write("quit\n");
    input.close();
    engine_thread.join();
  }
};

/**
 * @brief Wait until a condition becomes true or timeout expires.
 *
 * Polls the predicate periodically until it returns true or the timeout
 * is reached. Useful for synchronizing with asynchronous engine behavior.
 *
 * @param pred Condition to evaluate.
 * @param timeout Maximum duration to wait.
 * @param interval Delay between successive checks.
 * @return true if condition became true within timeout, false otherwise.
 */
bool wait_for(std::function<bool()> pred, std::chrono::milliseconds timeout = std::chrono::milliseconds(500),
              std::chrono::milliseconds interval = std::chrono::milliseconds(10)) {
  const auto start = std::chrono::steady_clock::now();

  while (std::chrono::steady_clock::now() - start < timeout) {
    if (pred()) {
      return true;
    }
    std::this_thread::sleep_for(interval);
  }
  return false;
}

TEST_F(UciEngineTest, UciCommandOutputsIdAndOk) {
  input.write("uci\n");

  ASSERT_TRUE(wait_for([&] { return output.str().find("uciok") != std::string::npos; }));

  const std::string res = output.str();

  EXPECT_NE(res.find("id name BitBishop"), std::string::npos);
  EXPECT_NE(res.find("id author Hardcode"), std::string::npos);
}

TEST_F(UciEngineTest, IsReadyCommandOutputsReadyOk) {
  input.write("isready\n");

  ASSERT_TRUE(wait_for([&] { return output.str().find("readyok") != std::string::npos; }));
}

TEST_F(UciEngineTest, PositionCommandWithOneArgDoesNothing) {
  input.write("position");

  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  ASSERT_EQ(engine->get_board(), Board::StartingPosition());
}

TEST_F(UciEngineTest, PositionCommandWithMissingFenArgDoesNothing) {
  // Invalid fen: missing color to play (FEN has 5 components instead of 6)
  input.write("position fen rnkqnbbr/pppppppp/8/8/8/8/PPPPPPPP/RNKQNBBR - - 0 1");

  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  ASSERT_EQ(engine->get_board(), Board::StartingPosition());
}

TEST_F(UciEngineTest, PositionCommandWithInvalidSecondaryKeyworkDoesNothing) {
  // Invalid command: fren instead of fen
  input.write("position fren ...");

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

  ASSERT_TRUE(wait_for([&] { return output.str().find("bestmove ") != std::string::npos; }));
}

TEST_F(UciEngineTest, UnknownCommandProducesNoOutput) {
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

  ASSERT_TRUE(wait_for([&] { return output.str().find("bestmove ") != std::string::npos; }));
}

TEST_F(UciEngineTest, GoWithoutDepthIsInfinite) {
  input.write("go");

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  ASSERT_TRUE(output.str().find("bestmove ") == std::string::npos);
}

TEST_F(UciEngineTest, GoStopGoDoesNotCrash) {
  input.write(
      "go infinite\n"
      "stop\n"
      "go depth 2\n");

  ASSERT_TRUE(wait_for([&] { return output.str().find("bestmove ") != std::string::npos; }));
}
