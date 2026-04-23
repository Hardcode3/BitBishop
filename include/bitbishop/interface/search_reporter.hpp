#pragma once

#include <bitbishop/engine/search.hpp>
#include <chrono>
#include <functional>

/**
 * @brief Interface for reporting progress and results of a search.
 *
 * SearchReporter provides a callback-based mechanism for observing the
 * progress of a search as well as its final result. Implementations can
 * forward this information to different outputs (e.g., UCI protocol,
 * benchmarking logs, GUI, etc.).
 */
struct SearchReporter {
  virtual ~SearchReporter() = default;

  /**
   * @brief Called after each completed search iteration.
   *
   * @param best  Current best move found so far.
   * @param depth Depth reached in the current iteration.
   * @param stats Accumulated search statistics.
   */
  virtual void on_iteration(const Search::BestMove& best, int depth, const Search::SearchStats& stats) {}

  /**
   * @brief Called once when the search finishes.
   *
   * Must be implemented by derived classes to handle final reporting.
   *
   * @param best  Final best move found by the search.
   * @param stats Final search statistics.
   */
  virtual void on_finish(const Search::BestMove& best, const Search::SearchStats& stats) = 0;
};

/**
 * @brief Reporter that outputs results in UCI (Universal Chess Interface) format.
 *
 * UciReporter writes the final best move to the provided output stream
 * following the UCI protocol specification. Intended for communication
 * with chess GUIs or other UCI-compatible tools.
 */
struct UciReporter : SearchReporter {
 private:
  /** Output stream used for writing UCI messages. */
  std::ostream& out_stream;

 public:
  /**
   * @brief Constructs a UciReporter.
   *
   * @param out Output stream where UCI messages will be written.
   */
  UciReporter(std::ostream& out);

  /**
   * @brief Outputs the final best move in UCI format.
   *
   * Prints a line of the form:
   *   "bestmove <move>"
   * If no move is available, "0000" is used.
   *
   * @param best  Final best move found by the search.
   * @param stats Final search statistics (unused).
   */
  void on_finish(const Search::BestMove& best, const Search::SearchStats& stats) override;
};

/**
 * @brief Reporter that outputs benchmarking information.
 *
 * BenchReporter measures elapsed time from construction to completion
 * and reports total nodes searched along with nodes-per-second (NPS).
 */
struct BenchReporter : SearchReporter {
  using Clock = std::chrono::steady_clock;

 public:
  /** Injectable time source. Mainly for testing.
   * Must be declared *before* start.
   */
  std::function<Clock::time_point()> now;

 private:
  /** Output stream used for writing benchmark results. */
  std::ostream& out_stream;

  /** Start time of the benchmark measurement. */
  Clock::time_point start;

 public:
  /**
   * @brief Constructs a BenchReporter and records the start time.
   *
   * @param out Output stream where benchmark results will be written.
   */
  BenchReporter(std::ostream& out);
  BenchReporter(std::ostream& out, std::function<Clock::time_point()> now_fn);

  /**
   * @brief Outputs benchmark statistics upon search completion.
   *
   * Computes total nodes searched (negamax + quiescence), elapsed time,
   * and nodes per second (NPS), then prints a summary line (see implementation for details).
   *
   * @param best  Final best move found by the search (unused).
   * @param stats Final search statistics.
   */
  void on_finish(const Search::BestMove& best, const Search::SearchStats& stats) override;
};
