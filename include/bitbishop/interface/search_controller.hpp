#pragma once

#include <bitbishop/engine/search.hpp>
#include <bitbishop/moves/position.hpp>
#include <optional>
#include <ostream>
#include <stop_token>
#include <thread>

namespace Uci {

/**
 * @brief Represents search limits for the UCI interface.
 *
 * This struct holds various parameters that control the search depth and timing.
 */
struct SearchLimits {
  std::optional<int> depth;         ///< Search depth limit (in ply)
  std::optional<int> movetime;      ///< Move time limit (in milliseconds)
  std::optional<int> wtime, btime;  ///< White/black time limits (in milliseconds)
  std::optional<int> winc, binc;    ///< White/black increment limits (in milliseconds)
  bool infinite = false;            ///< Flag for infinite search mode
};

/**
 * @brief Manages the search process for UCI commands.
 *
 * This class handles the execution of search operations based on UCI parameters.
 * It uses a background thread to perform the search and communicates results through a callback.
 */
class SearchController {
  std::thread worker;                  ///< Worker thread for search operations
  std::atomic<bool> stop_flag{false};  ///< Flag used to forward the stop order to the worker(s)
  Board board;                         ///< Current chess board
  Position position;                   ///< Game position associated to the current chess board
  SearchLimits limits;                 ///< Current search parameters
  std::ostream* out;                   ///< Output stream for UCI communication

  /**
   * @brief Executes the search algorithm in a background thread.
   */
  void run();

 public:
  SearchController(Board board, SearchLimits limits, std::ostream& ostream = std::cout);
  ~SearchController();

  /**
   * @brief Starts the search process with given parameters.
   *
   * Initializes the search controller with a new worker thread and parameters.
   * If no depth is specified, switches to infinite search mode.
   */
  void start();

  /**
   * @brief Requests the worker thread to stop naturaly the best move search.
   *
   * Thread finishes when a best move has been found.
   */
  void wait();

  /**
   * @brief Requests the worker thread to interrupt (stop early) the best move search.
   *
   * Thread interruption finishes early the best move search and may not be able to return a best move.
   * An intermediate state may be returned instead.
   */
  void stop();
};

}  // namespace Uci
