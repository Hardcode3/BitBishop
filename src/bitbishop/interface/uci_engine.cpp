#include <bitbishop/interface/uci_engine.hpp>
#include <cassert>

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
    : is_running(true),
      board(Board::StartingPosition()),
      position(Position(this->board)),
      in_stream(input),
      out_stream(output),
      search_worker_ptr(nullptr) {}

void Uci::UciEngine::loop() {
  std::string input_str;
  std::vector<std::string> line;
  while (is_running && std::getline(in_stream, input_str)) {
    line = split(input_str);
    dispatch(line);
  }
};

void Uci::UciEngine::dispatch(std::vector<std::string> &line) {
  if (line.empty()) {
    return;
  }

  if (line.front() == "uci") {
    handle_uci();
  } else if (line.front() == "isready") {
    out_stream << "readyok\n" << std::flush;
  } else if (line.front() == "ucinewgame") {
    handle_new_game();
  } else if (line.front() == "position") {
    handle_position(line);
  } else if (line.front() == "go") {
    handle_go(line);
  } else if (line.front() == "stop") {
    handle_stop();
  } else if (line.front() == "quit") {
    handle_quit();
  }
  // unknown lines are discarded silently following uci rules
};

void Uci::UciEngine::handle_uci() {
  out_stream << "id name BitBishop\n"
             << "id author Hardcode (Baptiste Penot)\n"
             << "uciok\n"
             << std::flush;
}

void Uci::UciEngine::handle_new_game() {
  board = Board::StartingPosition();
  position.reset();
}

void Uci::UciEngine::handle_position(std::vector<std::string> &line) {
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

void Uci::UciEngine::handle_go(std::vector<std::string> &line) {
  reset_search_worker();

  SearchLimits limits;

  for (std::size_t i = 1; i < line.size(); ++i) {
    const auto &tok = line[i];

    auto read = [&](std::optional<int> &target) {
      if (i + 1 < line.size()) {
        target = std::stoi(line[++i]);
      }
    };

    if (tok == "depth") {
      read(limits.depth);
    } else if (tok == "movetime") {
      read(limits.movetime);
    } else if (tok == "wtime") {
      read(limits.wtime);
    } else if (tok == "btime") {
      read(limits.btime);
    } else if (tok == "winc") {
      read(limits.winc);
    } else if (tok == "binc") {
      read(limits.binc);
    } else if (tok == "infinite") {
      limits.infinite = true;
    }
  }

  search_worker_ptr = std::make_unique<SearchWorker>(board, limits, out_stream);
  assert(search_worker_ptr != nullptr);
  search_worker_ptr->start();
}

void Uci::UciEngine::handle_stop() { reset_search_worker(); }

void Uci::UciEngine::handle_quit() {
  reset_search_worker();
  is_running = false;
}

void Uci::UciEngine::reset_search_worker() {
  if (search_worker_ptr) {
    search_worker_ptr->stop();
    search_worker_ptr.reset();
  }
  assert(search_worker_ptr == nullptr);
}
