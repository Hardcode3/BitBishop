#include <bitbishop/interface/search_session.hpp>

#include <cassert>

Uci::SearchSession::SearchSession(std::ostream& out_stream)
    : out_stream(out_stream), worker(nullptr), reporter(nullptr) {}

Uci::SearchSession::~SearchSession() { stop_and_join(); }

void Uci::SearchSession::start_go(Board board, SearchLimits limits) {
  stop_and_join();

  reporter = std::make_unique<UciReporter>(out_stream);
  worker = std::make_unique<SearchWorker>(board, limits);
  assert(worker != nullptr);
  worker->start();
}

void Uci::SearchSession::start_bench(Board board, SearchLimits limits) {
  stop_and_join();

  if (limits.infinite) {
    limits.depth = 10;  // NOLINT(readability-magic-numbers)
    limits.infinite = false;
  }

  reporter = std::make_unique<BenchReporter>(out_stream);
  worker = std::make_unique<SearchWorker>(board, limits);
  assert(worker != nullptr);
  worker->start();
}

void Uci::SearchSession::request_stop() {
  if (worker) {
    worker->request_stop();
  }
}

void Uci::SearchSession::emit_reports() {
  if (!worker || !reporter) {
    return;
  }

  const auto reports = worker->drain_reports();
  for (const SearchReport& report : reports) {
    if (report.kind == SearchReportKind::Iteration) {
      reporter->on_iteration(report.best, report.depth, report.stats);
    } else if (report.kind == SearchReportKind::Finish) {
      reporter->on_finish(report.best, report.stats);
    }
  }
}

void Uci::SearchSession::finalize_if_done() {
  if (!worker || !worker->is_finished()) {
    return;
  }

  worker->wait();
  emit_reports();
  worker.reset();
  reporter.reset();
}

void Uci::SearchSession::poll() {
  emit_reports();
  finalize_if_done();
}

void Uci::SearchSession::stop_and_join() {
  if (worker) {
    worker->stop();
    emit_reports();
    worker.reset();
  }
  reporter.reset();
}
