#pragma once

#include <bitbishop/interface/search_session.hpp>
#include <bitbishop/interface/uci_command_channel.hpp>
#include <bitbishop/interface/uci_command_registry.hpp>
#include <bitbishop/moves/position.hpp>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace Uci {

/**
 * @brief Splits a string into a vector of tokens.
 *
 * @param str The input string view to split
 * @return std::vector<std::string> Vector of tokens extracted from the input string
 *
 * @note Discards all kinds of whitespaces (tabs, return, multi-spaces) from a string.
 */
[[nodiscard]] std::vector<std::string> split(const std::string &str);

/**
 * @brief Implements the UCI protocol interface for a chess engine.
 *
 * This class handles communication with the UCI protocol, including command dispatching,
 * game state management, and search control.
 */
class UciEngine {
  Board board;                  ///< Current chess board
  Position position;            ///< Game position associated to the current chess board
  UciCommandChannel command_channel;  ///< Command listener (input thread + queue)
  SearchSession search_session;       ///< Search lifecycle owner (worker + reporter)
  UciCommandRegistry command_registry;  ///< UCI command -> handler registry
  bool is_running;

  std::ostream& out_stream;  ///< Output stream for UCI responses

 public:
  UciEngine() = delete;

  /**
   * @brief Constructs a UCI engine with specified input/output streams.
   *
   * Initializes the engine with default streams (std::cin and std::cout) if none are provided.
   * Sets up the initial board position and search controller.
   *
   * @param input Reference to input stream (default: std::cin)
   * @param output Reference to output stream (default: std::cout)
   */
  UciEngine(std::istream &input = std::cin, std::ostream &output = std::cout);

  /**
   * @brief Main loop to process incoming UCI commands.
   *
   * Continuously reads lines from the input stream and dispatches them to appropriate handlers.
   */
  void loop();

  /**
   * @brief Gets the current board state.
   *
   * Provides read-only access to the current board representation.
   *
   * @return const Board& Reference to the current board state
   */
  [[nodiscard]] const Board &get_board() const { return board; }

 private:
  /**
   * @brief Dispatches UCI commands to their respective handlers.
   *
   * Processes each line of input and routes it to the appropriate command handler.
   * Supported commands:
   * - "uci"
   * - "isready"
   * - "ucinewgame"
   * - "position"
   * - "go"
   * - "stop"
   * - "quit"
   *
   * @param line The input command tokens to process
   */
  void dispatch(const std::vector<std::string> &line);

  /**
   * @brief Registers all built-in UCI command handlers.
   */
  void register_handlers();

  /**
   * @brief Handles the "uci" command.
   *
   * Responds to the UCI command by identifying the engine and signaling readiness.
   */
  void handle_uci();

  /**
   * @brief Handles the "ucinewgame" command.
   *
   * Resets the game state and search controller to start a new game.
   */
  void handle_new_game();

  /**
   * @brief Parses and handles "position" commands.
   *
   * Processes position setup commands including startpos, fen, and move sequences.
   *
   * @param line The input command tokens containing the position information
   */
  void handle_position(const std::vector<std::string> &line);

  /**
   * @brief Parses and handles "go" commands.
   *
   * Processes search commands (depth, movetime, etc.) and starts the search controller.
   *
   * @param line The input command tokens containing the search parameters
   */
  void handle_go(const std::vector<std::string> &line);

  /**
   * @brief Handles the "stop" command.
   *
   * Terminates early the best move search. Best move may be inconsistent.
   */
  void handle_stop();

  /**
   * @brief Handles the "quit" command.
   *
   * Terminates the best move search as soon as possible and exits the program by breaking the UCI loop.
   */
  void handle_quit();

  /**
   * @brief Displays the current board state as well as usefull information.
   *
   * Other information includes:
   *
   * - FEN
   * - Zobrist hash key
   */
  void handle_display();

  /**
   * @brief Displays help to the user.
   */
  void handle_help();

  /**
   * @brief Displays the startup message with engine's version and contributor-s.
   *
   * Should be used before the UCI loop starts.
   */
  void send_startup_msg();

  /**
   * @brief Runs a benchmark.
   *
   * @param line The input command tokens containing the benchmark information
   */
  void handle_bench(const std::vector<std::string> &line);
};

}  // namespace Uci
