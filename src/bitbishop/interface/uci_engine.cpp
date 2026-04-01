#include <bitbishop/interface/uci_engine.hpp>
#include <cassert>

std::vector<std::string> Uci::split(std::string_view str) {
  std::vector<std::string> tokens;
  std::istringstream token_stream{std::string(str)};
  std::string token;
  while (token_stream >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

Uci::UciEngine::UciEngine(std::istream &input, std::ostream &output)
    : is_running(true),
      board(Board::StartingPosition()),
      position(Position(board)),
      in_stream(input),
      out_stream(output),
      search_worker_ptr(nullptr) {}

void Uci::UciEngine::loop() {
  std::string line;
  while (is_running && std::getline(in_stream, line)) {
    dispatch(line);
  }
};

void Uci::UciEngine::dispatch(std::string_view line) {
  if (line == "uci") {
    handle_uci();
  } else if (line == "isready") {
    out_stream << "readyok\n" << std::flush;
  } else if (line == "ucinewgame") {
    handle_new_game();
  } else if (line.starts_with("position")) {
    handle_position(line);
  } else if (line.starts_with("go")) {
    handle_go(line);
  } else if (line == "stop") {
    handle_stop();
  } else if (line == "quit") {
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

void Uci::UciEngine::handle_position(std::string_view line) {
  using namespace Const;

  std::vector<std::string> tokens = split(line);
  if (tokens.size() < 2) {
    return;
  }

  std::size_t offset = 1;              // skip "position"
  if (tokens[offset] == "startpos") {  // "position startpos ..."
    board = Board::StartingPosition();
    ++offset;
  } else if (tokens[offset] == "fen") {  // "position fen ..."
    ++offset;
    if (tokens.size() < offset + FEN_NOTATION_COMPONENT_COUNT) {
      return;
    }
    std::string fen;
    fen.reserve(FEN_NOTATION_MAX_CHAR_COUNT);
    fen += tokens[offset];
    for (int i = 1; i < FEN_NOTATION_COMPONENT_COUNT; ++i) {
      fen += " ";
      fen += tokens[offset + i];
    }
    offset += FEN_NOTATION_COMPONENT_COUNT;
    board = Board(fen);
  } else {
    return;
  }

  // "position ... moves e2e4 f2f4"
  if (offset < tokens.size() && tokens[offset] == "moves") {
    ++offset;
    while (offset < tokens.size()) {
      try {
        position.apply_move(Move::from_uci(tokens[offset]));
      } catch (const std::exception &) {
        break;
      }
      ++offset;
    }
  }
}

void Uci::UciEngine::handle_go(std::string_view line) {
  release_search_controller();

  std::vector<std::string> tokens = split(line);
  SearchLimits limits;

  for (std::size_t i = 1; i < tokens.size(); ++i) {
    const auto &tok = tokens[i];

    auto read = [&](std::optional<int> &target) {
      if (i + 1 < tokens.size()) {
        target = std::stoi(tokens[++i]);
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

void Uci::UciEngine::handle_stop() { release_search_controller(); }

void Uci::UciEngine::handle_quit() {
  release_search_controller();
  is_running = false;
}

void Uci::UciEngine::release_search_controller() {
  if (search_worker_ptr) {
    search_worker_ptr->stop();
    search_worker_ptr.reset();
  }
  assert(search_worker_ptr == nullptr);
}