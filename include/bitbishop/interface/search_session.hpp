#pragma once

#include <bitbishop/interface/search_controller.hpp>
#include <bitbishop/interface/search_reporter.hpp>
#include <memory>
#include <ostream>

namespace Uci {

/**
 * @brief Owns the lifecycle of one active search and its reporter.
 *
 * Worker threads only publish events. This class consumes those events on the
 * control thread and forwards them to the configured reporter.
 */
class SearchSession {
  std::ostream& out_stream;
  std::unique_ptr<SearchWorker> worker;
  std::unique_ptr<SearchReporter> reporter;

  /**
   * @brief Emits pending reports from worker to reporter.
   */
  void emit_reports();

  /**
   * @brief Finalizes and clears resources if search already finished.
   */
  void finalize_if_done();

 public:
  explicit SearchSession(std::ostream& out_stream);
  ~SearchSession();

  SearchSession(const SearchSession&) = delete;
  SearchSession& operator=(const SearchSession&) = delete;

  /**
   * @brief Starts a regular UCI search.
   */
  void start_go(Board board, SearchLimits limits);

  /**
   * @brief Starts a benchmark search.
   */
  void start_bench(Board board, SearchLimits limits);

  /**
   * @brief Requests current search to stop (non-blocking).
   */
  void request_stop();

  /**
   * @brief Pumps reports and finalization (non-blocking).
   */
  void poll();

  /**
   * @brief Stops and joins current search if any (blocking).
   */
  void stop_and_join();

  /**
   * @brief Returns true when no search is active.
   */
  [[nodiscard]] bool is_idle() const { return worker == nullptr; }
};

}  // namespace Uci
