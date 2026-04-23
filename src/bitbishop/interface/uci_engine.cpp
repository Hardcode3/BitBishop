#include <BitBishop.h>

#include <bitbishop/interface/uci_engine.hpp>

[[nodiscard]] std::vector<std::string> Uci::split(const std::string &str) {
  std::vector<std::string> tokens;
  std::istringstream token_stream{str};
  std::string token;
  while (token_stream >> token) {  // operator>> skips all whitespace
    tokens.push_back(token);
  }
  return tokens;
}

Uci::UciEngine::UciEngine(std::istream &input, std::ostream &output)
    : board(Board::StartingPosition()),
      position(Position(this->board)),
      command_channel(input),
      search_session(output),
      command_registry(),
      is_running(true),
      out_stream(output) {
  register_handlers();
}

void Uci::UciEngine::loop() {
  constexpr const std::chrono::milliseconds LINE_POLL_INTERVAL_MS(5);

  send_startup_msg();
  command_channel.start();

  while (is_running) {
    search_session.poll();

    std::string raw_line;
    const bool line_was_popped = command_channel.wait_and_pop_line(raw_line, LINE_POLL_INTERVAL_MS);
    if (line_was_popped) {
      std::vector<std::string> line = split(raw_line);
      dispatch(line);
      continue;
    }

    if (command_channel.eof()) {
      search_session.request_stop();
      search_session.poll();
      if (search_session.is_idle()) {
        is_running = false;
      }
    }
  }

  search_session.stop_and_join();
  command_channel.stop();
}

void Uci::UciEngine::dispatch(const std::vector<std::string>& line) {
  std::ignore = command_registry.dispatch(line);
  // unknown lines are discarded silently following uci rules
};

void Uci::UciEngine::register_handlers() {
  command_registry.register_handler("uci", [this](const std::vector<std::string>& line) {
    (void)line;
    handle_uci();
  });
  command_registry.register_handler("isready", [this](const std::vector<std::string>& line) {
    (void)line;
    out_stream << "readyok\n" << std::flush;
  });
  command_registry.register_handler("ucinewgame", [this](const std::vector<std::string>& line) {
    (void)line;
    handle_new_game();
  });
  command_registry.register_handler("position",
                                    [this](const std::vector<std::string>& line) { handle_position(line); });
  command_registry.register_handler("go", [this](const std::vector<std::string>& line) { handle_go(line); });
  command_registry.register_handler("stop", [this](const std::vector<std::string>& line) {
    (void)line;
    handle_stop();
  });
  command_registry.register_handler("quit", [this](const std::vector<std::string>& line) {
    (void)line;
    handle_quit();
  });
  command_registry.register_handler("d", [this](const std::vector<std::string>& line) {
    (void)line;
    handle_display();
  });
  command_registry.register_handler("help", [this](const std::vector<std::string>& line) {
    (void)line;
    handle_help();
  });
  command_registry.register_handler("bench", [this](const std::vector<std::string>& line) { handle_bench(line); });
}

void Uci::UciEngine::handle_uci() {
  out_stream << "id name " << BITBISHOP_PROJECT_NAME << "\n"
             << "id author Hardcode (Baptiste Penot)\n"
             << "uciok\n"
             << std::flush;
}

void Uci::UciEngine::handle_new_game() {
  board = Board::StartingPosition();
  position.reset();
}

void Uci::UciEngine::handle_position(const std::vector<std::string>& line) {
  using namespace Const;

  if (line.size() < 2) {
    return;
  }

  std::size_t offset = 1;              // skip "position"
  if (line[offset] == "startpos") {    // "position startpos ..."
    board = Board::StartingPosition();
    ++offset;
  } else if (line[offset] == "fen") {  // "position fen ..."
    ++offset;
    if (line.size() < offset + FEN_NOTATION_COMPONENT_COUNT) {
      return;
    }
    std::string fen;
    fen.reserve(FEN_NOTATION_MAX_CHAR_COUNT);
    fen += line[offset];
    for (int i = 1; i < FEN_NOTATION_COMPONENT_COUNT; ++i) {
      fen += " ";
      fen += line[offset + i];
    }
    offset += FEN_NOTATION_COMPONENT_COUNT;
    board = Board(fen);
  } else {
    return;
  }
  position.reset();

  // "position ... moves e2e4 f2f4"
  if (offset < line.size() && line[offset] == "moves") {
    ++offset;
    while (offset < line.size()) {
      try {
        position.apply_move(Move::from_uci(line[offset]));
      } catch (const std::exception &) {
        break;
      }
      ++offset;
    }
  }
}

void Uci::UciEngine::handle_go(const std::vector<std::string>& line) {
  SearchLimits limits = SearchLimits::from_uci_cmd(line);
  search_session.start_go(board, limits);
}

void Uci::UciEngine::handle_stop() { search_session.request_stop(); }

void Uci::UciEngine::handle_quit() {
  search_session.request_stop();
  is_running = false;
}

void Uci::UciEngine::handle_display() {
  out_stream << "\n";
  out_stream << board << "\n";
  out_stream << "FEN notation: " << board.get_fen() << "\n";
  out_stream << "Zobrist hash: " << board.get_zobrist_hash() << "\n";
  out_stream << std::flush;
}

void Uci::UciEngine::handle_help() {
  // NOLINTBEGIN(modernize-avoid-c-arrays)
  static constexpr char HELP_MSG[] = BITBISHOP_PROJECT_NAME
      " "
      R"(is a chess engine written in C++23, built as a personal learning project around bitboards, move generation, search, and engine architecture.
Its source code is published under the MIT license (see https://github.com/Hardcode3/BitBishop/blob/main/LICENSE).
This chess engine can be used with a graphical user interface (GUI) and implements the Universal Chess Interface (UCI) protocol to communicate with a GUI or any kind of API.
For any further information, visit its GitHub repository: https://github.com/Hardcode3/BitBishop.
)";
  // NOLINTEND(modernize-avoid-c-arrays)
  out_stream << HELP_MSG;
}

void Uci::UciEngine::send_startup_msg() {
  out_stream << BITBISHOP_PROJECT_NAME << " " << BITBISHOP_VERSION << " " << "by Hardcode3 (Baptiste Penot).\n";
}

void Uci::UciEngine::handle_bench(const std::vector<std::string>& line) {
  SearchLimits limits = SearchLimits::from_uci_cmd(line);
  search_session.start_bench(board, limits);
}
