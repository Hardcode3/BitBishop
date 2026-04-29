#include <algorithm>
#include <bitbishop/interface/search_worker.hpp>
#include <bitbishop/tools/time_guard.hpp>
#include <limits>

namespace {

constexpr int DEFAULT_MOVES_TO_GO = 20;
constexpr int MIN_THINK_TIME_MS = 1;
constexpr int SAFETY_BUFFER_MS = 50;
constexpr int MAX_DEPTH = 50;

[[nodiscard]] int estimate_clock_think_time_ms(int remaining_ms, int increment_ms) {
  remaining_ms = std::max(remaining_ms, 0);
  increment_ms = std::max(increment_ms, 0);

  if (remaining_ms <= MIN_THINK_TIME_MS) {
    return MIN_THINK_TIME_MS;
  }

  const int reserve_ms = std::min(remaining_ms - MIN_THINK_TIME_MS, std::max(SAFETY_BUFFER_MS, remaining_ms / 20));
  const int spendable_ms = std::max(MIN_THINK_TIME_MS, remaining_ms - reserve_ms);
  const int base_ms = spendable_ms / DEFAULT_MOVES_TO_GO;
  const int increment_bonus_ms = (increment_ms * 3) / 4;

  return std::clamp(base_ms + increment_bonus_ms, MIN_THINK_TIME_MS, spendable_ms);
}

}  // namespace

Uci::SearchLimits Uci::SearchLimits::from_uci_cmd(const std::vector<std::string>& line) {
  SearchLimits limits;

  for (std::size_t i = 1; i < line.size(); ++i) {
    const auto& tok = line[i];

    auto read = [&](std::optional<int>& target) {
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

  if (!limits.depth && !limits.has_time_limit() && !limits.infinite) {
    limits.infinite = true;
  }

  return limits;
}

bool Uci::SearchLimits::has_time_limit() const {
  return movetime.has_value() || wtime.has_value() || btime.has_value() || winc.has_value() || binc.has_value();
}

std::optional<int> Uci::SearchLimits::think_time_ms(Color side_to_move) const {
  if (movetime.has_value()) {
    return std::max(*movetime, MIN_THINK_TIME_MS);
  }

  const std::optional<int>& remaining_opt = (side_to_move == Color::WHITE) ? wtime : btime;
  if (!remaining_opt.has_value()) {
    return std::nullopt;
  }

  const std::optional<int>& increment_opt = (side_to_move == Color::WHITE) ? winc : binc;
  return estimate_clock_think_time_ms(*remaining_opt, increment_opt.value_or(0));
}

Uci::SearchWorker::SearchWorker(Board board, SearchLimits limits)
    : board(board), position(Position(this->board)), limits(limits) {}

Uci::SearchWorker::~SearchWorker() { stop(); }

void Uci::SearchWorker::push_report(SearchReport report) {
  std::lock_guard<std::mutex> lock(reports_mutex);
  reports.push_back(report);
}

void Uci::SearchWorker::run() {
  using namespace Search;

  // scope_exit function  is not yet shipped in all stdlibs (linux is fine, not windows nor macos - 2026-04-29)
  // const auto guard = std::experimental::scope_exit([this] { finished.store(true); });
  // Replacing it by a struct with custom destructor.
  struct FinishGuard {
    std::atomic<bool>& finished_ref;
    ~FinishGuard() { finished_ref.store(true); }
  } guard{finished};

  SearchStats stats{};
  SearchReport current_best_report{.kind = SearchReportKind::Iteration};

  const auto side = board.get_side_to_move();
  const auto think_time = limits.infinite ? std::nullopt : limits.think_time_ms(side);
  std::optional<Tools::TimeGuard> timeguard;
  if (think_time) {
    timeguard.emplace(stop_flag, std::chrono::milliseconds(*think_time));
  }

  auto perform_search_at_depth = [&](int depth) {
    auto result = negamax(position, depth, ALPHA_INIT, BETA_INIT, 0, stats, &stop_flag);

    if (!stop_flag.load()) {
      current_best_report.best = result;
      current_best_report.depth = depth;
      current_best_report.stats = stats;
      push_report(current_best_report);
      return true;
    }
    return false;
  };

  if (limits.depth && !limits.infinite && !think_time) {
    // Case: Fixed depth search (e.g., "go depth 10")
    perform_search_at_depth(*limits.depth);
  } else {
    // Case: Iterative deepening (Infinite or Time-limited)
    for (int depth = 1; depth <= MAX_DEPTH && !stop_flag.load(); ++depth) {
      if (!perform_search_at_depth(depth)) {
        break;
      }
    }
  }

  current_best_report.kind = SearchReportKind::Finish;
  push_report(current_best_report);
}

void Uci::SearchWorker::start() {
  stop();
  stop_flag.store(false);
  finished.store(false);
  {
    std::lock_guard<std::mutex> lock(reports_mutex);
    reports.clear();
  }
  worker = std::thread(&SearchWorker::run, this);
}

void Uci::SearchWorker::wait() {
  if (worker.joinable()) {
    worker.join();
  }
}

void Uci::SearchWorker::request_stop() { stop_flag.store(true); }

void Uci::SearchWorker::stop() {
  request_stop();
  wait();
}

std::vector<Uci::SearchReport> Uci::SearchWorker::drain_reports() {
  std::lock_guard<std::mutex> lock(reports_mutex);
  std::vector<SearchReport> drained;
  drained.swap(reports);
  return drained;
}
