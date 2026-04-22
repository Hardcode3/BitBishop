#pragma once

#include <bitbishop/engine/search.hpp>
#include <chrono>

struct SearchReporter {
  virtual ~SearchReporter() = default;

  virtual void on_iteration(const Search::BestMove& best, int depth, const Search::SearchStats& stats) {}

  virtual void on_finish(const Search::BestMove& best, const Search::SearchStats& stats) = 0;
};

struct UciReporter : SearchReporter {
  std::ostream& out_stream;

  UciReporter(std::ostream& out) : out_stream(out) {}

  void on_finish(const Search::BestMove& best, const Search::SearchStats& stats) override {
    const std::string best_move_str = (best.move) ? (*best.move).to_uci() : "0000";
    out_stream << "bestmove " << best_move_str << "\n";
    out_stream << std::flush;
  }
};

struct BenchReporter : SearchReporter {
  std::ostream& out_stream;
  std::chrono::steady_clock::time_point start;

  BenchReporter(std::ostream& out) : out_stream(out), start(std::chrono::steady_clock::now()) {}

  void on_finish(const Search::BestMove& best, const Search::SearchStats& stats) override {
    auto end = std::chrono::steady_clock::now();
    double seconds = std::chrono::duration<double>(end - start).count();

    uint64_t total = stats.negamax_nodes + stats.quiescence_nodes;
    uint64_t nps = (seconds > 0.0) ? static_cast<uint64_t>(static_cast<double>(total) / seconds) : 0;

    out_stream << "bench nodes " << total << " time " << seconds << "s" << " nps " << nps << "\n" << std::flush;
  }
};
