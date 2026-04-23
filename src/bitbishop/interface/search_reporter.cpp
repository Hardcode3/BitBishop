#include <bitbishop/interface/search_reporter.hpp>

UciReporter::UciReporter(std::ostream& out) : out_stream(out) {}

void UciReporter::on_finish(const Search::BestMove& best, const Search::SearchStats& stats) {
  const std::string best_move_str = (best.move) ? (*best.move).to_uci() : "0000";
  out_stream << "bestmove " << best_move_str << "\n";
  out_stream << std::flush;
}

BenchReporter::BenchReporter(std::ostream& out) : out_stream(out), start(std::chrono::steady_clock::now()) {}

void BenchReporter::on_finish(const Search::BestMove& best, const Search::SearchStats& stats) {
  auto end = std::chrono::steady_clock::now();
  double seconds = std::chrono::duration<double>(end - start).count();

  uint64_t total = stats.negamax_nodes + stats.quiescence_nodes;
  uint64_t nps = (seconds > 0.0) ? static_cast<uint64_t>(static_cast<double>(total) / seconds) : 0;

  out_stream << "bench nodes " << total << " negamax_nodes " << stats.negamax_nodes << " quiescence_nodes "
             << stats.quiescence_nodes << " time(s) " << seconds << "s" << " nps " << nps << "\n"
             << std::flush;
}
