#include <bitbishop/interface/uci_engine.hpp>

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
      controller(SearchController(board, SearchLimits{}, out_stream)) {}

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
    controller.stop();
  } else if (line == "quit") {
    controller.stop();
    is_running = false;
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
  controller.stop();
  board = Board::StartingPosition();
  position.reset();
}

void Uci::UciEngine::handle_position(std::string_view line) {
  using namespace Const;

  std::vector<std::string> tokens = split(line);
  if (tokens.size() < 2) {
    return;
  }

  std::size_t offset = 1;  // skip "position"
  if (tokens[offset] == "startpos") {  // "position startpos ..."
    const Board new_position = Board::StartingPosition();
    ++offset;
  } else if (tokens[offset] == "fen") {  // "position fen ..."
    ++offset;
    if (tokens.size() < offset + FEN_NOTATION_COMPONENT_COUNT) {
      return;
    }
    std::string fen;
    fen.reserve(FEN_NOTATION_MAX_CHAR_COUNT);
    fen += tokens[offset];
    for (int i = 1; i < 6; ++i) {
      fen += " ";
      fen += tokens[offset + i];
    }
    offset += FEN_NOTATION_COMPONENT_COUNT;
    const Board new_position(fen);
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
  controller.stop();

  std::vector<std::string> tokens = split(line);
  SearchLimits limits;

  for (std::size_t offset = 1; offset < tokens.size(); ++offset) {
    if (tokens[offset] == "depth") {
      if (offset + 1 < tokens.size()) {
        limits.depth = std::stoi(tokens[++offset]);
      }
    } else if (tokens[offset] == "movetime") {
      if (offset + 1 < tokens.size()) {
        limits.movetime = std::stoi(tokens[++offset]);
      }
    } else if (tokens[offset] == "wtime") {
      if (offset + 1 < tokens.size()) {
        limits.wtime = std::stoi(tokens[++offset]);
      }
    } else if (tokens[offset] == "btime") {
      if (offset + 1 < tokens.size()) {
        limits.btime = std::stoi(tokens[++offset]);
      }
    } else if (tokens[offset] == "winc") {
      if (offset + 1 < tokens.size()) {
        limits.winc = std::stoi(tokens[++offset]);
      }
    } else if (tokens[offset] == "binc") {
      if (offset + 1 < tokens.size()) {
        limits.binc = std::stoi(tokens[++offset]);
      }
    } else if (tokens[offset] == "infinite") {
      limits.infinite = true;
    }
  }

  controller.start();
  controller.wait();
}
