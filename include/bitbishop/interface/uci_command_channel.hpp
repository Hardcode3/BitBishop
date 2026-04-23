#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <istream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace Uci {

/**
 * @brief Thread-safe command line channel for UCI input.
 *
 * This class owns a reader thread that continuously reads raw lines from an input stream.
 * Consuming code can wait and pop lines without dealing with synchronization primitives.
 */
class UciCommandChannel {
  struct State {
    std::mutex lines_mutex;
    std::condition_variable lines_cv;
    std::deque<std::string> pending_lines;
    std::atomic<bool> eof_reached{false};
    std::atomic<bool> stop_requested{false};
  };

  std::istream& input_stream;
  std::thread reader_thread;
  std::shared_ptr<State> state;

  /**
   * @brief Reader loop running in a dedicated thread.
   */
  static void reader_loop(std::istream& input_stream, std::shared_ptr<State> state);

 public:
  explicit UciCommandChannel(std::istream& input_stream);
  ~UciCommandChannel();

  UciCommandChannel(const UciCommandChannel&) = delete;
  UciCommandChannel& operator=(const UciCommandChannel&) = delete;

  /**
   * @brief Starts the reader thread.
   */
  void start();

  /**
   * @brief Stops reader resources.
   *
   * If input is already at EOF, this joins the thread.
   * Otherwise, thread is detached because std::getline cannot be forcibly interrupted.
   */
  void stop();

  /**
   * @brief Waits for one line and pops it if available.
   *
   * @param line Destination string receiving one raw input line
   * @param timeout Max waiting duration
   * @return true if a line was popped, false otherwise
   */
  bool wait_and_pop_line(std::string& line, std::chrono::milliseconds timeout);

  /**
   * @brief Returns whether input EOF was reached by the reader thread.
   */
  [[nodiscard]] bool eof() const;
};

}  // namespace Uci
