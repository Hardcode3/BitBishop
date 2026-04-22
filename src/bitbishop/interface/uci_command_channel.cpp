#include <bitbishop/interface/uci_command_channel.hpp>

Uci::UciCommandChannel::UciCommandChannel(std::istream& input_stream)
    : input_stream(input_stream), reader_thread(), state(nullptr) {}

Uci::UciCommandChannel::~UciCommandChannel() { stop(); }

void Uci::UciCommandChannel::reader_loop(std::istream& input_stream, std::shared_ptr<State> state) {
  std::string line;
  while (!state->stop_requested.load() && std::getline(input_stream, line)) {
    {
      std::lock_guard<std::mutex> lock(state->lines_mutex);
      state->pending_lines.push_back(std::move(line));
    }
    state->lines_cv.notify_one();
  }

  state->eof_reached.store(true);
  state->lines_cv.notify_all();
}

void Uci::UciCommandChannel::start() {
  stop();

  state = std::make_shared<State>();
  reader_thread = std::thread(&UciCommandChannel::reader_loop, std::ref(input_stream), state);
}

void Uci::UciCommandChannel::stop() {
  if (!state) {
    return;
  }

  state->stop_requested.store(true);
  state->lines_cv.notify_all();

  if (!reader_thread.joinable()) {
    state.reset();
    return;
  }

  if (state->eof_reached.load()) {
    reader_thread.join();
  } else {
    reader_thread.detach();
  }

  state.reset();
}

bool Uci::UciCommandChannel::wait_and_pop_line(std::string& line, std::chrono::milliseconds timeout) {
  if (!state) {
    return false;
  }

  std::unique_lock<std::mutex> lock(state->lines_mutex);
  state->lines_cv.wait_for(lock, timeout, [channel_state = state] {
    return !channel_state->pending_lines.empty() || channel_state->eof_reached.load();
  });

  if (state->pending_lines.empty()) {
    return false;
  }

  line = std::move(state->pending_lines.front());
  state->pending_lines.pop_front();
  return true;
}

bool Uci::UciCommandChannel::eof() const { return state && state->eof_reached.load(); }
