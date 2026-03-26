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
  std::jthread worker;          // Worker thread for search operations
  Position *pos = nullptr;      // Pointer to the current game position
  std::ostream *out = nullptr;  // Output stream for UCI communication
  SearchLimits limits;          // Current search parameters

  /**
   * @brief Executes the search algorithm in a background thread.
   *
   * @param stop_token Token to request cancellation of the search
   */
  void run(std::stop_token stop_token);

 public:
  /**
   * @brief Starts the search process with given parameters.
   *
   * Initializes the search controller with a new worker thread and parameters.
   * If no depth is specified, switches to infinite search mode.
   *
   * @param pos Reference to the current game position
   * @param limits Search parameters to use
   * @param out_stream Output stream for UCI communication
   */
  void start(Position &pos, SearchLimits limits, std::ostream &out_stream);

  /**
   * @brief Requests the worker thread to stop its current operation.
   */
  void stop();
};

}  // namespace Uci
