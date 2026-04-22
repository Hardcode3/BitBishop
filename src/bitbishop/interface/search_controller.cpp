#include <bitbishop/interface/search_controller.hpp>

Uci::SearchLimits Uci::SearchLimits::from_uci_cmd(std::vector<std::string>& line) {
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

  if (!limits.depth) {
    limits.infinite = true;  // Only depth and infinite limits are supported for now
  }

  return limits;
}

Uci::SearchWorker::SearchWorker(Board board, SearchLimits limits)
    : board(board), position(Position(this->board)), limits(limits) {}

Uci::SearchWorker::~SearchWorker() { stop(); }

void Uci::SearchWorker::push_report(SearchReport report) {
  std::lock_guard<std::mutex> lock(reports_mutex);
  reports.push_back(std::move(report));
}

void Uci::SearchWorker::run() {
  using namespace Search;
  struct FinishGuard {
    std::atomic<bool>& finished_ref;
    ~FinishGuard() { finished_ref.store(true); }
  } guard{finished};

  SearchStats stats{};
  BestMove best;
  BestMove last_best;

  if (limits.infinite) {
    for (int current_depth = 1; !stop_flag.load(); ++current_depth) {
      best = negamax(position, current_depth, ALPHA_INIT, BETA_INIT, 0, stats, &stop_flag);
      if (!stop_flag.load()) {
        last_best = best;
        push_report(SearchReport{
            .kind = SearchReportKind::Iteration,
            .best = last_best,
            .depth = current_depth,
            .stats = stats,
        });
      }
    }
  } else if (limits.depth) {
    best = negamax(position, *limits.depth, ALPHA_INIT, BETA_INIT, 0, stats, &stop_flag);
    if (!stop_flag.load()) {
      push_report(SearchReport{
          .kind = SearchReportKind::Iteration,
          .best = best,
          .depth = *limits.depth,
          .stats = stats,
      });
    }
  }

  const BestMove& final = (last_best.move) ? last_best : best;
  push_report(SearchReport{
      .kind = SearchReportKind::Finish,
      .best = final,
      .depth = limits.depth.value_or(0),
      .stats = stats,
  });
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
