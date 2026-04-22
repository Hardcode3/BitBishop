#include <BitBishop.h>

#include <bitbishop/interface/uci_engine.hpp>
#include <cassert>

struct Uci::UciEngine::InputState {
  std::mutex commands_mutex;
  std::condition_variable commands_cv;
  std::deque<std::vector<std::string>> pending_commands;
  std::atomic<bool> input_eof{false};
  std::atomic<bool> stop_input_reader{false};
};

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
      search_worker_ptr(nullptr),
      search_reporter_ptr(nullptr),
      input_thread(),
      input_state(nullptr),
      is_running(true),
      in_stream(input),
      out_stream(output) {}

void Uci::UciEngine::loop() {
  send_startup_msg();
  start_input_reader();

  while (is_running) {
    poll_search_worker();

    std::vector<std::string> line;
    if (wait_and_pop_command(line, std::chrono::milliseconds(5))) {
      dispatch(line);
      continue;
    }

    if (input_state && input_state->input_eof.load()) {
      request_stop_search_worker();
      poll_search_worker();
      if (!search_worker_ptr) {
        is_running = false;
      }
    }
  }

  reset_search_worker();
  stop_input_reader_loop();
}

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
  } else if (line.front() == "d") {
    handle_display();
  } else if (line.front() == "help") {
    handle_help();
  } else if (line.front() == "bench") {
    handle_bench(line);
  }
  // unknown lines are discarded silently following uci rules
};

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

  SearchLimits limits = SearchLimits::from_uci_cmd(line);

  search_reporter_ptr = std::make_unique<UciReporter>(out_stream);
  search_worker_ptr = std::make_unique<SearchWorker>(board, limits);
  assert(search_worker_ptr != nullptr);
  search_worker_ptr->start();
}

void Uci::UciEngine::handle_stop() { request_stop_search_worker(); }

void Uci::UciEngine::handle_quit() {
  request_stop_search_worker();
  is_running = false;
}

void Uci::UciEngine::reset_search_worker() {
  if (search_worker_ptr) {
    search_worker_ptr->stop();
    emit_search_reports();
    search_worker_ptr.reset();
  }

  search_reporter_ptr.reset();
  assert(search_worker_ptr == nullptr);
}

void Uci::UciEngine::request_stop_search_worker() {
  if (search_worker_ptr) {
    search_worker_ptr->request_stop();
  }
}

void Uci::UciEngine::finalize_search_worker_if_done() {
  if (!search_worker_ptr || !search_worker_ptr->is_finished()) {
    return;
  }

  search_worker_ptr->wait();
  emit_search_reports();
  search_worker_ptr.reset();
  search_reporter_ptr.reset();
}

void Uci::UciEngine::emit_search_reports() {
  if (!search_worker_ptr || !search_reporter_ptr) {
    return;
  }

  const auto reports = search_worker_ptr->drain_reports();
  for (const SearchReport &report : reports) {
    if (report.kind == SearchReportKind::Iteration) {
      search_reporter_ptr->on_iteration(report.best, report.depth, report.stats);
    } else if (report.kind == SearchReportKind::Finish) {
      search_reporter_ptr->on_finish(report.best, report.stats);
    }
  }
}

void Uci::UciEngine::poll_search_worker() {
  emit_search_reports();
  finalize_search_worker_if_done();
}

void Uci::UciEngine::input_reader_loop(std::istream &input_stream, std::shared_ptr<InputState> state) {
  std::string input_str;
  while (!state->stop_input_reader.load() && std::getline(input_stream, input_str)) {
    std::vector<std::string> line = split(input_str);
    {
      std::lock_guard<std::mutex> lock(state->commands_mutex);
      state->pending_commands.push_back(std::move(line));
    }
    state->commands_cv.notify_one();
  }

  state->input_eof.store(true);
  state->commands_cv.notify_all();
}

void Uci::UciEngine::start_input_reader() {
  input_state = std::make_shared<InputState>();
  input_thread = std::thread(&UciEngine::input_reader_loop, std::ref(in_stream), input_state);
}

void Uci::UciEngine::stop_input_reader_loop() {
  if (!input_state) {
    return;
  }

  input_state->stop_input_reader.store(true);
  input_state->commands_cv.notify_all();

  if (!input_thread.joinable()) {
    input_state.reset();
    return;
  }

  if (input_state->input_eof.load()) {
    input_thread.join();
  } else {
    input_thread.detach();
  }

  input_state.reset();
}

bool Uci::UciEngine::wait_and_pop_command(std::vector<std::string> &line, std::chrono::milliseconds timeout) {
  if (!input_state) {
    return false;
  }

  std::unique_lock<std::mutex> lock(input_state->commands_mutex);
  input_state->commands_cv.wait_for(
      lock, timeout, [state = input_state] { return !state->pending_commands.empty() || state->input_eof.load(); });

  if (input_state->pending_commands.empty()) {
    return false;
  }

  line = std::move(input_state->pending_commands.front());
  input_state->pending_commands.pop_front();
  return true;
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

void Uci::UciEngine::handle_bench(std::vector<std::string> &line) {
  reset_search_worker();

  SearchLimits limits = SearchLimits::from_uci_cmd(line);

  search_reporter_ptr = std::make_unique<BenchReporter>(out_stream);
  search_worker_ptr = std::make_unique<SearchWorker>(board, limits);
  assert(search_worker_ptr != nullptr);
  search_worker_ptr->start();
}
