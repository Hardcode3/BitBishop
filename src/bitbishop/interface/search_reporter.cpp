#include <bitbishop/interface/search_reporter.hpp>
#include <utility>

UciReporter::UciReporter(std::ostream& out) : out_stream(out) {}

void UciReporter::on_finish(const Search::BestMove& best, const Search::SearchStats& stats) {
  const std::string best_move_str = (best.move) ? (*best.move).to_uci() : "0000";
  out_stream << "bestmove " << best_move_str << "\n";
  out_stream << std::flush;
}

BenchReporter::BenchReporter(std::ostream& out) : BenchReporter(out, Clock::now) {}

BenchReporter::BenchReporter(std::ostream& out, std::function<Clock::time_point()> now_fn)
    : out_stream(out), now(std::move(now_fn)), start(now()) {}

void BenchReporter::on_finish(const Search::BestMove& best, const Search::SearchStats& stats) {
  using Duration = std::chrono::duration<double>;

  auto end = now();
  double seconds = Duration(end - start).count();

  uint64_t total = stats.negamax_nodes + stats.quiescence_nodes;
  uint64_t nps = (seconds > 0.0) ? static_cast<uint64_t>(static_cast<double>(total) / seconds) : 0;

  out_stream << "bench nodes " << total << " negamax_nodes " << stats.negamax_nodes << " quiescence_nodes "
             << stats.quiescence_nodes << " time(s) " << seconds << "s" << " nps " << nps << "\n"
             << std::flush;
}
